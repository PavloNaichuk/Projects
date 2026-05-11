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

function getConversationName(conversation: Conversation, currentUser: User) {
  const otherParticipant = conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  );

  return otherParticipant?.user.username ?? "Unknown user";
}

function getOtherParticipant(conversation: Conversation, currentUser: User) {
  return conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  )?.user;
}

function formatMessageTime(dateString: string) {
  return new Date(dateString).toLocaleTimeString([], {
    hour: "2-digit",
    minute: "2-digit",
  });
}

function formatShortTime(dateString: string) {
  return new Date(dateString).toLocaleTimeString([], {
    hour: "2-digit",
    minute: "2-digit",
  });
}

function formatMessageDate(dateString: string) {
  return new Date(dateString).toLocaleDateString([], {
    day: "2-digit",
    month: "long",
    year: "numeric",
  });
}

function isSameMessageDate(firstDate: string, secondDate: string) {
  return (
    new Date(firstDate).toDateString() === new Date(secondDate).toDateString()
  );
}

function sortConversationsByUpdatedAt(conversations: Conversation[]) {
  return [...conversations].sort(
    (firstConversation, secondConversation) =>
      new Date(secondConversation.updated_at).getTime() -
      new Date(firstConversation.updated_at).getTime()
  );
}

function App() {
  const [currentUser, setCurrentUser] = useState<User | null>(null);
  const [accessToken, setAccessToken] = useState<string | null>(null);

  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] =
    useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);

  const socketRef = useRef<WebSocket | null>(null);
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

    setCurrentUser(null);
    setAccessToken(null);
    setConversations([]);
    setSelectedConversation(null);
    setMessages([]);
    setSearchResults([]);
    setUserSearchQuery("");
    setNewMessage("");
    setMessageError("");
  }

  async function handleLogout() {
    const savedRefreshToken = localStorage.getItem("refreshToken");

    if (socketRef.current) {
      socketRef.current.close();
      socketRef.current = null;
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
    if (!accessToken || !selectedConversation) {
      return;
    }

    const websocketUrl = `${WS_BASE_URL}/conversations/${selectedConversation.id}/?token=${accessToken}`;
    const socket = new WebSocket(websocketUrl);

    socketRef.current = socket;

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
          } catch {
            console.error("Failed to mark WebSocket message as read.");
          }
        }
      }

      if (data.type === "error") {
        setMessageError(data.detail);
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
    const isRegisterMode = authMode === "register";

    return (
      <div className="auth-page">
        <form
          className="auth-card"
          onSubmit={isRegisterMode ? handleRegister : handleLogin}
        >
          <h1>Messenger</h1>
          <p>{isRegisterMode ? "Create your account" : "Sign in to continue"}</p>

          <label>
            Username
            <input
              type="text"
              value={username}
              onChange={(event) => setUsername(event.target.value)}
              placeholder="Username"
            />
          </label>

          {isRegisterMode && (
            <label>
              Email
              <input
                type="email"
                value={email}
                onChange={(event) => setEmail(event.target.value)}
                placeholder="Email"
              />
            </label>
          )}

          <label>
            Password
            <input
              type="password"
              value={password}
              onChange={(event) => setPassword(event.target.value)}
              placeholder="Password"
            />
          </label>

          {isRegisterMode && (
            <label>
              Confirm password
              <input
                type="password"
                value={passwordConfirm}
                onChange={(event) => setPasswordConfirm(event.target.value)}
                placeholder="Confirm password"
              />
            </label>
          )}

          {error && (
            <div className="auth-error" role="alert">
              <strong>
                {authMode === "register"
                  ? "Please check the form:"
                  : "Sign in failed:"}
              </strong>

              <ul>
                {error.split("\n").map((message) => (
                  <li key={message}>{message}</li>
                ))}
              </ul>
            </div>
          )}

          <button type="submit" disabled={isLoading}>
            {isLoading
              ? isRegisterMode
                ? "Creating account..."
                : "Signing in..."
              : isRegisterMode
              ? "Create account"
              : "Sign in"}
          </button>

          <button
            type="button"
            className="auth-switch-button"
            onClick={() => {
              setError("");
              setAuthMode(isRegisterMode ? "login" : "register");
            }}
          >
            {isRegisterMode
              ? "Already have an account? Sign in"
              : "No account? Create one"}
          </button>
        </form>
      </div>
    );
  }

  const selectedConversationName = selectedConversation
    ? getConversationName(selectedConversation, currentUser)
    : "No conversation selected";

  const selectedConversationUser = selectedConversation
    ? getOtherParticipant(selectedConversation, currentUser)
    : null;

  return (
    <div className="app">
      <aside className="sidebar">
        <div className="sidebar-header">
          <h1>Messenger</h1>
          <p>Logged in as {currentUser.username}</p>
          <button className="logout-button" type="button" onClick={handleLogout}>
            Logout
          </button>
        </div>

        <form className="search-box" onSubmit={handleSearchUsers}>
          <input
            type="text"
            value={userSearchQuery}
            onChange={(event) => setUserSearchQuery(event.target.value)}
            placeholder="Search users..."
          />
          <button type="submit" disabled={isSearchingUsers}>
            {isSearchingUsers ? "Searching..." : "Search"}
          </button>
        </form>

        {userSearchError && (
          <div className="sidebar-error">{userSearchError}</div>
        )}

        {searchResults.length > 0 && (
          <div className="user-search-results">
            <div className="user-search-title">Users</div>

            {searchResults.map((user) => (
              <button
                key={user.id}
                type="button"
                className="user-search-item"
                onClick={() => handleStartConversation(user)}
              >
                <span className="user-search-name">{user.username}</span>
                <span className="user-search-email">{user.email}</span>
              </button>
            ))}
          </div>
        )}

        <div className="conversation-list">
          {conversations.length === 0 && (
            <div className="empty-state">No conversations yet.</div>
          )}

          {conversations.map((conversation) => (
            <button
              key={conversation.id}
              type="button"
              className={
                selectedConversation?.id === conversation.id
                  ? "conversation-item active"
                  : "conversation-item"
              }
              onClick={() => handleSelectConversation(conversation)}
            >
              <div className="conversation-name">
                {getConversationName(conversation, currentUser)}
              </div>

              <div className="conversation-preview">
                <span className="conversation-last-message">
                  {conversation.last_message?.is_deleted
                    ? "This message was deleted"
                    : conversation.last_message?.text || "No messages yet"}
                </span>

                {conversation.last_message && (
                  <span className="conversation-time">
                    {formatShortTime(conversation.last_message.created_at)}
                  </span>
                )}
              </div>

              {conversation.unread_count > 0 && (
                <span className="unread-badge">{conversation.unread_count}</span>
              )}
            </button>
          ))}
        </div>
      </aside>

      <main className="chat">
        <header className="chat-header">
          <div>
            <h2>{selectedConversationName}</h2>
            {selectedConversationUser && (
              <p className="chat-user-email">{selectedConversationUser.email}</p>
            )}
          </div>
        </header>

        <section className="messages">
          {!selectedConversation && (
            <div className="empty-state">Select a conversation.</div>
          )}

          {selectedConversation && isMessagesLoading && (
            <div className="empty-state">Loading messages...</div>
          )}

          {selectedConversation &&
            !isMessagesLoading &&
            messages.length === 0 && (
              <div className="empty-state">No messages yet.</div>
            )}

          {messages.map((message, index) => {
            const previousMessage = messages[index - 1];
            const shouldShowDateSeparator =
              !previousMessage ||
              !isSameMessageDate(
                previousMessage.created_at,
                message.created_at
              );

            const isOwnMessage = message.sender.id === currentUser.id;
            const isEditing = editingMessageId === message.id;

            return (
              <div key={message.id} className="message-row">
                {shouldShowDateSeparator && (
                  <div className="date-separator">
                    {formatMessageDate(message.created_at)}
                  </div>
                )}

                <div
                  className={
                    message.is_deleted
                      ? "message deleted"
                      : isOwnMessage
                      ? "message outgoing"
                      : "message incoming"
                  }
                >
                  {isEditing ? (
                    <div className="edit-message-form">
                      <input
                        type="text"
                        value={editingMessageText}
                        onChange={(event) =>
                          setEditingMessageText(event.target.value)
                        }
                        disabled={isEditingMessage}
                      />

                      <div className="message-actions">
                        <button
                          type="button"
                          onClick={() => handleSaveEditedMessage(message.id)}
                          disabled={isEditingMessage}
                        >
                          Save
                        </button>
                        <button
                          type="button"
                          onClick={handleCancelEditMessage}
                          disabled={isEditingMessage}
                        >
                          Cancel
                        </button>
                      </div>
                    </div>
                  ) : (
                    <>
                      <p>
                        {message.is_deleted
                          ? "This message was deleted"
                          : message.text}
                      </p>

                      <div className="message-footer">
                        {message.edited_at && !message.is_deleted && (
                          <span className="message-meta">edited</span>
                        )}

                        <span className="message-time">
                          {formatMessageTime(message.created_at)}
                        </span>
                      </div>

                      {isOwnMessage && !message.is_deleted && (
                        <div className="message-actions">
                          <button
                            type="button"
                            onClick={() => handleStartEditMessage(message)}
                          >
                            Edit
                          </button>
                          <button
                            type="button"
                            onClick={() => handleDeleteMessage(message.id)}
                            disabled={isDeletingMessageId === message.id}
                          >
                            {isDeletingMessageId === message.id
                              ? "Deleting..."
                              : "Delete"}
                          </button>
                        </div>
                      )}
                    </>
                  )}
                </div>
              </div>
            );
          })}

          <div ref={messagesEndRef} />
        </section>
        {typingUser && (
          <div className="typing-indicator">
            {typingUser.username} is typing...
          </div>
        )}
        {messageError && <div className="message-error">{messageError}</div>}

        <form className="message-form" onSubmit={handleSendMessage}>
          <textarea
            value={newMessage}
            onChange={(event) => handleNewMessageChange(event.target.value)}
            onKeyDown={handleMessageKeyDown}
            placeholder="Type a message..."
            disabled={!selectedConversation || isSending}
            rows={1}
          />
          <button type="submit" disabled={!selectedConversation || isSending}>
            {isSending ? "Sending..." : "Send"}
          </button>
        </form>
      </main>
    </div>
  );
}

export default App;