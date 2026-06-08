import { useEffect } from "react";

import type { Message } from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";

type MessageInfoModalProps = {
  message: Message;
  onClose: () => void;
};

function formatMessageInfoDate(dateString: string | null) {
  if (!dateString) {
    return "No";
  }

  const date = new Date(dateString);

  if (Number.isNaN(date.getTime())) {
    return "Unknown";
  }

  return date.toLocaleString([], {
    day: "2-digit",
    month: "long",
    year: "numeric",
    hour: "2-digit",
    minute: "2-digit",
  });
}

function MessageInfoModal({ message, onClose }: MessageInfoModalProps) {
  useEffect(() => {
    function handleEscapeKey(event: KeyboardEvent) {
      if (event.key === "Escape") {
        onClose();
      }
    }

    document.addEventListener("keydown", handleEscapeKey);

    return () => {
      document.removeEventListener("keydown", handleEscapeKey);
    };
  }, [onClose]);

  return (
    <div className="modal-backdrop">
      <div className="message-info-modal" role="dialog" aria-modal="true">
        <div className="message-info-header">
          <h3>Message info</h3>

          <button type="button" onClick={onClose}>
            ×
          </button>
        </div>

        <div className="message-info-list">
          <div className="message-info-row">
            <span>Sender</span>
            <strong>{getUserDisplayName(message.sender)}</strong>
          </div>

          <div className="message-info-row">
            <span>Username</span>
            <strong>@{message.sender.username}</strong>
          </div>

          <div className="message-info-row">
            <span>Sent at</span>
            <strong>{formatMessageInfoDate(message.created_at)}</strong>
          </div>

          <div className="message-info-row">
            <span>Delivered</span>
            <strong>
              {message.is_delivered
                ? formatMessageInfoDate(message.delivered_at)
                : "No"}
            </strong>
          </div>

          <div className="message-info-row">
            <span>Read</span>
            <strong>{message.is_read ? "Yes" : "No"}</strong>
          </div>

          <div className="message-info-row">
            <span>Edited</span>
            <strong>
              {message.edited_at
                ? formatMessageInfoDate(message.edited_at)
                : "No"}
            </strong>
          </div>

          <div className="message-info-row">
            <span>Attachment</span>
            <strong>{message.attachment_name || "No"}</strong>
          </div>
        </div>
      </div>
    </div>
  );
}

export default MessageInfoModal;
