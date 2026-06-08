import type { Message } from "../api/conversations";

type MessageAttachmentProps = {
  message: Message;
};

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

function MessageAttachment({ message }: MessageAttachmentProps) {
  if (!message.attachment_url) {
    return null;
  }

  return (
    <div className="message-attachment">
      {message.attachment_is_image ? (
        <a href={message.attachment_url} target="_blank" rel="noreferrer">
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
  );
}

export default MessageAttachment;
