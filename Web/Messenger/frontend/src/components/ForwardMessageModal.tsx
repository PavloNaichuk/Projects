import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";
import { getConversationName, getForwardPreviewText } from "../utils/chat";

type ForwardMessageModalProps = {
  forwardingMessage: Message;
  conversations: Conversation[];
  currentUser: User;
  isForwardingMessage: boolean;
  handleForwardMessageToConversation: (conversationId: number) => void;
  handleCancelForwardMessage: () => void;
};

function ForwardMessageModal({
  forwardingMessage,
  conversations,
  currentUser,
  isForwardingMessage,
  handleForwardMessageToConversation,
  handleCancelForwardMessage,
}: ForwardMessageModalProps) {
  return (
    <div className="modal-backdrop">
      <div className="forward-modal">
        <h3>Forward message</h3>

        <div className="forward-message-preview">
          {getForwardPreviewText(forwardingMessage)}
        </div>

        <div className="forward-conversation-list">
          {conversations.map((conversation) => {
            const lastMessage = conversation.last_message;
            const lastMessageText =
              lastMessage?.text.trim() ||
              lastMessage?.attachment_name ||
              "No messages yet";

            return (
              <button
                key={conversation.id}
                type="button"
                onClick={() =>
                  handleForwardMessageToConversation(conversation.id)
                }
                disabled={isForwardingMessage}
              >
                <span className="forward-conversation-name">
                  {getConversationName(conversation, currentUser)}
                </span>
                <span className="forward-conversation-preview">
                  {lastMessageText}
                </span>
              </button>
            );
          })}
        </div>

        <div className="forward-modal-actions">
          <button
            type="button"
            onClick={handleCancelForwardMessage}
            disabled={isForwardingMessage}
          >
            Cancel
          </button>
        </div>
      </div>
    </div>
  );
}

export default ForwardMessageModal;
