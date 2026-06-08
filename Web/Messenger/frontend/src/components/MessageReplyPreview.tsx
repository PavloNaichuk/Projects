import type { MessageReply } from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";

type MessageReplyPreviewProps = {
  reply: MessageReply;
};

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

function MessageReplyPreview({ reply }: MessageReplyPreviewProps) {
  return (
    <div className="reply-preview-message">
      <span className="reply-preview-title">
        Reply to {getUserDisplayName(reply.sender)}
      </span>
      <span className="reply-preview-text">{getReplyPreviewText(reply)}</span>
    </div>
  );
}

export default MessageReplyPreview;
