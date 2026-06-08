import {
  useEffect,
  useRef,
  useState,
  type ChangeEvent,
  type FormEvent,
  type KeyboardEvent as ReactKeyboardEvent,
  type MouseEvent as ReactMouseEvent,
} from "react";

import type { Conversation, Message } from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";

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

type MessageInputProps = {
  selectedConversation: Conversation | null;
  isChatBlocked: boolean;
  newMessage: string;
  selectedAttachment: File | null;
  replyToMessage: Message | null;
  isSending: boolean;
  handleSendMessage: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleAttachmentChange: (event: ChangeEvent<HTMLInputElement>) => void;
  handleRemoveAttachment: () => void;
  handleNewMessageChange: (value: string) => void;
  handleMessageKeyDown: (
    event: ReactKeyboardEvent<HTMLTextAreaElement>
  ) => void;
  handleCancelReplyMessage: () => void;
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

function MessageInput({
  selectedConversation,
  isChatBlocked,
  newMessage,
  selectedAttachment,
  replyToMessage,
  isSending,
  handleSendMessage,
  handleAttachmentChange,
  handleRemoveAttachment,
  handleNewMessageChange,
  handleMessageKeyDown,
  handleCancelReplyMessage,
}: MessageInputProps) {
  const messageInputRef = useRef<HTMLTextAreaElement | null>(null);
  const textContextMenuRef = useRef<HTMLDivElement | null>(null);
  const emojiPickerRef = useRef<HTMLDivElement | null>(null);

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
    <>
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
            placeholder={
              isChatBlocked ? "Messaging is blocked" : "Type a message..."
            }
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
    </>
  );
}

export default MessageInput;
