import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import { formatShortTime, getConversationName } from "../utils/chat";
import SidebarConversationMenu from "./SidebarConversationMenu";
import UserAvatar from "./UserAvatar";

type SidebarConversationItemProps = {
  currentUser: User;
  onlineUserIds: number[];
  conversation: Conversation;
  selectedConversationId: number | null;
  openedMenuConversationId: number | null;
  isDeletingConversationId: number | null;
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
  handleSelectConversation: (conversation: Conversation) => Promise<void>;
  handleMuteConversation: (conversation: Conversation) => Promise<void>;
  handlePinConversation: (conversation: Conversation) => Promise<void>;
  handleMarkConversationAsUnread: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
};

function SidebarConversationItem({
  currentUser,
  onlineUserIds,
  conversation,
  selectedConversationId,
  openedMenuConversationId,
  isDeletingConversationId,
  isBlockingUserId,
  setOpenedMenuConversationId,
  toggleConversationMenu,
  openDeleteConfirm,
  openClearHistoryConfirm,
  openBlockConfirm,
  openUnblockConfirm,
  handleSelectConversation,
  handleMuteConversation,
  handlePinConversation,
  handleMarkConversationAsUnread,
  handleOpenContactNicknameModal,
}: SidebarConversationItemProps) {
  const otherParticipant = conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  );

  const conversationUser = otherParticipant?.user ?? currentUser;

  const isOnline = otherParticipant
    ? onlineUserIds.includes(otherParticipant.user.id)
    : false;

  const isDeleting = isDeletingConversationId === conversation.id;
  const isMenuOpen = openedMenuConversationId === conversation.id;

  return (
    <div
      className={[
        "conversation-item",
        selectedConversationId === conversation.id ? "active" : "",
        conversation.is_marked_unread && conversation.unread_count === 0
          ? "marked-unread"
          : "",
      ]
        .filter(Boolean)
        .join(" ")}
    >
      <button
        type="button"
        className="conversation-main"
        onClick={() => {
          setOpenedMenuConversationId(null);
          handleSelectConversation(conversation);
        }}
      >
        <UserAvatar user={conversationUser} size="medium" isOnline={isOnline} />

        <div className="conversation-content">
          <div className="conversation-name">
            <div className="conversation-name-block">
              <div className="conversation-display-name-row">
                <span className="conversation-display-name">
                  {getConversationName(conversation, currentUser)}
                </span>

                {conversation.is_pinned && (
                  <span className="pinned-conversation-icon">📌</span>
                )}

                {conversation.is_muted && (
                  <span className="muted-conversation-icon">🔇</span>
                )}

                {conversationUser.is_blocked_by_me && (
                  <span className="blocked-conversation-icon">🚫</span>
                )}
              </div>
            </div>
          </div>

          <div className="conversation-preview">
            <span className="conversation-last-message">
              {conversation.last_message?.is_deleted
                ? "This message was deleted"
                : conversation.last_message?.text ||
                  conversation.last_message?.attachment_name ||
                  "No messages yet"}
            </span>

            {conversation.last_message && (
              <span className="conversation-time">
                {formatShortTime(conversation.last_message.created_at)}
              </span>
            )}
          </div>
        </div>

        {conversation.unread_count > 0 && (
          <span className="unread-badge">{conversation.unread_count}</span>
        )}

        {conversation.unread_count === 0 && conversation.is_marked_unread && (
          <span className="manual-unread-dot" />
        )}
      </button>

      <SidebarConversationMenu
        conversation={conversation}
        conversationUser={conversationUser}
        isMenuOpen={isMenuOpen}
        isDeleting={isDeleting}
        isBlockingUserId={isBlockingUserId}
        setOpenedMenuConversationId={setOpenedMenuConversationId}
        toggleConversationMenu={toggleConversationMenu}
        openDeleteConfirm={openDeleteConfirm}
        openClearHistoryConfirm={openClearHistoryConfirm}
        openBlockConfirm={openBlockConfirm}
        openUnblockConfirm={openUnblockConfirm}
        handleMuteConversation={handleMuteConversation}
        handlePinConversation={handlePinConversation}
        handleMarkConversationAsUnread={handleMarkConversationAsUnread}
        handleOpenContactNicknameModal={handleOpenContactNicknameModal}
      />
    </div>
  );
}

export default SidebarConversationItem;
