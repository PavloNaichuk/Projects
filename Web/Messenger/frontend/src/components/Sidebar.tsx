import { useState, type FormEvent } from "react";
import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import SidebarConfirmModal from "./SidebarConfirmModal";
import SidebarConversationItem from "./SidebarConversationItem";
import SidebarHeader from "./SidebarHeader";
import SidebarUserSearch from "./SidebarUserSearch";

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

        {conversations.map((conversation) => (
          <SidebarConversationItem
            key={conversation.id}
            currentUser={currentUser}
            onlineUserIds={onlineUserIds}
            conversation={conversation}
            selectedConversationId={selectedConversation?.id ?? null}
            openedMenuConversationId={openedMenuConversationId}
            isDeletingConversationId={isDeletingConversationId}
            isBlockingUserId={isBlockingUserId}
            setOpenedMenuConversationId={setOpenedMenuConversationId}
            toggleConversationMenu={toggleConversationMenu}
            openDeleteConfirm={openDeleteConfirm}
            openClearHistoryConfirm={openClearHistoryConfirm}
            openBlockConfirm={openBlockConfirm}
            openUnblockConfirm={openUnblockConfirm}
            handleSelectConversation={handleSelectConversation}
            handleMuteConversation={handleMuteConversation}
            handlePinConversation={handlePinConversation}
            handleMarkConversationAsUnread={handleMarkConversationAsUnread}
            handleOpenContactNicknameModal={handleOpenContactNicknameModal}
          />
        ))}
      </div>

      {pendingBlockAction && (
        <SidebarConfirmModal
          title={
            pendingBlockAction.action === "block"
              ? `Block ${pendingBlockAction.user.display_name}?`
              : `Unblock ${pendingBlockAction.user.display_name}?`
          }
          message={
            pendingBlockAction.action === "block"
              ? "They will not be able to send you messages. You will not be able to send messages to them until you unblock this user."
              : "You will be able to send messages to each other again."
          }
          confirmLabel={
            pendingBlockAction.action === "block" ? "Block user" : "Unblock user"
          }
          loadingLabel="Updating..."
          isLoading={isBlockingUserId !== null}
          isDanger={pendingBlockAction.action === "block"}
          onCancel={closeBlockConfirm}
          onConfirm={confirmBlockAction}
        />
      )}

      {pendingClearHistory && (
        <SidebarConfirmModal
          title="Clear chat history?"
          message="This will remove all messages only for you. The conversation will stay in your list. The other user will still see the messages."
          confirmLabel="Clear history"
          loadingLabel="Clearing..."
          isLoading={isDeletingConversationId !== null}
          isDanger
          onCancel={closeClearHistoryConfirm}
          onConfirm={confirmClearHistory}
        />
      )}

      {pendingDelete && (
        <SidebarConfirmModal
          title={
            pendingDelete.mode === "for_everyone"
              ? "Delete conversation for everyone?"
              : "Delete conversation for you?"
          }
          message={
            pendingDelete.mode === "for_everyone"
              ? "This will remove the conversation and all messages for both users."
              : "This will hide the conversation only for you. The other user will still see it."
          }
          confirmLabel={
            pendingDelete.mode === "for_everyone"
              ? "Delete for everyone"
              : "Delete for me"
          }
          loadingLabel="Deleting..."
          isLoading={isDeletingConversationId !== null}
          isDanger={pendingDelete.mode === "for_everyone"}
          onCancel={closeDeleteConfirm}
          onConfirm={confirmDeleteConversation}
        />
      )}
    </aside>
  );
}

export default Sidebar;