import type { Message } from "../api/conversations";
import { formatMessageTime } from "../utils/chat";

type MessageFooterProps = {
  message: Message;
  isOwnMessage: boolean;
};

function getMessageStatus(message: Message) {
  if (message.is_read) {
    return "Read";
  }

  if (message.is_delivered) {
    return "Delivered";
  }

  return "Sent";
}

function MessageFooter({ message, isOwnMessage }: MessageFooterProps) {
  return (
    <div className="message-footer">
      {message.edited_at && !message.is_deleted && (
        <span className="message-meta">edited</span>
      )}

      <span className="message-time">
        {formatMessageTime(message.created_at)}
      </span>

      {isOwnMessage && !message.is_deleted && (
        <span className="message-read-status">{getMessageStatus(message)}</span>
      )}
    </div>
  );
}

export default MessageFooter;
