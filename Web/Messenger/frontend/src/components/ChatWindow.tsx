import {
  useEffect,
  useRef,
  useState,
  type ChangeEvent,
  type FormEvent,
  type KeyboardEvent as ReactKeyboardEvent,
  type MouseEvent as ReactMouseEvent,
  type RefObject,
} from "react";
import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
  DeleteMessageMode,
  Message,
} from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";
import MessageBubble from "./MessageBubble";

const MESSAGE_INPUT_EMOJIS = [
  "😀",
  "😂",
  "😍",
  "😎",
  "😢",
  "😡",
  "👍",
  "👎",
  "❤️",
  "🔥",
  "🎉",
  "🙏",
  "😮",
  "🤔",
  "🥳",
  "💪",
  "✅",
  "❌",
  "👏",
  "🤝",
];

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

type UserAvatarProps = {
  user: User;
  isOnline?: boolean;
};

type TextContextMenuState = {
  x: number;
  y: number;
  hasSelection: boolean;
  hasText: boolean;
};

type EmojiPickerState = {
  x: number;
  y: number;
};

function UserAvatar({ user, isOnline = false }: UserAvatarProps) {
  const displayName = getUserDisplayName(user);
  const initial = displayName.trim().charAt(0).toUpperCase() || "?";

  return (
    <span className="user-avatar medium">
      {user.avatar_url ? (
        <img src={user.avatar_url} alt={displayName} />
      ) : (
        <span>{initial}</span>
      )}

      {isOnline && <span className="avatar-online-dot" />}
    </span>
  );
}

function formatTime(date: Date) {
  const hours = String(date.getHours()).padStart(2, "0");
  const minutes = String(date.getMinutes()).padStart(2, "0");

  return `${hours}:${minutes}`;
}

function isSameDate(firstDate: Date, secondDate: Date) {
  return (
    firstDate.getFullYear() === secondDate.getFullYear() &&
    firstDate.getMonth() === secondDate.getMonth() &&
    firstDate.getDate() === secondDate.getDate()
  );
}

function formatLastSeen(lastSeenAt: string | null) {
  if (!lastSeenAt) {
    return "Last seen unknown";
  }

  const lastSeenDate = new Date(lastSeenAt);

  if (Number.isNaN(lastSeenDate.getTime())) {
    return "Last seen unknown";
  }

  const now = new Date();
  const diffInMs = now.getTime() - lastSeenDate.getTime();
  const diffInMinutes = Math.floor(diffInMs / 60000);
  const diffInHours = Math.floor(diffInMinutes / 60);

  if (diffInMinutes < 1) {
    return "Last seen just now";
  }

  if (diffInMinutes < 60) {
    return `Last seen ${diffInMinutes} ${
      diffInMinutes === 1 ? "minute" : "minutes"
    } ago`;
  }

  if (diffInHours < 24) {
    return `Last seen ${diffInHours} ${
      diffInHours === 1 ? "hour" : "hours"
    } ago`;
  }

  if (isSameDate(lastSeenDate, now)) {
    return `Last seen today at ${formatTime(lastSeenDate)}`;
  }

  const yesterday = new Date(now);
  yesterday.setDate(now.getDate() - 1);

  if (isSameDate(lastSeenDate, yesterday)) {
    return `Last seen yesterday at ${formatTime(lastSeenDate)}`;
  }

  return `Last seen ${lastSeenDate.toLocaleDateString()} at ${formatTime(
    lastSeenDate
  )}`;
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

async function writeTextToClipboard(text: string) {
  if (navigator.clipboard && window.isSecureContext) {
    await navigator.clipboard.writeText(text);
    return;
  }

  const textarea = document.createElement("textarea");
  textarea.value = text;
  textarea.style.position = "fixed";
  textarea.style.left = "-9999px";
  textarea.style.top = "-9999px";

  document.body.appendChild(textarea);
  textarea.focus();
  textarea.select();
  document.execCommand("copy");
  textarea.remove();
}

async function readTextFromClipboard() {
  if (!navigator.clipboard || !window.isSecureContext) {
    return "";
  }

  return navigator.clipboard.readText();
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
  isBlockingUserId,
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
  handleBlockUser,
  handleUnblockUser,
}: ChatWindowProps) {
  const messageInputRef = useRef<HTMLTextAreaElement | null>(null);
  const textContextMenuRef = useRef<HTMLDivElement | null>(null);
  const emojiPickerRef = useRef<HTMLDivElement | null>(null);

  const isSelectedUserBlockedByMe = Boolean(
    selectedConversationUser?.is_blocked_by_me
  );
  const hasSelectedUserBlockedMe = Boolean(
    selectedConversationUser?.has_blocked_me
  );
  const isChatBlocked = isSelectedUserBlockedByMe || hasSelectedUserBlockedMe;
  const isSelectedUserBlockUpdating =
    selectedConversationUser !== null &&
    isBlockingUserId === selectedConversationUser.id;

  const [textContextMenu, setTextContextMenu] =
    useState<TextContextMenuState | null>(null);
  const [emojiPicker, setEmojiPicker] = useState<EmojiPickerState | null>(null);

  useEffect(() => {
    if (!textContextMenu && !emojiPicker) {
      return;
    }

    function handleDocumentMouseDown(event: MouseEvent) {
      const target = event.target as Node;

      if (
        textContextMenuRef.current &&
        textContextMenuRef.current.contains(target)
      ) {
        return;
      }

      if (emojiPickerRef.current && emojiPickerRef.current.contains(target)) {
        return;
      }

      setTextContextMenu(null);
      setEmojiPicker(null);
    }

    function handleEscapeKey(event: KeyboardEvent) {
      if (event.key === "Escape") {
        setTextContextMenu(null);
        setEmojiPicker(null);
      }
    }

    document.addEventListener("mousedown", handleDocumentMouseDown);
    document.addEventListener("keydown", handleEscapeKey);

    return () => {
      document.removeEventListener("mousedown", handleDocumentMouseDown);
      document.removeEventListener("keydown", handleEscapeKey);
    };
  }, [textContextMenu, emojiPicker]);

  function closeTextContextMenu() {
    setTextContextMenu(null);
  }

  function closeEmojiPicker() {
    setEmojiPicker(null);
  }

  function handleMessageInputContextMenu(
    event: ReactMouseEvent<HTMLTextAreaElement>
  ) {
    event.preventDefault();

    const textarea = event.currentTarget;
    const hasSelection = textarea.selectionStart !== textarea.selectionEnd;
    const hasText = textarea.value.length > 0;

    const menuWidth = 260;
    const menuHeight = 350;

    const x = Math.min(
      Math.max(8, event.clientX),
      window.innerWidth - menuWidth - 8
    );
    const y = Math.min(
      Math.max(8, event.clientY),
      window.innerHeight - menuHeight - 8
    );

    setEmojiPicker(null);
    setTextContextMenu({
      x,
      y,
      hasSelection,
      hasText,
    });
  }

  function focusMessageInput() {
    messageInputRef.current?.focus();
  }

  function syncMessageInputValue() {
    const textarea = messageInputRef.current;

    if (!textarea) {
      return;
    }

    handleNewMessageChange(textarea.value);
  }

  function replaceMessageInputSelection(text: string) {
    const textarea = messageInputRef.current;

    if (!textarea) {
      return;
    }

    const start = textarea.selectionStart;
    const end = textarea.selectionEnd;
    const updatedValue =
      textarea.value.slice(0, start) + text + textarea.value.slice(end);
    const cursorPosition = start + text.length;

    handleNewMessageChange(updatedValue);

    window.requestAnimationFrame(() => {
      textarea.focus();
      textarea.setSelectionRange(cursorPosition, cursorPosition);
    });
  }

  function handleEmojiMenuClick() {
    if (!textContextMenu) {
      return;
    }

    const pickerWidth = 240;
    const pickerHeight = 170;

    const x = Math.min(textContextMenu.x, window.innerWidth - pickerWidth - 8);
    const y = Math.min(textContextMenu.y, window.innerHeight - pickerHeight - 8);

    closeTextContextMenu();
    setEmojiPicker({ x, y });
    focusMessageInput();
  }

  function handleEmojiClick(emoji: string) {
    replaceMessageInputSelection(emoji);
    closeEmojiPicker();
  }

  function handleUndoMenuClick() {
    closeTextContextMenu();
    focusMessageInput();
    document.execCommand("undo");
    window.requestAnimationFrame(syncMessageInputValue);
  }

  function handleRedoMenuClick() {
    closeTextContextMenu();
    focusMessageInput();
    document.execCommand("redo");
    window.requestAnimationFrame(syncMessageInputValue);
  }

  async function handleCutMenuClick() {
    const textarea = messageInputRef.current;

    if (!textarea || textarea.selectionStart === textarea.selectionEnd) {
      return;
    }

    const selectedText = textarea.value.slice(
      textarea.selectionStart,
      textarea.selectionEnd
    );

    await writeTextToClipboard(selectedText);
    replaceMessageInputSelection("");
    closeTextContextMenu();
  }

  async function handleCopyMenuClick() {
    const textarea = messageInputRef.current;

    if (!textarea || textarea.selectionStart === textarea.selectionEnd) {
      return;
    }

    const selectedText = textarea.value.slice(
      textarea.selectionStart,
      textarea.selectionEnd
    );

    await writeTextToClipboard(selectedText);
    closeTextContextMenu();
  }

  async function handlePasteMenuClick() {
    const pastedText = await readTextFromClipboard();

    if (pastedText) {
      replaceMessageInputSelection(pastedText);
    }

    closeTextContextMenu();
  }

  function handleSelectAllMenuClick() {
    const textarea = messageInputRef.current;

    if (!textarea) {
      return;
    }

    closeTextContextMenu();
    textarea.focus();
    textarea.select();
  }

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
              <p
                className={
                  isChatBlocked
                    ? "chat-user-status blocked"
                    : selectedConversationUserIsOnline
                      ? "chat-user-status online"
                      : "chat-user-status offline"
                }
              >
                {isSelectedUserBlockedByMe
                  ? "You blocked this user"
                  : hasSelectedUserBlockedMe
                    ? "This user blocked you"
                    : selectedConversationUserIsOnline
                      ? "Online"
                      : formatLastSeen(selectedConversationUser.last_seen_at)}
              </p>
            )}
          </div>
        </div>

        {selectedConversationUser && (
          <div className="chat-header-actions">
            <button
              type="button"
              className={isSelectedUserBlockedByMe ? "" : "danger"}
              onClick={() => {
                if (isSelectedUserBlockedByMe) {
                  handleUnblockUser(selectedConversationUser);
                } else {
                  handleBlockUser(selectedConversationUser);
                }
              }}
              disabled={isSelectedUserBlockUpdating}
            >
              {isSelectedUserBlockUpdating
                ? "Updating..."
                : isSelectedUserBlockedByMe
                  ? "Unblock"
                  : "Block"}
            </button>
          </div>
        )}

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

        <div className="messages-end-spacer" ref={messagesEndRef} />
      </section>

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

      <form className="message-form" onSubmit={handleSendMessage}>
        <label className="attachment-button">
          +
          <input
            type="file"
            onChange={handleAttachmentChange}
            disabled={!selectedConversation || isSending || isChatBlocked}
          />
        </label>

        <div className="message-input-area">
          {replyToMessage && (
            <div className="reply-preview-input">
              <div>
                <span className="reply-preview-title">
                  Replying to {getUserDisplayName(replyToMessage.sender)}
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
            ref={messageInputRef}
            value={newMessage}
            onChange={(event) => handleNewMessageChange(event.target.value)}
            onKeyDown={handleMessageKeyDown}
            onContextMenu={handleMessageInputContextMenu}
            placeholder={isChatBlocked ? "Messaging is blocked" : "Type a message..."}
            disabled={!selectedConversation || isSending || isChatBlocked}
            rows={1}
          />
        </div>

        <button
          type="submit"
          disabled={!selectedConversation || isSending || isChatBlocked}
        >
          {isSending ? "Sending..." : "Send"}
        </button>
      </form>

      {textContextMenu && (
        <div
          ref={textContextMenuRef}
          className="text-context-menu"
          style={{
            left: textContextMenu.x,
            top: textContextMenu.y,
          }}
        >
          <button type="button" onClick={handleEmojiMenuClick}>
            <span>Emoji</span>
            <span>Open</span>
          </button>

          <div className="text-context-menu-separator" />

          <button type="button" onClick={handleUndoMenuClick}>
            <span>Undo</span>
            <span>Ctrl+Z</span>
          </button>

          <button type="button" onClick={handleRedoMenuClick}>
            <span>Redo</span>
            <span>Ctrl+Shift+Z</span>
          </button>

          <div className="text-context-menu-separator" />

          <button
            type="button"
            onClick={handleCutMenuClick}
            disabled={!textContextMenu.hasSelection}
          >
            <span>Cut</span>
            <span>Ctrl+X</span>
          </button>

          <button
            type="button"
            onClick={handleCopyMenuClick}
            disabled={!textContextMenu.hasSelection}
          >
            <span>Copy</span>
            <span>Ctrl+C</span>
          </button>

          <button type="button" onClick={handlePasteMenuClick}>
            <span>Paste</span>
            <span>Ctrl+V</span>
          </button>

          <button
            type="button"
            onClick={handleSelectAllMenuClick}
            disabled={!textContextMenu.hasText}
          >
            <span>Select all</span>
            <span>Ctrl+A</span>
          </button>
        </div>
      )}

      {emojiPicker && (
        <div
          ref={emojiPickerRef}
          className="text-emoji-picker"
          style={{
            left: emojiPicker.x,
            top: emojiPicker.y,
          }}
        >
          {MESSAGE_INPUT_EMOJIS.map((emoji) => (
            <button
              key={emoji}
              type="button"
              onClick={() => handleEmojiClick(emoji)}
            >
              {emoji}
            </button>
          ))}
        </div>
      )}
    </main>
  );
}

export default ChatWindow;
