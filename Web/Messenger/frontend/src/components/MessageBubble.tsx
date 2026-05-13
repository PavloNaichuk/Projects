import type { ReactNode } from "react";
import type { User } from "../api/auth";
import type { Message } from "../api/conversations";
import {
  formatMessageDate,
  formatMessageTime,
  isSameMessageDate,
} from "../utils/chat";

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

  handleStartEditMessage: (message: Message) => void;
  handleCancelEditMessage: () => void;
  handleSaveEditedMessage: (messageId: number) => Promise<void>;
  handleDeleteMessage: (messageId: number) => Promise<void>;
  handleRemoveMessageAttachment: (messageId: number) => Promise<void>;
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

function formatFileSize(size: number | null) {
  if (!size) {
    return "";
  }

  if (size < 1024) {
    return `${size} B`;
  }

  if (size < 1024 * 1024) {
    return `${(size / 1024).toFixed(1)} KB`;
  }

  return `${(size / (1024 * 1024)).toFixed(1)} MB`;
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
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
  handleRemoveMessageAttachment,
}: MessageBubbleProps) {
  const shouldShowDateSeparator =
    !previousMessage ||
    !isSameMessageDate(previousMessage.created_at, message.created_at);

  const isOwnMessage = message.sender.id === currentUser.id;
  const isEditing = editingMessageId === message.id;
  const hasText = Boolean(message.text.trim());
  const hasAttachment = Boolean(message.attachment_url);

  return (
    <div className="message-row">
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
                {hasAttachment && message.attachment_url && (
                  <div className="message-attachment">
                    {message.attachment_is_image ? (
                      <a
                        href={message.attachment_url}
                        target="_blank"
                        rel="noreferrer"
                      >
                        <img
                          src={message.attachment_url}
                          alt={message.attachment_name || "Attached image"}
                          className="message-image"
                        />
                      </a>
                    ) : (
                      <a
                        href={message.attachment_url}
                        target="_blank"
                        rel="noreferrer"
                        className="message-file"
                      >
                        <span className="message-file-icon">📎</span>
                        <span className="message-file-info">
                          <span className="message-file-name">
                            {message.attachment_name || "Attached file"}
                          </span>
                          <span className="message-file-size">
                            {formatFileSize(message.attachment_size)}
                          </span>
                        </span>
                      </a>
                    )}
                  </div>
                )}

                {hasText && (
                  <p>{renderHighlightedText(message.text, searchQuery)}</p>
                )}
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
                  {message.is_read ? "Read" : "Sent"}
                </span>
              )}
            </div>

            {isOwnMessage && !message.is_deleted && (
              <div className="message-actions">
                {hasText && (
                  <button
                    type="button"
                    onClick={() => handleStartEditMessage(message)}
                  >
                    Edit
                  </button>
                )}

                {hasAttachment && (
                  <button
                    type="button"
                    onClick={() => handleRemoveMessageAttachment(message.id)}
                    disabled={isDeletingMessageId === message.id}
                  >
                    {isDeletingMessageId === message.id
                      ? "Deleting..."
                      : "Delete file"}
                  </button>
                )}

                <button
                  type="button"
                  onClick={() => handleDeleteMessage(message.id)}
                  disabled={isDeletingMessageId === message.id}
                >
                  {isDeletingMessageId === message.id ? "Deleting..." : "Delete"}
                </button>
              </div>
            )}
          </>
        )}
      </div>
    </div>
  );
}

export default MessageBubble;