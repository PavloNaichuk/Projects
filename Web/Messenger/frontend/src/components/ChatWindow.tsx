import type {
  ChangeEvent,
  FormEvent,
  KeyboardEvent as ReactKeyboardEvent,
  RefObject,
} from "react";
import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
  DeleteMessageMode,
  Message,
} from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";
import ChatHeader from "./ChatHeader";
import MessageInput from "./MessageInput";
import MessageList from "./MessageList";

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
  isDeletingConversationId: number | null;
  isBlockingUserId: number | null;

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
  handleMessageKeyDown: (
    event: ReactKeyboardEvent<HTMLTextAreaElement>
  ) => void;
  handleSendMessage: (event: FormEvent<HTMLFormElement>) => Promise<void>;

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

  handleMuteConversation: (conversation: Conversation) => Promise<void>;
  handlePinConversation: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
  handleDeleteConversation: (
    conversation: Conversation,
    mode: DeleteConversationMode
  ) => Promise<void>;
  handleBlockUser: (user: User) => Promise<void>;
  handleUnblockUser: (user: User) => Promise<void>;
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
  const isSelectedUserBlockedByMe = Boolean(
    selectedConversationUser?.is_blocked_by_me
  );
  const hasSelectedUserBlockedMe = Boolean(
    selectedConversationUser?.has_blocked_me
  );
  const isChatBlocked = isSelectedUserBlockedByMe || hasSelectedUserBlockedMe;

  return (
    <main className="chat">
      <ChatHeader
        selectedConversation={selectedConversation}
        selectedConversationName={selectedConversationName}
        selectedConversationUser={selectedConversationUser}
        selectedConversationUserIsOnline={selectedConversationUserIsOnline}
        messageSearchQuery={messageSearchQuery}
        setMessageSearchQuery={setMessageSearchQuery}
        isSearchingMessages={isSearchingMessages}
        handleSearchMessages={handleSearchMessages}
        handleClearMessageSearch={handleClearMessageSearch}
      />

      <MessageList
        currentUser={currentUser}
        selectedConversation={selectedConversation}
        messages={messages}
        isMessagesLoading={isMessagesLoading}
        isOlderMessagesLoading={isOlderMessagesLoading}
        hasMoreMessages={hasMoreMessages}
        isMessageSearchActive={isMessageSearchActive}
        messageSearchQuery={messageSearchQuery}
        editingMessageId={editingMessageId}
        editingMessageText={editingMessageText}
        setEditingMessageText={setEditingMessageText}
        isEditingMessage={isEditingMessage}
        isDeletingMessageId={isDeletingMessageId}
        messagesContainerRef={messagesContainerRef}
        messagesEndRef={messagesEndRef}
        handleMessagesScroll={handleMessagesScroll}
        handleStartReplyMessage={handleStartReplyMessage}
        handleStartForwardMessage={handleStartForwardMessage}
        handleStartEditMessage={handleStartEditMessage}
        handleCancelEditMessage={handleCancelEditMessage}
        handleSaveEditedMessage={handleSaveEditedMessage}
        handleDeleteMessage={handleDeleteMessage}
        handleRemoveMessageAttachment={handleRemoveMessageAttachment}
        handleToggleMessageReaction={handleToggleMessageReaction}
      />

      {typingUser && (
        <div className="typing-indicator">
          {getUserDisplayName(typingUser)} is typing...
        </div>
      )}

      {isChatBlocked && (
        <div className="blocked-chat-notice">
          {isSelectedUserBlockedByMe
            ? "You blocked this user. Unblock them to send messages."
            : "You cannot send messages to this user."}
        </div>
      )}

      {messageError && <div className="message-error">{messageError}</div>}

      <MessageInput
        selectedConversation={selectedConversation}
        isChatBlocked={isChatBlocked}
        newMessage={newMessage}
        selectedAttachment={selectedAttachment}
        replyToMessage={replyToMessage}
        isSending={isSending}
        handleSendMessage={handleSendMessage}
        handleAttachmentChange={handleAttachmentChange}
        handleRemoveAttachment={handleRemoveAttachment}
        handleNewMessageChange={handleNewMessageChange}
        handleMessageKeyDown={handleMessageKeyDown}
        handleCancelReplyMessage={handleCancelReplyMessage}
      />
    </main>
  );
}

export default ChatWindow;
