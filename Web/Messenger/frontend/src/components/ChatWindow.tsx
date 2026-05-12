import type { FormEvent, KeyboardEvent, RefObject } from "react";
import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";
import MessageBubble from "./MessageBubble";

type ChatWindowProps = {
  currentUser: User;

  selectedConversation: Conversation | null;
  selectedConversationName: string;
  selectedConversationUser: User | null;

  messages: Message[];
  isMessagesLoading: boolean;

  typingUser: User | null;
  messageError: string;

  newMessage: string;
  isSending: boolean;

  editingMessageId: number | null;
  editingMessageText: string;
  setEditingMessageText: (value: string) => void;

  isEditingMessage: boolean;
  isDeletingMessageId: number | null;

  messagesEndRef: RefObject<HTMLDivElement | null>;

  handleNewMessageChange: (value: string) => void;
  handleMessageKeyDown: (event: KeyboardEvent<HTMLTextAreaElement>) => void;
  handleSendMessage: (event: FormEvent<HTMLFormElement>) => Promise<void>;

  handleStartEditMessage: (message: Message) => void;
  handleCancelEditMessage: () => void;
  handleSaveEditedMessage: (messageId: number) => Promise<void>;
  handleDeleteMessage: (messageId: number) => Promise<void>;
};

function ChatWindow({
  currentUser,
  selectedConversation,
  selectedConversationName,
  selectedConversationUser,
  messages,
  isMessagesLoading,
  typingUser,
  messageError,
  newMessage,
  isSending,
  editingMessageId,
  editingMessageText,
  setEditingMessageText,
  isEditingMessage,
  isDeletingMessageId,
  messagesEndRef,
  handleNewMessageChange,
  handleMessageKeyDown,
  handleSendMessage,
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
}: ChatWindowProps) {
  return (
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

        {selectedConversation && !isMessagesLoading && messages.length === 0 && (
          <div className="empty-state">No messages yet.</div>
        )}

        {messages.map((message, index) => (
          <MessageBubble
            key={message.id}
            message={message}
            previousMessage={messages[index - 1]}
            currentUser={currentUser}
            editingMessageId={editingMessageId}
            editingMessageText={editingMessageText}
            setEditingMessageText={setEditingMessageText}
            isEditingMessage={isEditingMessage}
            isDeletingMessageId={isDeletingMessageId}
            handleStartEditMessage={handleStartEditMessage}
            handleCancelEditMessage={handleCancelEditMessage}
            handleSaveEditedMessage={handleSaveEditedMessage}
            handleDeleteMessage={handleDeleteMessage}
          />
        ))}

        <div ref={messagesEndRef} />
      </section>

      {typingUser && (
        <div className="typing-indicator">{typingUser.username} is typing...</div>
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
  );
}

export default ChatWindow;