import type { ReactNode } from "react";
import type { User } from "../api/auth";
import type { Message, MessageReply } from "../api/conversations";
import {
  formatMessageDate,
  formatMessageTime,
  isSameMessageDate,
} from "../utils/chat";

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
  handleDeleteMessage: (messageId: number) => Promise<void>;
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

function getReplyPreviewText(reply: MessageReply) {
  if (reply.is_deleted) {
    return "This message was deleted";
  }

  if (reply.text.trim()) {
    return reply.text;
  }

  if (reply.attachment_name) {
    return reply.attachment_is_image
      ? `Image: ${reply.attachment_name}`
      : `File: ${reply.attachment_name}`;
  }

  return "Message";
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
  const shouldShowDateSeparator =
    !previousMessage ||
    !isSameMessageDate(previousMessage.created_at, message.created_at);

  const isOwnMessage = message.sender.id === currentUser.id;
  const isEditing = editingMessageId === message.id;
  const hasText = Boolean(message.text.trim());
  const hasAttachment = Boolean(message.attachment_url);
  const hasReactions = message.reactions.length > 0;

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
                {message.forwarded_from_message && (
                  <div className="forwarded-message-label">
                    Forwarded from{" "}
                    {message.forwarded_from_message.sender.username}
                  </div>
                )}

                {message.reply_to_message && (
                  <div className="reply-preview-message">
                    <span className="reply-preview-title">
                      Reply to {message.reply_to_message.sender.username}
                    </span>
                    <span className="reply-preview-text">
                      {getReplyPreviewText(message.reply_to_message)}
                    </span>
                  </div>
                )}

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

                {hasReactions && (
                  <div className="message-reactions">
                    {message.reactions.map((reaction) => (
                      <button
                        key={reaction.emoji}
                        type="button"
                        className={
                          reaction.reacted_by_me
                            ? "message-reaction active"
                            : "message-reaction"
                        }
                        onClick={() =>
                          handleToggleMessageReaction(message.id, reaction.emoji)
                        }
                      >
                        <span>{reaction.emoji}</span>
                        <span>{reaction.count}</span>
                      </button>
                    ))}
                  </div>
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
                  {getMessageStatus(message)}
                </span>
              )}
            </div>

            {!message.is_deleted && (
              <>
                <div className="message-reaction-picker">
                  {REACTION_EMOJIS.map((emoji) => (
                    <button
                      key={emoji}
                      type="button"
                      onClick={() => handleToggleMessageReaction(message.id, emoji)}
                    >
                      {emoji}
                    </button>
                  ))}
                </div>

                <div className="message-actions">
                  <button
                    type="button"
                    onClick={() => handleStartReplyMessage(message)}
                  >
                    Reply
                  </button>

                  <button
                    type="button"
                    onClick={() => handleStartForwardMessage(message)}
                  >
                    Forward
                  </button>

                  {isOwnMessage && hasText && (
                    <button
                      type="button"
                      onClick={() => handleStartEditMessage(message)}
                    >
                      Edit
                    </button>
                  )}

                  {isOwnMessage && hasAttachment && hasText && (
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

                  {isOwnMessage && (
                    <button
                      type="button"
                      onClick={() => handleDeleteMessage(message.id)}
                      disabled={isDeletingMessageId === message.id}
                    >
                      {isDeletingMessageId === message.id
                        ? "Deleting..."
                        : "Delete"}
                    </button>
                  )}
                </div>
              </>
            )}
          </>
        )}
      </div>
    </div>
  );
}

export default MessageBubble;