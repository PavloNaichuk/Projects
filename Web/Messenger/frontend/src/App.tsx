import { useState, type FormEvent } from "react";
import "./App.css";
import { getCurrentUser, login, type User } from "./api/auth";
import {
  createConversationMessage,
  getConversationMessages,
  getConversations,
  type Conversation,
  type Message,
} from "./api/conversations";

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

  const [username, setUsername] = useState("pavlo");
  const [password, setPassword] = useState("");

  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [isMessagesLoading, setIsMessagesLoading] = useState(false);
  const [newMessage, setNewMessage] = useState("");
  const [messageError, setMessageError] = useState("");
  const [isSending, setIsSending] = useState(false);

  async function loadMessages(token: string, conversationId: number) {
    setIsMessagesLoading(true);

    try {
      const messagesData = await getConversationMessages(token, conversationId);
      setMessages(messagesData);
    } catch {
      setMessages([]);
    } finally {
      setIsMessagesLoading(false);
    }
  }

  async function handleLogin(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    setError("");
    setIsLoading(true);

    try {
      const tokens = await login(username, password);

      setAccessToken(tokens.access);
      localStorage.setItem("accessToken", tokens.access);
      localStorage.setItem("refreshToken", tokens.refresh);

      const user = await getCurrentUser(tokens.access);
      setCurrentUser(user);

      const conversationsData = await getConversations(tokens.access);
      setConversations(conversationsData);

      const firstConversation = conversationsData[0] ?? null;
      setSelectedConversation(firstConversation);

      if (firstConversation) {
        await loadMessages(tokens.access, firstConversation.id);
      }
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
    await loadMessages(accessToken, conversation.id);
  }

  async function handleSendMessage(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    if (!accessToken || !selectedConversation) {
      return;
    }

    const text = newMessage.trim();

    if (!text) {
      setMessageError("Message text cannot be empty.");
      return;
    }

    setMessageError("");
    setIsSending(true);

    try {
      const createdMessage = await createConversationMessage(
        accessToken,
        selectedConversation.id,
        text
      );

      setMessages((previousMessages) => [...previousMessages, createdMessage]);
      setNewMessage("");

      const conversationsData = await getConversations(accessToken);
      setConversations(conversationsData);

      const updatedSelectedConversation = conversationsData.find(
        (conversation) => conversation.id === selectedConversation.id
      );

      if (updatedSelectedConversation) {
        setSelectedConversation(updatedSelectedConversation);
      }
    } catch {
      setMessageError("Failed to send message.");
    } finally {
      setIsSending(false);
    }
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
        </div>

        <div className="search-box">
          <input type="text" placeholder="Search users..." />
        </div>

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

          {messages.map((message) => (
            <div
              key={message.id}
              className={
                message.sender.id === currentUser.id
                  ? "message outgoing"
                  : "message incoming"
              }
            >
              <p>
                {message.is_deleted ? "This message was deleted" : message.text}
              </p>

              {message.edited_at && !message.is_deleted && (
                <span className="message-meta">edited</span>
              )}
            </div>
          ))}
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