import type { RefObject } from "react";

import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteMessageMode,
  Message,
} from "../api/conversations";
import MessageBubble from "./MessageBubble";

type MessageListProps = {
  currentUser: User;
  selectedConversation: Conversation | null;
  messages: Message[];
  isMessagesLoading: boolean;
  isOlderMessagesLoading: boolean;
  hasMoreMessages: boolean;
  isMessageSearchActive: boolean;
  messageSearchQuery: string;
  editingMessageId: number | null;
  editingMessageText: string;
  setEditingMessageText: (value: string) => void;
  isEditingMessage: boolean;
  isDeletingMessageId: number | null;
  messagesContainerRef: RefObject<HTMLElement | null>;
  messagesEndRef: RefObject<HTMLDivElement | null>;
  handleMessagesScroll: () => void;
  handleStartReplyMessage: (message: Message) => void;
  handleStartForwardMessage: (message: Message) => void;
  handleStartEditMessage: (message: Message) => void;
  handleCancelEditMessage: () => void;
  handleSaveEditedMessage: (messageId: number) => Promise<void>;
  handleDeleteMessage: (
    messageId: number,
    mode: DeleteMessageMode
  ) => Promise<void>;
  handleRemoveMessageAttachment: (messageId: number) => Promise<void>;
  handleToggleMessageReaction: (
    messageId: number,
    emoji: string
  ) => Promise<void>;
};

function MessageList({
  currentUser,
  selectedConversation,
  messages,
  isMessagesLoading,
  isOlderMessagesLoading,
  hasMoreMessages,
  isMessageSearchActive,
  messageSearchQuery,
  editingMessageId,
  editingMessageText,
  setEditingMessageText,
  isEditingMessage,
  isDeletingMessageId,
  messagesContainerRef,
  messagesEndRef,
  handleMessagesScroll,
  handleStartReplyMessage,
  handleStartForwardMessage,
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
  handleRemoveMessageAttachment,
  handleToggleMessageReaction,
}: MessageListProps) {
  return (
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
          <div className="older-messages-loader">Loading older messages...</div>
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

      <div className="messages-end-spacer" ref={messagesEndRef} />
    </section>
  );
}

export default MessageList;
