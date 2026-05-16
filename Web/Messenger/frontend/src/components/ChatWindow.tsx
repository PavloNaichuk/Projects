import type {
  ChangeEvent,
  FormEvent,
  KeyboardEvent,
  RefObject,
} from "react";
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
  selectedAttachment: File | null;
  replyToMessage: Message | null;
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
  handleAttachmentChange: (event: ChangeEvent<HTMLInputElement>) => void;
  handleRemoveAttachment: () => void;
  handleStartReplyMessage: (message: Message) => void;
  handleCancelReplyMessage: () => void;
  handleStartForwardMessage: (message: Message) => void;
  handleMessageKeyDown: (event: KeyboardEvent<HTMLTextAreaElement>) => void;
  handleSendMessage: (event: FormEvent<HTMLFormElement>) => Promise<void>;

  handleStartEditMessage: (message: Message) => void;
  handleCancelEditMessage: () => void;
  handleSaveEditedMessage: (messageId: number) => Promise<void>;
  handleDeleteMessage: (messageId: number) => Promise<void>;
  handleRemoveMessageAttachment: (messageId: number) => Promise<void>;
  handleToggleMessageReaction: (
    messageId: number,
    emoji: string
  ) => Promise<void>;
};

type UserAvatarProps = {
  user: User;
  isOnline?: boolean;
};

function UserAvatar({ user, isOnline = false }: UserAvatarProps) {
  const initial = user.username.trim().charAt(0).toUpperCase() || "?";

  return (
    <span className="user-avatar medium">
      {user.avatar_url ? (
        <img src={user.avatar_url} alt={user.username} />
      ) : (
        <span>{initial}</span>
      )}

      {isOnline && <span className="avatar-online-dot" />}
    </span>
  );
}

function getReplyPreviewText(message: Message) {
  if (message.is_deleted) {
    return "This message was deleted";
  }

  if (message.text.trim()) {
    return message.text;
  }

  if (message.attachment_name) {
    return message.attachment_is_image
      ? `Image: ${message.attachment_name}`
      : `File: ${message.attachment_name}`;
  }

  return "Message";
}

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
  selectedAttachment,
  replyToMessage,
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
  handleAttachmentChange,
  handleRemoveAttachment,
  handleStartReplyMessage,
  handleCancelReplyMessage,
  handleStartForwardMessage,
  handleMessageKeyDown,
  handleSendMessage,
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
  handleRemoveMessageAttachment,
  handleToggleMessageReaction,
}: ChatWindowProps) {
  return (
    <main className="chat">
      <header className="chat-header">
        <div className="chat-header-user">
          {selectedConversationUser && (
            <UserAvatar
              user={selectedConversationUser}
              isOnline={selectedConversationUserIsOnline}
            />
          )}

          <div>
            <h2>{selectedConversationName}</h2>

            {selectedConversationUser && (
              <>
                <p className="chat-user-email">
                  {selectedConversationUser.email}
                </p>
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
            handleStartReplyMessage={handleStartReplyMessage}
            handleStartForwardMessage={handleStartForwardMessage}
            handleStartEditMessage={handleStartEditMessage}
            handleCancelEditMessage={handleCancelEditMessage}
            handleSaveEditedMessage={handleSaveEditedMessage}
            handleDeleteMessage={handleDeleteMessage}
            handleRemoveMessageAttachment={handleRemoveMessageAttachment}
            handleToggleMessageReaction={handleToggleMessageReaction}
          />
        ))}

        <div ref={messagesEndRef} />
      </section>

      {typingUser && (
        <div className="typing-indicator">{typingUser.username} is typing...</div>
      )}

      {messageError && <div className="message-error">{messageError}</div>}

      <form className="message-form" onSubmit={handleSendMessage}>
        <label className="attachment-button">
          +
          <input
            type="file"
            onChange={handleAttachmentChange}
            disabled={!selectedConversation || isSending}
          />
        </label>

        <div className="message-input-area">
          {replyToMessage && (
            <div className="reply-preview-input">
              <div>
                <span className="reply-preview-title">
                  Replying to {replyToMessage.sender.username}
                </span>
                <span className="reply-preview-text">
                  {getReplyPreviewText(replyToMessage)}
                </span>
              </div>

              <button
                type="button"
                onClick={handleCancelReplyMessage}
                disabled={isSending}
              >
                ×
              </button>
            </div>
          )}

          {selectedAttachment && (
            <div className="selected-attachment">
              <span>{selectedAttachment.name}</span>
              <button
                type="button"
                onClick={handleRemoveAttachment}
                disabled={isSending}
              >
                Remove
              </button>
            </div>
          )}

          <textarea
            value={newMessage}
            onChange={(event) => handleNewMessageChange(event.target.value)}
            onKeyDown={handleMessageKeyDown}
            placeholder="Type a message..."
            disabled={!selectedConversation || isSending}
            rows={1}
          />
        </div>

        <button type="submit" disabled={!selectedConversation || isSending}>
          {isSending ? "Sending..." : "Send"}
        </button>
      </form>
    </main>
  );
}

export default ChatWindow;