import {
  useEffect,
  useRef,
  useState,
  type FormEvent,
  type KeyboardEvent,
} from "react";
import { WS_BASE_URL } from "./api/config";
import "./App.css";
import {
  getCurrentUser,
  login,
  logout,
  refreshAccessToken,
  register,
  type User,
} from "./api/auth";
import {
  createConversation,
  deleteMessage,
  editMessage,
  getConversationMessages,
  getConversations,
  markConversationAsRead,
  type Conversation,
  type Message,
} from "./api/conversations";
import { searchUsers } from "./api/users";
import AuthPage from "./components/AuthPage";
import ChatWindow from "./components/ChatWindow";
import Sidebar from "./components/Sidebar";
import {
  getConversationName,
  getOtherParticipant,
  sortConversationsByUpdatedAt,
} from "./utils/chat";

function App() {
  const [currentUser, setCurrentUser] = useState<User | null>(null);
  const [accessToken, setAccessToken] = useState<string | null>(null);

  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] =
    useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);

  const socketRef = useRef<WebSocket | null>(null);
  const notificationSocketRef = useRef<WebSocket | null>(null);
  const messagesEndRef = useRef<HTMLDivElement | null>(null);
  const typingTimeoutRef = useRef<number | null>(null);

  const [authMode, setAuthMode] = useState<"login" | "register">("login");

  const [username, setUsername] = useState("pavlo");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [passwordConfirm, setPasswordConfirm] = useState("");

  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [isMessagesLoading, setIsMessagesLoading] = useState(false);
  const [isAuthChecking, setIsAuthChecking] = useState(true);

  const [newMessage, setNewMessage] = useState("");
  const [messageError, setMessageError] = useState("");
  const [isSending, setIsSending] = useState(false);
  const [typingUser, setTypingUser] = useState<User | null>(null);
  const [onlineUserIds, setOnlineUserIds] = useState<number[]>([]);

  const [userSearchQuery, setUserSearchQuery] = useState("");
  const [searchResults, setSearchResults] = useState<User[]>([]);
  const [isSearchingUsers, setIsSearchingUsers] = useState(false);
  const [userSearchError, setUserSearchError] = useState("");

  const [editingMessageId, setEditingMessageId] = useState<number | null>(null);
  const [editingMessageText, setEditingMessageText] = useState("");
  const [isEditingMessage, setIsEditingMessage] = useState(false);
  const [isDeletingMessageId, setIsDeletingMessageId] = useState<number | null>(
    null
  );

  function markConversationReadInState(conversationId: number) {
    setConversations((previousConversations) =>
      previousConversations.map((conversation) =>
        conversation.id === conversationId
          ? {
              ...conversation,
              unread_count: 0,
            }
          : conversation
      )
    );

    setSelectedConversation((previousConversation) =>
      previousConversation?.id === conversationId
        ? {
            ...previousConversation,
            unread_count: 0,
          }
        : previousConversation
    );
  }

  async function loadMessages(
    token: string,
    conversationId: number,
    shouldMarkAsRead = true
  ) {
    setIsMessagesLoading(true);

    try {
      const messagesData = await getConversationMessages(token, conversationId);
      setMessages(messagesData);

      if (shouldMarkAsRead) {
        await markConversationAsRead(token, conversationId);
        markConversationReadInState(conversationId);
      }
    } catch {
      setMessages([]);
    } finally {
      setIsMessagesLoading(false);
    }
  }

  async function refreshConversations(token: string) {
    try {
      const conversationsData = await getConversations(token);
      const sortedConversations =
        sortConversationsByUpdatedAt(conversationsData);

      setConversations(sortedConversations);

      setSelectedConversation((previousConversation) => {
        if (!previousConversation) {
          return previousConversation;
        }

        return (
          sortedConversations.find(
            (conversation) => conversation.id === previousConversation.id
          ) ?? previousConversation
        );
      });
    } catch {
      console.error("Failed to refresh conversations.");
    }
  }

  async function loadUserData(token: string) {
    const user = await getCurrentUser(token);
    setCurrentUser(user);
    setAccessToken(token);

    const conversationsData = await getConversations(token);
    setConversations(sortConversationsByUpdatedAt(conversationsData));

    setSelectedConversation(null);
    setMessages([]);
  }

  function clearSession() {
    localStorage.removeItem("accessToken");
    localStorage.removeItem("refreshToken");

    if (socketRef.current) {
      socketRef.current.close();
      socketRef.current = null;
    }

    if (notificationSocketRef.current) {
      notificationSocketRef.current.close();
      notificationSocketRef.current = null;
    }

    setCurrentUser(null);
    setAccessToken(null);
    setConversations([]);
    setSelectedConversation(null);
    setMessages([]);
    setSearchResults([]);
    setUserSearchQuery("");
    setNewMessage("");
    setMessageError("");
    setTypingUser(null);
    setOnlineUserIds([]);
  }

  async function handleLogout() {
    const savedRefreshToken = localStorage.getItem("refreshToken");

    if (socketRef.current) {
      socketRef.current.close();
      socketRef.current = null;
    }

    if (notificationSocketRef.current) {
      notificationSocketRef.current.close();
      notificationSocketRef.current = null;
    }

    if (accessToken && savedRefreshToken) {
      try {
        await logout(accessToken, savedRefreshToken);
      } catch {
        console.error("Logout request failed.");
      }
    }

    clearSession();
  }

  function updateMessageInState(updatedMessage: Message) {
    setMessages((previousMessages) =>
      previousMessages.map((message) =>
        message.id === updatedMessage.id ? updatedMessage : message
      )
    );

    setConversations((previousConversations) =>
      sortConversationsByUpdatedAt(
        previousConversations.map((conversation) =>
          conversation.last_message?.id === updatedMessage.id
            ? {
                ...conversation,
                last_message: updatedMessage,
                updated_at: updatedMessage.updated_at,
              }
            : conversation
        )
      )
    );

    setSelectedConversation((previousConversation) =>
      previousConversation?.last_message?.id === updatedMessage.id
        ? {
            ...previousConversation,
            last_message: updatedMessage,
            updated_at: updatedMessage.updated_at,
          }
        : previousConversation
    );
  }

  function sendReadStatus() {
    const socket = socketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "read",
      })
    );
  }

  function sendTypingStatus(isTyping: boolean) {
    const socket = socketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "typing",
        is_typing: isTyping,
      })
    );
  }

  useEffect(() => {
    async function restoreSession() {
      const savedAccessToken = localStorage.getItem("accessToken");
      const savedRefreshToken = localStorage.getItem("refreshToken");

      if (!savedAccessToken && !savedRefreshToken) {
        setIsAuthChecking(false);
        return;
      }

      try {
        if (savedAccessToken) {
          await loadUserData(savedAccessToken);
          return;
        }

        if (savedRefreshToken) {
          const refreshedToken = await refreshAccessToken(savedRefreshToken);
          localStorage.setItem("accessToken", refreshedToken.access);
          await loadUserData(refreshedToken.access);
        }
      } catch {
        if (!savedRefreshToken) {
          clearSession();
          return;
        }

        try {
          const refreshedToken = await refreshAccessToken(savedRefreshToken);
          localStorage.setItem("accessToken", refreshedToken.access);
          await loadUserData(refreshedToken.access);
        } catch {
          clearSession();
        }
      } finally {
        setIsAuthChecking(false);
      }
    }

    restoreSession();
  }, []);

  useEffect(() => {
    if (!accessToken || !currentUser) {
      return;
    }

    const notificationSocket = new WebSocket(
      `${WS_BASE_URL}/notifications/?token=${accessToken}`
    );

    notificationSocketRef.current = notificationSocket;

    notificationSocket.onmessage = async (event) => {
      const data = JSON.parse(event.data);

      if (data.type === "sidebar_message") {
        await refreshConversations(accessToken);
        return;
      }

      if (data.type === "online_users") {
        setOnlineUserIds(data.user_ids);
        return;
      }

      if (data.type === "online_status") {
        const user = data.user as User;

        setOnlineUserIds((previousOnlineUserIds) => {
          if (data.is_online) {
            if (previousOnlineUserIds.includes(user.id)) {
              return previousOnlineUserIds;
            }

            return [...previousOnlineUserIds, user.id];
          }

          return previousOnlineUserIds.filter((userId) => userId !== user.id);
        });
      }
    };

    notificationSocket.onerror = () => {
      console.error("Notification WebSocket connection error.");
    };

    return () => {
      notificationSocket.close();

      if (notificationSocketRef.current === notificationSocket) {
        notificationSocketRef.current = null;
      }
    };
  }, [accessToken, currentUser?.id]);

  useEffect(() => {
    if (!accessToken || !selectedConversation) {
      return;
    }

    const websocketUrl = `${WS_BASE_URL}/conversations/${selectedConversation.id}/?token=${accessToken}`;
    const socket = new WebSocket(websocketUrl);

    socketRef.current = socket;

    socket.onopen = () => {
      sendReadStatus();
    };

    socket.onmessage = async (event) => {
      const data = JSON.parse(event.data);

      if (data.type === "message") {
        const receivedMessage = data.message as Message;

        setMessages((previousMessages) => {
          const alreadyExists = previousMessages.some(
            (message) => message.id === receivedMessage.id
          );

          if (alreadyExists) {
            return previousMessages;
          }

          return [...previousMessages, receivedMessage];
        });

        setConversations((previousConversations) =>
          sortConversationsByUpdatedAt(
            previousConversations.map((conversation) =>
              conversation.id === receivedMessage.conversation
                ? {
                    ...conversation,
                    last_message: receivedMessage,
                    updated_at: receivedMessage.created_at,
                  }
                : conversation
            )
          )
        );

        setSelectedConversation((previousConversation) =>
          previousConversation?.id === receivedMessage.conversation
            ? {
                ...previousConversation,
                last_message: receivedMessage,
                updated_at: receivedMessage.created_at,
                unread_count: 0,
              }
            : previousConversation
        );

        if (
          accessToken &&
          selectedConversation.id === receivedMessage.conversation &&
          receivedMessage.sender.id !== currentUser?.id
        ) {
          try {
            await markConversationAsRead(accessToken, selectedConversation.id);
            markConversationReadInState(selectedConversation.id);
            sendReadStatus();
          } catch {
            console.error("Failed to mark WebSocket message as read.");
          }
        }

        return;
      }

      if (data.type === "read") {
        const reader = data.user as User;

        if (reader.id === currentUser?.id) {
          return;
        }

        setMessages((previousMessages) =>
          previousMessages.map((message) =>
            message.sender.id === currentUser?.id && !message.is_deleted
              ? {
                  ...message,
                  is_read: true,
                }
              : message
          )
        );

        return;
      }

      if (data.type === "typing") {
        const receivedTypingUser = data.user as User;

        if (receivedTypingUser.id === currentUser?.id) {
          return;
        }

        if (data.is_typing) {
          setTypingUser(receivedTypingUser);
        } else {
          setTypingUser(null);
        }

        return;
      }

      if (data.type === "error") {
        setMessageError(data.detail);
      }
    };

    socket.onerror = () => {
      setMessageError("WebSocket connection error.");
    };

    return () => {
      if (typingTimeoutRef.current) {
        window.clearTimeout(typingTimeoutRef.current);
        typingTimeoutRef.current = null;
      }

      setTypingUser(null);

      socket.close();

      if (socketRef.current === socket) {
        socketRef.current = null;
      }
    };
  }, [accessToken, selectedConversation?.id, currentUser?.id]);

  useEffect(() => {
    messagesEndRef.current?.scrollIntoView({
      behavior: "smooth",
    });
  }, [messages, selectedConversation?.id]);

  async function handleLogin(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    setError("");
    setIsLoading(true);

    try {
      const tokens = await login(username, password);

      localStorage.setItem("accessToken", tokens.access);
      localStorage.setItem("refreshToken", tokens.refresh);

      await loadUserData(tokens.access);
    } catch {
      setError("Invalid username or password.");
    } finally {
      setIsLoading(false);
    }
  }

  async function handleRegister(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    const trimmedUsername = username.trim();
    const trimmedEmail = email.trim();
    const trimmedPassword = password.trim();
    const trimmedPasswordConfirm = passwordConfirm.trim();

    if (!trimmedUsername || !trimmedEmail || !trimmedPassword) {
      setError("Username, email and password are required.");
      return;
    }

    if (trimmedPassword !== trimmedPasswordConfirm) {
      setError("Passwords do not match.");
      return;
    }

    setError("");
    setIsLoading(true);

    try {
      const tokens = await register(
        trimmedUsername,
        trimmedEmail,
        trimmedPassword,
        trimmedPasswordConfirm
      );

      localStorage.setItem("accessToken", tokens.access);
      localStorage.setItem("refreshToken", tokens.refresh);

      await loadUserData(tokens.access);
    } catch (error) {
      if (error instanceof Error) {
        setError(error.message);
      } else {
        setError("Failed to create account.");
      }
    } finally {
      setIsLoading(false);
    }
  }

  async function handleSelectConversation(conversation: Conversation) {
    if (!accessToken) {
      return;
    }

    setSelectedConversation(conversation);
    setEditingMessageId(null);
    setEditingMessageText("");
    setTypingUser(null);
    setMessageError("");

    await loadMessages(accessToken, conversation.id);
    await refreshConversations(accessToken);
  }

  async function handleSearchUsers(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    if (!accessToken) {
      return;
    }

    const query = userSearchQuery.trim();

    if (!query) {
      setSearchResults([]);
      setUserSearchError("");
      return;
    }

    setIsSearchingUsers(true);
    setUserSearchError("");

    try {
      const users = await searchUsers(accessToken, query);
      setSearchResults(users);
    } catch {
      setSearchResults([]);
      setUserSearchError("Failed to search users.");
    } finally {
      setIsSearchingUsers(false);
    }
  }

  async function handleStartConversation(user: User) {
    if (!accessToken) {
      return;
    }

    setUserSearchError("");

    try {
      const conversation = await createConversation(accessToken, user.id);

      setConversations((previousConversations) => {
        const alreadyExists = previousConversations.some(
          (item) => item.id === conversation.id
        );

        if (alreadyExists) {
          return sortConversationsByUpdatedAt(
            previousConversations.map((item) =>
              item.id === conversation.id ? conversation : item
            )
          );
        }

        return sortConversationsByUpdatedAt([
          conversation,
          ...previousConversations,
        ]);
      });

      setSelectedConversation(conversation);
      await loadMessages(accessToken, conversation.id);

      setUserSearchQuery("");
      setSearchResults([]);
    } catch {
      setUserSearchError("Failed to create conversation.");
    }
  }

  function handleNewMessageChange(value: string) {
    setNewMessage(value);

    if (!selectedConversation) {
      return;
    }

    sendTypingStatus(true);

    if (typingTimeoutRef.current) {
      window.clearTimeout(typingTimeoutRef.current);
    }

    typingTimeoutRef.current = window.setTimeout(() => {
      sendTypingStatus(false);
    }, 1200);
  }

  async function handleSendMessage(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    if (!selectedConversation) {
      return;
    }

    const text = newMessage.trim();

    if (!text) {
      setMessageError("Message text cannot be empty.");
      return;
    }

    const socket = socketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      setMessageError("WebSocket is not connected.");
      return;
    }

    setMessageError("");
    setIsSending(true);

    try {
      socket.send(JSON.stringify({ text }));
      sendTypingStatus(false);

      if (typingTimeoutRef.current) {
        window.clearTimeout(typingTimeoutRef.current);
        typingTimeoutRef.current = null;
      }

      setNewMessage("");
    } catch {
      setMessageError("Failed to send message.");
    } finally {
      setIsSending(false);
    }
  }

  function handleMessageKeyDown(event: KeyboardEvent<HTMLTextAreaElement>) {
    if (event.key === "Enter" && !event.shiftKey) {
      event.preventDefault();

      const form = event.currentTarget.form;

      if (form) {
        form.requestSubmit();
      }
    }
  }

  function handleStartEditMessage(message: Message) {
    setEditingMessageId(message.id);
    setEditingMessageText(message.text);
    setMessageError("");
  }

  function handleCancelEditMessage() {
    setEditingMessageId(null);
    setEditingMessageText("");
  }

  async function handleSaveEditedMessage(messageId: number) {
    if (!accessToken) {
      return;
    }

    const text = editingMessageText.trim();

    if (!text) {
      setMessageError("Message text cannot be empty.");
      return;
    }

    setIsEditingMessage(true);
    setMessageError("");

    try {
      const updatedMessage = await editMessage(accessToken, messageId, text);
      updateMessageInState(updatedMessage);
      setEditingMessageId(null);
      setEditingMessageText("");
    } catch {
      setMessageError("Failed to edit message.");
    } finally {
      setIsEditingMessage(false);
    }
  }

  async function handleDeleteMessage(messageId: number) {
    if (!accessToken) {
      return;
    }

    setIsDeletingMessageId(messageId);
    setMessageError("");

    try {
      const deletedMessage = await deleteMessage(accessToken, messageId);
      updateMessageInState(deletedMessage);

      if (editingMessageId === messageId) {
        setEditingMessageId(null);
        setEditingMessageText("");
      }
    } catch {
      setMessageError("Failed to delete message.");
    } finally {
      setIsDeletingMessageId(null);
    }
  }

  if (isAuthChecking) {
    return (
      <div className="auth-page">
        <div className="auth-card">
          <h1>Messenger</h1>
          <p>Loading...</p>
        </div>
      </div>
    );
  }

  if (!currentUser || !accessToken) {
    return (
      <AuthPage
        authMode={authMode}
        setAuthMode={setAuthMode}
        username={username}
        setUsername={setUsername}
        email={email}
        setEmail={setEmail}
        password={password}
        setPassword={setPassword}
        passwordConfirm={passwordConfirm}
        setPasswordConfirm={setPasswordConfirm}
        error={error}
        setError={setError}
        isLoading={isLoading}
        handleLogin={handleLogin}
        handleRegister={handleRegister}
      />
    );
  }

  const selectedConversationName = selectedConversation
    ? getConversationName(selectedConversation, currentUser)
    : "No conversation selected";

  const selectedConversationUser = selectedConversation
    ? getOtherParticipant(selectedConversation, currentUser) ?? null
    : null;

  const selectedConversationUserIsOnline = selectedConversationUser
    ? onlineUserIds.includes(selectedConversationUser.id)
    : false;

  return (
    <div className="app">
      <Sidebar
        currentUser={currentUser}
        onlineUserIds={onlineUserIds}
        conversations={conversations}
        selectedConversation={selectedConversation}
        userSearchQuery={userSearchQuery}
        setUserSearchQuery={setUserSearchQuery}
        searchResults={searchResults}
        isSearchingUsers={isSearchingUsers}
        userSearchError={userSearchError}
        handleLogout={handleLogout}
        handleSearchUsers={handleSearchUsers}
        handleStartConversation={handleStartConversation}
        handleSelectConversation={handleSelectConversation}
      />

      <ChatWindow
        currentUser={currentUser}
        selectedConversation={selectedConversation}
        selectedConversationName={selectedConversationName}
        selectedConversationUser={selectedConversationUser}
        selectedConversationUserIsOnline={selectedConversationUserIsOnline}
        messages={messages}
        isMessagesLoading={isMessagesLoading}
        typingUser={typingUser}
        messageError={messageError}
        newMessage={newMessage}
        isSending={isSending}
        editingMessageId={editingMessageId}
        editingMessageText={editingMessageText}
        setEditingMessageText={setEditingMessageText}
        isEditingMessage={isEditingMessage}
        isDeletingMessageId={isDeletingMessageId}
        messagesEndRef={messagesEndRef}
        handleNewMessageChange={handleNewMessageChange}
        handleMessageKeyDown={handleMessageKeyDown}
        handleSendMessage={handleSendMessage}
        handleStartEditMessage={handleStartEditMessage}
        handleCancelEditMessage={handleCancelEditMessage}
        handleSaveEditedMessage={handleSaveEditedMessage}
        handleDeleteMessage={handleDeleteMessage}
      />
    </div>
  );
}

export default App;