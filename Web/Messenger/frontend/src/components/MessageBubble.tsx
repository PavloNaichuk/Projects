import { useEffect, useRef, useState, type ReactNode } from "react";
import type { User } from "../api/auth";
import type {
  DeleteMessageMode,
  Message,
} from "../api/conversations";
import {
  formatMessageDate,
  formatMessageTime,
  getUserDisplayName,
  isSameMessageDate,
} from "../utils/chat";
import MessageAttachment from "./MessageAttachment";
import MessageInfoModal from "./MessageInfoModal";
import MessageReactions from "./MessageReactions";
import MessageReplyPreview from "./MessageReplyPreview";

const REACTION_EMOJIS = ["👍", "❤️", "😂", "😮"];

type MessageBubbleProps = {
  message: Message;
  previousMessage?: Message;

  currentUser: User;

  searchQuery?: string;

  editingMessageId: number | null;
  editingMessageText: string;
  setEditingMessageText: (value: string) => void;

  isEditingMessage: boolean;
  isDeletingMessageId: number | null;

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

function renderHighlightedText(text: string, searchQuery: string) {
  const query = searchQuery.trim();

  if (!query) {
    return text;
  }

  const lowerText = text.toLowerCase();
  const lowerQuery = query.toLowerCase();

  const parts: ReactNode[] = [];
  let currentIndex = 0;

  while (currentIndex < text.length) {
    const matchIndex = lowerText.indexOf(lowerQuery, currentIndex);

    if (matchIndex === -1) {
      parts.push(text.slice(currentIndex));
      break;
    }

    if (matchIndex > currentIndex) {
      parts.push(text.slice(currentIndex, matchIndex));
    }

    const matchedText = text.slice(matchIndex, matchIndex + query.length);

    parts.push(<mark key={`${matchIndex}-${matchedText}`}>{matchedText}</mark>);

    currentIndex = matchIndex + query.length;
  }

  return parts;
}

function getMessageStatus(message: Message) {
  if (message.is_read) {
    return "Read";
  }

  if (message.is_delivered) {
    return "Delivered";
  }

  return "Sent";
}

async function copyTextToClipboard(text: string) {
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

async function downloadAttachment(url: string, filename: string) {
  try {
    const response = await fetch(url);

    if (!response.ok) {
      throw new Error("Failed to download attachment.");
    }

    const blob = await response.blob();
    const objectUrl = URL.createObjectURL(blob);

    const link = document.createElement("a");
    link.href = objectUrl;
    link.download = filename;
    document.body.appendChild(link);
    link.click();

    link.remove();
    URL.revokeObjectURL(objectUrl);
  } catch {
    const link = document.createElement("a");
    link.href = url;
    link.download = filename;
    link.target = "_blank";
    link.rel = "noreferrer";
    document.body.appendChild(link);
    link.click();
    link.remove();
  }
}

function MessageBubble({
  message,
  previousMessage,
  currentUser,
  searchQuery = "",
  editingMessageId,
  editingMessageText,
  setEditingMessageText,
  isEditingMessage,
  isDeletingMessageId,
  handleStartReplyMessage,
  handleStartForwardMessage,
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
  handleRemoveMessageAttachment,
  handleToggleMessageReaction,
}: MessageBubbleProps) {
  const [isMenuOpen, setIsMenuOpen] = useState(false);
  const [isCopied, setIsCopied] = useState(false);
  const [isInfoOpen, setIsInfoOpen] = useState(false);
  const menuRef = useRef<HTMLDivElement | null>(null);
  const messageRowRef = useRef<HTMLDivElement | null>(null);

  const shouldShowDateSeparator =
    !previousMessage ||
    !isSameMessageDate(previousMessage.created_at, message.created_at);

  const isOwnMessage = message.sender.id === currentUser.id;
  const isEditing = editingMessageId === message.id;
  const hasText = Boolean(message.text.trim());
  const hasAttachment = Boolean(message.attachment_url);
  const hasReactions = message.reactions.length > 0;
  const isDeleting = isDeletingMessageId === message.id;

  useEffect(() => {
    if (!isMenuOpen) {
      return;
    }

    window.requestAnimationFrame(() => {
      messageRowRef.current?.scrollIntoView({
        block: "center",
        behavior: "smooth",
      });
    });

    function handleDocumentClick(event: MouseEvent) {
      if (menuRef.current && !menuRef.current.contains(event.target as Node)) {
        setIsMenuOpen(false);
      }
    }

    function handleEscapeKey(event: KeyboardEvent) {
      if (event.key === "Escape") {
        setIsMenuOpen(false);
      }
    }

    document.addEventListener("mousedown", handleDocumentClick);
    document.addEventListener("keydown", handleEscapeKey);

    return () => {
      document.removeEventListener("mousedown", handleDocumentClick);
      document.removeEventListener("keydown", handleEscapeKey);
    };
  }, [isMenuOpen]);

  useEffect(() => {
    if (!isCopied) {
      return;
    }

    const timeoutId = window.setTimeout(() => {
      setIsCopied(false);
    }, 1200);

    return () => {
      window.clearTimeout(timeoutId);
    };
  }, [isCopied]);

  function closeMenu() {
    setIsMenuOpen(false);
  }

  async function handleCopyMessageText() {
    if (!hasText) {
      return;
    }

    await copyTextToClipboard(message.text);
    setIsCopied(true);
  }

  async function handleDownloadAttachment() {
    if (!message.attachment_url) {
      return;
    }

    await downloadAttachment(
      message.attachment_url,
      message.attachment_name || "attachment"
    );
  }

  return (
    <div className="message-row" ref={messageRowRef}>
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
              onChange={(event) => setEditingMessageText(event.target.value)}
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
            {message.is_deleted ? (
              <p>This message was deleted</p>
            ) : (
              <>
                {message.forwarded_from_message && (
                  <div className="forwarded-message-label">
                    Forwarded from{" "}
                    {getUserDisplayName(message.forwarded_from_message.sender)}
                  </div>
                )}

                {message.reply_to_message && (
                  <MessageReplyPreview reply={message.reply_to_message} />
                )}

                {hasAttachment && <MessageAttachment message={message} />}

                {hasText && (
                  <p>{renderHighlightedText(message.text, searchQuery)}</p>
                )}

                {hasReactions && (
                  <MessageReactions
                    message={message}
                    handleToggleMessageReaction={handleToggleMessageReaction}
                  />
                )}

                <div className="message-toolbar" ref={menuRef}>
                  <div className="message-toolbar-reactions">
                    {REACTION_EMOJIS.map((emoji) => (
                      <button
                        key={emoji}
                        type="button"
                        onClick={() =>
                          handleToggleMessageReaction(message.id, emoji)
                        }
                      >
                        {emoji}
                      </button>
                    ))}
                  </div>

                  <button
                    type="button"
                    className={
                      isMenuOpen
                        ? "message-menu-button active"
                        : "message-menu-button"
                    }
                    onClick={() => setIsMenuOpen((value) => !value)}
                    aria-label="Message actions"
                  >
                    ⋯
                  </button>

                  {isMenuOpen && (
                    <div className="message-context-menu">
                      <button
                        type="button"
                        onClick={() => {
                          closeMenu();
                          handleStartReplyMessage(message);
                        }}
                      >
                        Reply
                      </button>

                      <button
                        type="button"
                        onClick={() => {
                          closeMenu();
                          handleStartForwardMessage(message);
                        }}
                      >
                        Forward
                      </button>

                      <button
                        type="button"
                        onClick={() => {
                          closeMenu();
                          setIsInfoOpen(true);
                        }}
                      >
                        Message info
                      </button>

                      {hasAttachment && (
                        <button
                          type="button"
                          onClick={() => {
                            closeMenu();
                            handleDownloadAttachment();
                          }}
                        >
                          {message.attachment_is_image
                            ? "Download image"
                            : "Download file"}
                        </button>
                      )}

                      {hasText && (
                        <button
                          type="button"
                          onClick={() => {
                            closeMenu();
                            handleCopyMessageText();
                          }}
                        >
                          {isCopied ? "Copied!" : "Copy text"}
                        </button>
                      )}

                      {isOwnMessage && hasText && (
                        <button
                          type="button"
                          onClick={() => {
                            closeMenu();
                            handleStartEditMessage(message);
                          }}
                        >
                          Edit
                        </button>
                      )}

                      {isOwnMessage && hasAttachment && hasText && (
                        <button
                          type="button"
                          onClick={() => {
                            closeMenu();
                            handleRemoveMessageAttachment(message.id);
                          }}
                          disabled={isDeleting}
                        >
                          {isDeleting ? "Deleting..." : "Delete file"}
                        </button>
                      )}

                      <button
                        type="button"
                        onClick={() => {
                          closeMenu();
                          handleDeleteMessage(message.id, "for_me");
                        }}
                        disabled={isDeleting}
                      >
                        {isDeleting ? "Deleting..." : "Delete for me"}
                      </button>

                      {isOwnMessage && (
                        <button
                          type="button"
                          className="danger"
                          onClick={() => {
                            closeMenu();
                            handleDeleteMessage(message.id, "for_everyone");
                          }}
                          disabled={isDeleting}
                        >
                          {isDeleting ? "Deleting..." : "Delete for everyone"}
                        </button>
                      )}
                    </div>
                  )}
                </div>
              </>
            )}

            <div className="message-footer">
              {message.edited_at && !message.is_deleted && (
                <span className="message-meta">edited</span>
              )}

              <span className="message-time">
                {formatMessageTime(message.created_at)}
              </span>

              {isOwnMessage && !message.is_deleted && (
                <span className="message-read-status">
                  {getMessageStatus(message)}
                </span>
              )}
            </div>
          </>
        )}
      </div>

      {isInfoOpen && (
        <MessageInfoModal
          message={message}
          onClose={() => setIsInfoOpen(false)}
        />
      )}
    </div>
  );
}

export default MessageBubble;
