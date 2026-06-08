import { useState, type FormEvent } from "react";
import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import { formatShortTime, getConversationName } from "../utils/chat";
import SidebarHeader from "./SidebarHeader";
import SidebarUserSearch from "./SidebarUserSearch";
import UserAvatar from "./UserAvatar";

type PendingDelete = {
  conversation: Conversation;
  mode: DeleteConversationMode;
};

type PendingBlockAction = {
  user: User;
  action: "block" | "unblock";
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
  isBlockingUserId: number | null;

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
  handlePinConversation: (conversation: Conversation) => Promise<void>;
  handleMarkConversationAsUnread: (conversation: Conversation) => Promise<void>;
  handleClearConversationHistory: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
  handleBlockUser: (user: User) => Promise<void>;
  handleUnblockUser: (user: User) => Promise<void>;
};

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
  isBlockingUserId,
  handleLogout,
  handleOpenProfileSettings,
  handleSearchUsers,
  handleStartConversation,
  handleSelectConversation,
  handleDeleteConversation,
  handleMuteConversation,
  handlePinConversation,
  handleMarkConversationAsUnread,
  handleClearConversationHistory,
  handleOpenContactNicknameModal,
  handleBlockUser,
  handleUnblockUser,
}: SidebarProps) {
  const [openedMenuConversationId, setOpenedMenuConversationId] = useState<
    number | null
  >(null);
  const [pendingDelete, setPendingDelete] = useState<PendingDelete | null>(null);
  const [pendingClearHistory, setPendingClearHistory] = useState<Conversation | null>(null);
  const [pendingBlockAction, setPendingBlockAction] =
    useState<PendingBlockAction | null>(null);

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

  function openClearHistoryConfirm(conversation: Conversation) {
    setOpenedMenuConversationId(null);
    setPendingClearHistory(conversation);
  }

  function openBlockConfirm(user: User) {
    setOpenedMenuConversationId(null);
    setPendingBlockAction({
      user,
      action: "block",
    });
  }

  function openUnblockConfirm(user: User) {
    setOpenedMenuConversationId(null);
    setPendingBlockAction({
      user,
      action: "unblock",
    });
  }

  async function confirmBlockAction() {
    if (!pendingBlockAction) {
      return;
    }

    if (pendingBlockAction.action === "block") {
      await handleBlockUser(pendingBlockAction.user);
    } else {
      await handleUnblockUser(pendingBlockAction.user);
    }

    setPendingBlockAction(null);
  }

  function closeBlockConfirm() {
    setPendingBlockAction(null);
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

  async function confirmClearHistory() {
    if (!pendingClearHistory) {
      return;
    }

    await handleClearConversationHistory(pendingClearHistory);

    setPendingClearHistory(null);
  }

  function closeClearHistoryConfirm() {
    setPendingClearHistory(null);
  }

  return (
    <aside className="sidebar">
      <SidebarHeader
        currentUser={currentUser}
        handleOpenProfileSettings={handleOpenProfileSettings}
        handleLogout={handleLogout}
      />

      <SidebarUserSearch
        userSearchQuery={userSearchQuery}
        setUserSearchQuery={setUserSearchQuery}
        searchResults={searchResults}
        isSearchingUsers={isSearchingUsers}
        userSearchError={userSearchError}
        handleSearchUsers={handleSearchUsers}
        handleStartConversation={handleStartConversation}
      />

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
              className={[
                "conversation-item",
                selectedConversation?.id === conversation.id ? "active" : "",
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
                <UserAvatar
                  user={conversationUser}
                  size="medium"
                  isOnline={isOnline}
                />

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
                  <span className="unread-badge">
                    {conversation.unread_count}
                  </span>
                )}

                {conversation.unread_count === 0 &&
                  conversation.is_marked_unread && (
                    <span className="manual-unread-dot" />
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
                      disabled={
                        isDeleting || isBlockingUserId === conversationUser.id
                      }
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

      {pendingBlockAction && (
        <div className="modal-backdrop">
          <div className="confirm-modal" role="dialog" aria-modal="true">
            <h3>
              {pendingBlockAction.action === "block"
                ? `Block ${pendingBlockAction.user.display_name}?`
                : `Unblock ${pendingBlockAction.user.display_name}?`}
            </h3>

            <p>
              {pendingBlockAction.action === "block"
                ? "They will not be able to send you messages. You will not be able to send messages to them until you unblock this user."
                : "You will be able to send messages to each other again."}
            </p>

            <div className="confirm-modal-actions">
              <button
                type="button"
                className="confirm-modal-cancel"
                onClick={closeBlockConfirm}
                disabled={isBlockingUserId !== null}
              >
                Cancel
              </button>

              <button
                type="button"
                className={
                  pendingBlockAction.action === "block"
                    ? "confirm-modal-delete danger"
                    : "confirm-modal-delete"
                }
                onClick={confirmBlockAction}
                disabled={isBlockingUserId !== null}
              >
                {isBlockingUserId !== null
                  ? "Updating..."
                  : pendingBlockAction.action === "block"
                    ? "Block user"
                    : "Unblock user"}
              </button>
            </div>
          </div>
        </div>
      )}

      {pendingClearHistory && (
        <div className="modal-backdrop">
          <div className="confirm-modal" role="dialog" aria-modal="true">
            <h3>Clear chat history?</h3>

            <p>
              This will remove all messages only for you. The conversation will
              stay in your list. The other user will still see the messages.
            </p>

            <div className="confirm-modal-actions">
              <button
                type="button"
                className="confirm-modal-cancel"
                onClick={closeClearHistoryConfirm}
                disabled={isDeletingConversationId !== null}
              >
                Cancel
              </button>

              <button
                type="button"
                className="confirm-modal-delete danger"
                onClick={confirmClearHistory}
                disabled={isDeletingConversationId !== null}
              >
                {isDeletingConversationId !== null
                  ? "Clearing..."
                  : "Clear history"}
              </button>
            </div>
          </div>
        </div>
      )}

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