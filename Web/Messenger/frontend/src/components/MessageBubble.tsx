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

  editingMessageId: number | null;
  editingMessageText: string;
  setEditingMessageText: (value: string) => void;

  isEditingMessage: boolean;
  isDeletingMessageId: number | null;

  handleStartEditMessage: (message: Message) => void;
  handleCancelEditMessage: () => void;
  handleSaveEditedMessage: (messageId: number) => Promise<void>;
  handleDeleteMessage: (messageId: number) => Promise<void>;
};

function MessageBubble({
  message,
  previousMessage,
  currentUser,
  editingMessageId,
  editingMessageText,
  setEditingMessageText,
  isEditingMessage,
  isDeletingMessageId,
  handleStartEditMessage,
  handleCancelEditMessage,
  handleSaveEditedMessage,
  handleDeleteMessage,
}: MessageBubbleProps) {
  const shouldShowDateSeparator =
    !previousMessage ||
    !isSameMessageDate(previousMessage.created_at, message.created_at);

  const isOwnMessage = message.sender.id === currentUser.id;
  const isEditing = editingMessageId === message.id;

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
            <p>{message.is_deleted ? "This message was deleted" : message.text}</p>

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
                <button
                  type="button"
                  onClick={() => handleStartEditMessage(message)}
                >
                  Edit
                </button>
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