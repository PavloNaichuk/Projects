import { useState, type FormEvent } from "react";
import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import { formatShortTime, getConversationName } from "../utils/chat";

type PendingDelete = {
  conversation: Conversation;
  mode: DeleteConversationMode;
};

type SidebarProps = {
  currentUser: User;
  onlineUserIds: number[];

  conversations: Conversation[];
  selectedConversation: Conversation | null;

  userSearchQuery: string;
  setUserSearchQuery: (value: string) => void;

  searchResults: User[];
  isSearchingUsers: boolean;
  userSearchError: string;
  isDeletingConversationId: number | null;

  handleLogout: () => Promise<void>;
  handleOpenProfileSettings: () => void;
  handleSearchUsers: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleStartConversation: (user: User) => Promise<void>;
  handleSelectConversation: (conversation: Conversation) => Promise<void>;
  handleDeleteConversation: (
    conversation: Conversation,
    mode: DeleteConversationMode
  ) => Promise<void>;
  handleMuteConversation: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
};

type UserAvatarProps = {
  user: User;
  size?: "small" | "medium" | "large";
  isOnline?: boolean;
};

function UserAvatar({
  user,
  size = "medium",
  isOnline = false,
}: UserAvatarProps) {
  const initial = user.display_name.trim().charAt(0).toUpperCase() || "?";

  return (
    <span className={`user-avatar ${size}`}>
      {user.avatar_url ? (
        <img src={user.avatar_url} alt={user.display_name} />
      ) : (
        <span>{initial}</span>
      )}

      {isOnline && <span className="avatar-online-dot" />}
    </span>
  );
}

function Sidebar({
  currentUser,
  onlineUserIds,
  conversations,
  selectedConversation,
  userSearchQuery,
  setUserSearchQuery,
  searchResults,
  isSearchingUsers,
  userSearchError,
  isDeletingConversationId,
  handleLogout,
  handleOpenProfileSettings,
  handleSearchUsers,
  handleStartConversation,
  handleSelectConversation,
  handleDeleteConversation,
  handleMuteConversation,
  handleOpenContactNicknameModal,
}: SidebarProps) {
  const [openedMenuConversationId, setOpenedMenuConversationId] = useState<
    number | null
  >(null);
  const [pendingDelete, setPendingDelete] = useState<PendingDelete | null>(null);

  function toggleConversationMenu(conversationId: number) {
    setOpenedMenuConversationId((previousConversationId) =>
      previousConversationId === conversationId ? null : conversationId
    );
  }

  function openDeleteConfirm(
    conversation: Conversation,
    mode: DeleteConversationMode
  ) {
    setOpenedMenuConversationId(null);
    setPendingDelete({
      conversation,
      mode,
    });
  }

  async function confirmDeleteConversation() {
    if (!pendingDelete) {
      return;
    }

    await handleDeleteConversation(
      pendingDelete.conversation,
      pendingDelete.mode
    );

    setPendingDelete(null);
  }

  function closeDeleteConfirm() {
    setPendingDelete(null);
  }

  return (
    <aside className="sidebar">
      <div className="sidebar-header">
        <h1>Messenger</h1>

        <button
          type="button"
          className="sidebar-profile-button"
          onClick={handleOpenProfileSettings}
        >
          <UserAvatar user={currentUser} size="large" />

          <div className="sidebar-profile-info">
            <p>{currentUser.display_name}</p>
            <span>Profile settings</span>
          </div>
        </button>

        <button className="logout-button" type="button" onClick={handleLogout}>
          Logout
        </button>
      </div>

      <form className="search-box" onSubmit={handleSearchUsers}>
        <input
          type="text"
          value={userSearchQuery}
          onChange={(event) => setUserSearchQuery(event.target.value)}
          placeholder="Search users..."
        />
        <button type="submit" disabled={isSearchingUsers}>
          {isSearchingUsers ? "Searching..." : "Search"}
        </button>
      </form>

      {userSearchError && <div className="sidebar-error">{userSearchError}</div>}

      {searchResults.length > 0 && (
        <div className="user-search-results">
          <div className="user-search-title">Users</div>

          {searchResults.map((user) => (
            <button
              key={user.id}
              type="button"
              className="user-search-item"
              onClick={() => handleStartConversation(user)}
            >
              <UserAvatar user={user} size="small" />

              <span className="user-search-text">
                <span className="user-search-name">{user.display_name}</span>
              </span>
            </button>
          ))}
        </div>
      )}

      <div className="conversation-list">
        {conversations.length === 0 && (
          <div className="empty-state">No conversations yet.</div>
        )}

        {conversations.map((conversation) => {
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
              key={conversation.id}
              className={
                selectedConversation?.id === conversation.id
                  ? "conversation-item active"
                  : "conversation-item"
              }
            >
              <button
                type="button"
                className="conversation-main"
                onClick={() => {
                  setOpenedMenuConversationId(null);
                  handleSelectConversation(conversation);
                }}
              >
                <UserAvatar
                  user={conversationUser}
                  size="medium"
                  isOnline={isOnline}
                />

                <div className="conversation-content">
                  <div className="conversation-name">
                    <span>{getConversationName(conversation, currentUser)}</span>

                    {conversation.is_muted && (
                      <span className="muted-conversation-icon">🔇</span>
                    )}
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
                  <span className="unread-badge">
                    {conversation.unread_count}
                  </span>
                )}
              </button>

              <div
                className={
                  isMenuOpen
                    ? "conversation-menu-wrapper open"
                    : "conversation-menu-wrapper"
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
                      onClick={() => openDeleteConfirm(conversation, "for_me")}
                      disabled={isDeleting}
                    >
                      {isDeleting ? "Deleting..." : "Delete for me"}
                    </button>

                    <button
                      type="button"
                      className="danger"
                      onClick={() =>
                        openDeleteConfirm(conversation, "for_everyone")
                      }
                      disabled={isDeleting}
                    >
                      Delete for everyone
                    </button>
                  </div>
                )}
              </div>
            </div>
          );
        })}
      </div>

      {pendingDelete && (
        <div className="modal-backdrop">
          <div className="confirm-modal" role="dialog" aria-modal="true">
            <h3>
              {pendingDelete.mode === "for_everyone"
                ? "Delete conversation for everyone?"
                : "Delete conversation for you?"}
            </h3>

            <p>
              {pendingDelete.mode === "for_everyone"
                ? "This will remove the conversation and all messages for both users."
                : "This will hide the conversation only for you. The other user will still see it."}
            </p>

            <div className="confirm-modal-actions">
              <button
                type="button"
                className="confirm-modal-cancel"
                onClick={closeDeleteConfirm}
                disabled={isDeletingConversationId !== null}
              >
                Cancel
              </button>

              <button
                type="button"
                className={
                  pendingDelete.mode === "for_everyone"
                    ? "confirm-modal-delete danger"
                    : "confirm-modal-delete"
                }
                onClick={confirmDeleteConversation}
                disabled={isDeletingConversationId !== null}
              >
                {isDeletingConversationId !== null
                  ? "Deleting..."
                  : pendingDelete.mode === "for_everyone"
                  ? "Delete for everyone"
                  : "Delete for me"}
              </button>
            </div>
          </div>
        </div>
      )}
    </aside>
  );
}

export default Sidebar;