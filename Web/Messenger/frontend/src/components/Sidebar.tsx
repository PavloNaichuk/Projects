import type { FormEvent } from "react";
import type { User } from "../api/auth";
import type { Conversation } from "../api/conversations";
import { formatShortTime, getConversationName } from "../utils/chat";

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

  handleLogout: () => Promise<void>;
  handleSearchUsers: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleStartConversation: (user: User) => Promise<void>;
  handleSelectConversation: (conversation: Conversation) => Promise<void>;
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
  handleLogout,
  handleSearchUsers,
  handleStartConversation,
  handleSelectConversation,
}: SidebarProps) {
  return (
    <aside className="sidebar">
      <div className="sidebar-header">
        <h1>Messenger</h1>
        <p>Logged in as {currentUser.username}</p>
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
              <span className="user-search-name">{user.username}</span>
              <span className="user-search-email">{user.email}</span>
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

          const isOnline = otherParticipant
            ? onlineUserIds.includes(otherParticipant.user.id)
            : false;

          return (
            <button
              key={conversation.id}
              type="button"
              className={
                selectedConversation?.id === conversation.id
                  ? "conversation-item active"
                  : "conversation-item"
              }
              onClick={() => handleSelectConversation(conversation)}
            >
              <div className="conversation-name">
                <span>{getConversationName(conversation, currentUser)}</span>
                {isOnline && <span className="online-dot" title="Online" />}
              </div>

              <div className="conversation-preview">
                <span className="conversation-last-message">
                  {conversation.last_message?.is_deleted
                    ? "This message was deleted"
                    : conversation.last_message?.text || "No messages yet"}
                </span>

                {conversation.last_message && (
                  <span className="conversation-time">
                    {formatShortTime(conversation.last_message.created_at)}
                  </span>
                )}
              </div>

              {conversation.unread_count > 0 && (
                <span className="unread-badge">{conversation.unread_count}</span>
              )}
            </button>
          );
        })}
      </div>
    </aside>
  );
}

export default Sidebar;