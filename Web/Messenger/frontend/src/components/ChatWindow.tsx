import type { FormEvent, KeyboardEvent, RefObject } from "react";
import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";
import MessageBubble from "./MessageBubble";

type ChatWindowProps = {
  currentUser: User;

  selectedConversation: Conversation | null;
  selectedConversationName: string;
  selectedConversationUser: User | null;
  selectedConversationUserIsOnline: boolean;

  messages: Message[];
  isMessagesLoading: boolean;
  isOlderMessagesLoading: boolean;
  hasMoreMessages: boolean;

  messageSearchQuery: string;
  setMessageSearchQuery: (value: string) => void;
  isMessageSearchActive: boolean;
  isSearchingMessages: boolean;

  typingUser: User | null;
  messageError: string;

  newMessage: string;
  isSending: boolean;

  editingMessageId: number | null;
  editingMessageText: string;
  setEditingMessageText: (value: string) => void;

  isEditingMessage: boolean;
  isDeletingMessageId: number | null;

  messagesContainerRef: RefObject<HTMLElement | null>;
  messagesEndRef: RefObject<HTMLDivElement | null>;

  handleMessagesScroll: () => void;
  handleSearchMessages: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleClearMessageSearch: () => Promise<void>;
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
  selectedConversationUserIsOnline,
  messages,
  isMessagesLoading,
  isOlderMessagesLoading,
  hasMoreMessages,
  messageSearchQuery,
  setMessageSearchQuery,
  isMessageSearchActive,
  isSearchingMessages,
  typingUser,
  messageError,
  newMessage,
  isSending,
  editingMessageId,
  editingMessageText,
  setEditingMessageText,
  isEditingMessage,
  isDeletingMessageId,
  messagesContainerRef,
  messagesEndRef,
  handleMessagesScroll,
  handleSearchMessages,
  handleClearMessageSearch,
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
            <>
              <p className="chat-user-email">{selectedConversationUser.email}</p>
              <p
                className={
                  selectedConversationUserIsOnline
                    ? "chat-user-status online"
                    : "chat-user-status offline"
                }
              >
                {selectedConversationUserIsOnline ? "Online" : "Offline"}
              </p>
            </>
          )}
        </div>

        <form className="message-search-form" onSubmit={handleSearchMessages}>
          <input
            type="text"
            value={messageSearchQuery}
            onChange={(event) => setMessageSearchQuery(event.target.value)}
            placeholder="Search messages..."
            disabled={!selectedConversation || isSearchingMessages}
          />

          {messageSearchQuery && (
            <button
              type="button"
              onClick={handleClearMessageSearch}
              disabled={isSearchingMessages}
            >
              Clear
            </button>
          )}

          <button
            type="submit"
            disabled={!selectedConversation || isSearchingMessages}
          >
            {isSearchingMessages ? "Searching..." : "Search"}
          </button>
        </form>
      </header>

      <section
        className="messages"
        ref={messagesContainerRef}
        onScroll={handleMessagesScroll}
      >
        {!selectedConversation && (
          <div className="empty-state">Select a conversation.</div>
        )}

        {selectedConversation && isMessagesLoading && (
          <div className="empty-state">Loading messages...</div>
        )}

        {selectedConversation && !isMessagesLoading && messages.length === 0 && (
          <div className="empty-state">
            {isMessageSearchActive ? "No messages found." : "No messages yet."}
          </div>
        )}

        {selectedConversation &&
          !isMessageSearchActive &&
          hasMoreMessages &&
          isOlderMessagesLoading && (
            <div className="older-messages-loader">
              Loading older messages...
            </div>
          )}

        {isMessageSearchActive && messages.length > 0 && (
          <div className="search-mode-label">Search results</div>
        )}

        {messages.map((message, index) => (
          <MessageBubble
            key={message.id}
            message={message}
            previousMessage={messages[index - 1]}
            currentUser={currentUser}
            searchQuery={isMessageSearchActive ? messageSearchQuery : ""}
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