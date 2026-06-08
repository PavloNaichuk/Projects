import type { FormEvent } from "react";

import type { User } from "../api/auth";
import UserAvatar from "./UserAvatar";

type SidebarUserSearchProps = {
  userSearchQuery: string;
  setUserSearchQuery: (value: string) => void;
  searchResults: User[];
  isSearchingUsers: boolean;
  userSearchError: string;
  handleSearchUsers: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleStartConversation: (user: User) => Promise<void>;
};

function SidebarUserSearch({
  userSearchQuery,
  setUserSearchQuery,
  searchResults,
  isSearchingUsers,
  userSearchError,
  handleSearchUsers,
  handleStartConversation,
}: SidebarUserSearchProps) {
  return (
    <>
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

                {(user.is_blocked_by_me || user.has_blocked_me) && (
                  <span className="blocked-user-label">
                    {user.is_blocked_by_me ? "Blocked by you" : "Blocked you"}
                  </span>
                )}
              </span>
            </button>
          ))}
        </div>
      )}
    </>
  );
}

export default SidebarUserSearch;
