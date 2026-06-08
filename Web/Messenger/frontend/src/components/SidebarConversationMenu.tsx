import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";

type SidebarConversationMenuProps = {
  conversation: Conversation;
  conversationUser: User;
  isMenuOpen: boolean;
  isDeleting: boolean;
  isBlockingUserId: number | null;
  setOpenedMenuConversationId: (conversationId: number | null) => void;
  toggleConversationMenu: (conversationId: number) => void;
  openDeleteConfirm: (
    conversation: Conversation,
    mode: DeleteConversationMode
  ) => void;
  openClearHistoryConfirm: (conversation: Conversation) => void;
  openBlockConfirm: (user: User) => void;
  openUnblockConfirm: (user: User) => void;
  handleMuteConversation: (conversation: Conversation) => Promise<void>;
  handlePinConversation: (conversation: Conversation) => Promise<void>;
  handleMarkConversationAsUnread: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
};

function SidebarConversationMenu({
  conversation,
  conversationUser,
  isMenuOpen,
  isDeleting,
  isBlockingUserId,
  setOpenedMenuConversationId,
  toggleConversationMenu,
  openDeleteConfirm,
  openClearHistoryConfirm,
  openBlockConfirm,
  openUnblockConfirm,
  handleMuteConversation,
  handlePinConversation,
  handleMarkConversationAsUnread,
  handleOpenContactNicknameModal,
}: SidebarConversationMenuProps) {
  return (
    <div
      className={
        isMenuOpen ? "conversation-menu-wrapper open" : "conversation-menu-wrapper"
      }
    >
      <button
        type="button"
        className="conversation-menu-button"
        onClick={() => toggleConversationMenu(conversation.id)}
        disabled={isDeleting}
        title="Conversation actions"
      >
        ⋯
      </button>

      {isMenuOpen && (
        <div className="conversation-menu">
          <button
            type="button"
            onClick={() => {
              setOpenedMenuConversationId(null);
              handlePinConversation(conversation);
            }}
            disabled={isDeleting}
          >
            {conversation.is_pinned ? "Unpin chat" : "Pin chat"}
          </button>

          <button
            type="button"
            onClick={() => {
              setOpenedMenuConversationId(null);
              handleMarkConversationAsUnread(conversation);
            }}
            disabled={isDeleting || conversation.is_marked_unread}
          >
            {conversation.is_marked_unread
              ? "Marked as unread"
              : "Mark as unread"}
          </button>

          <button
            type="button"
            onClick={() => {
              setOpenedMenuConversationId(null);
              handleMuteConversation(conversation);
            }}
            disabled={isDeleting}
          >
            {conversation.is_muted ? "Unmute sound" : "Mute sound"}
          </button>

          <button
            type="button"
            onClick={() => {
              setOpenedMenuConversationId(null);
              handleOpenContactNicknameModal(conversation);
            }}
            disabled={isDeleting}
          >
            Rename contact
          </button>

          <button
            type="button"
            className={conversationUser.is_blocked_by_me ? "" : "danger"}
            onClick={() => {
              if (conversationUser.is_blocked_by_me) {
                openUnblockConfirm(conversationUser);
              } else {
                openBlockConfirm(conversationUser);
              }
            }}
            disabled={isDeleting || isBlockingUserId === conversationUser.id}
          >
            {isBlockingUserId === conversationUser.id
              ? "Updating..."
              : conversationUser.is_blocked_by_me
                ? "Unblock user"
                : "Block user"}
          </button>

          <button
            type="button"
            onClick={() => openClearHistoryConfirm(conversation)}
            disabled={isDeleting}
          >
            Clear chat history
          </button>

          <button
            type="button"
            onClick={() => openDeleteConfirm(conversation, "for_me")}
            disabled={isDeleting}
          >
            {isDeleting ? "Deleting..." : "Delete for me"}
          </button>

          <button
            type="button"
            className="danger"
            onClick={() => openDeleteConfirm(conversation, "for_everyone")}
            disabled={isDeleting}
          >
            Delete for everyone
          </button>
        </div>
      )}
    </div>
  );
}

export default SidebarConversationMenu;
