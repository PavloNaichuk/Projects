import { useEffect, useRef, useState, type FormEvent } from "react";
import { WS_BASE_URL } from "./api/config";
import "./App.css";
import {
  getCurrentUser,
  login,
  logout,
  refreshAccessToken,
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

function App() {
  const [currentUser, setCurrentUser] = useState<User | null>(null);
  const [accessToken, setAccessToken] = useState<string | null>(null);

  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] =
    useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);

  const socketRef = useRef<WebSocket | null>(null);

  const [username, setUsername] = useState("pavlo");
  const [password, setPassword] = useState("");

  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [isMessagesLoading, setIsMessagesLoading] = useState(false);
  const [isAuthChecking, setIsAuthChecking] = useState(true);

  const [newMessage, setNewMessage] = useState("");
  const [messageError, setMessageError] = useState("");
  const [isSending, setIsSending] = useState(false);

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
    setConversations(conversationsData);

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
      previousConversations.map((conversation) =>
        conversation.last_message?.id === updatedMessage.id
          ? {
              ...conversation,
              last_message: updatedMessage,
              updated_at: updatedMessage.updated_at,
            }
          : conversation
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

    socket.onopen = () => {
      console.log("WebSocket connected");
    };

    socket.onmessage = (event) => {
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
          previousConversations.map((conversation) =>
            conversation.id === receivedMessage.conversation
              ? {
                  ...conversation,
                  last_message: receivedMessage,
                  updated_at: receivedMessage.created_at,
                }
              : conversation
          )
        );

        setSelectedConversation((previousConversation) =>
          previousConversation?.id === receivedMessage.conversation
            ? {
                ...previousConversation,
                last_message: receivedMessage,
                updated_at: receivedMessage.created_at,
              }
            : previousConversation
        );
      }

      if (data.type === "error") {
        setMessageError(data.detail);
      }
    };

    socket.onerror = (error) => {
      console.error("WebSocket error:", error);
      setMessageError("WebSocket connection error.");
    };

    socket.onclose = () => {
      console.log("WebSocket disconnected");
    };

    return () => {
      socket.close();

      if (socketRef.current === socket) {
        socketRef.current = null;
      }
    };
  }, [accessToken, selectedConversation?.id]);

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

  async function handleSelectConversation(conversation: Conversation) {
    if (!accessToken) {
      return;
    }

    setSelectedConversation(conversation);
    setEditingMessageId(null);
    setEditingMessageText("");
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
          return previousConversations.map((item) =>
            item.id === conversation.id ? conversation : item
          );
        }

        return [conversation, ...previousConversations];
      });

      setSelectedConversation(conversation);
      await loadMessages(accessToken, conversation.id);

      setUserSearchQuery("");
      setSearchResults([]);
    } catch {
      setUserSearchError("Failed to create conversation.");
    }
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
      setNewMessage("");
    } catch {
      setMessageError("Failed to send message.");
    } finally {
      setIsSending(false);
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
      <div className="auth-page">
        <form className="auth-card" onSubmit={handleLogin}>
          <h1>Messenger</h1>
          <p>Sign in to continue</p>

          <label>
            Username
            <input
              type="text"
              value={username}
              onChange={(event) => setUsername(event.target.value)}
              placeholder="Username"
            />
          </label>

          <label>
            Password
            <input
              type="password"
              value={password}
              onChange={(event) => setPassword(event.target.value)}
              placeholder="Password"
            />
          </label>

          {error && <div className="auth-error">{error}</div>}

          <button type="submit" disabled={isLoading}>
            {isLoading ? "Signing in..." : "Sign in"}
          </button>
        </form>
      </div>
    );
  }

  const selectedConversationName = selectedConversation
    ? getConversationName(selectedConversation, currentUser)
    : "No conversation selected";

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

              <div className="conversation-last-message">
                {conversation.last_message?.is_deleted
                  ? "This message was deleted"
                  : conversation.last_message?.text || "No messages yet"}
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
          <h2>{selectedConversationName}</h2>
          <span>Online</span>
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

          {messages.map((message) => {
            const isOwnMessage = message.sender.id === currentUser.id;
            const isEditing = editingMessageId === message.id;

            return (
              <div
                key={message.id}
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

                    {message.edited_at && !message.is_deleted && (
                      <span className="message-meta">edited</span>
                    )}

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
            );
          })}
        </section>

        {messageError && <div className="message-error">{messageError}</div>}

        <form className="message-form" onSubmit={handleSendMessage}>
          <input
            type="text"
            value={newMessage}
            onChange={(event) => setNewMessage(event.target.value)}
            placeholder="Type a message..."
            disabled={!selectedConversation || isSending}
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