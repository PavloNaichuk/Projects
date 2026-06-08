import type { FormEvent } from "react";

import type { User } from "../api/auth";
import type { Conversation } from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";

type ChatHeaderProps = {
  selectedConversation: Conversation | null;
  selectedConversationName: string;
  selectedConversationUser: User | null;
  selectedConversationUserIsOnline: boolean;
  messageSearchQuery: string;
  setMessageSearchQuery: (value: string) => void;
  isSearchingMessages: boolean;
  handleSearchMessages: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleClearMessageSearch: () => Promise<void>;
};

type UserAvatarProps = {
  user: User;
  isOnline?: boolean;
};

function UserAvatar({ user, isOnline = false }: UserAvatarProps) {
  const displayName = getUserDisplayName(user);
  const initial = displayName.trim().charAt(0).toUpperCase() || "?";

  return (
    <span className="user-avatar medium">
      {user.avatar_url ? (
        <img src={user.avatar_url} alt={displayName} />
      ) : (
        <span>{initial}</span>
      )}

      {isOnline && <span className="avatar-online-dot" />}
    </span>
  );
}

function formatTime(date: Date) {
  const hours = String(date.getHours()).padStart(2, "0");
  const minutes = String(date.getMinutes()).padStart(2, "0");

  return `${hours}:${minutes}`;
}

function isSameDate(firstDate: Date, secondDate: Date) {
  return (
    firstDate.getFullYear() === secondDate.getFullYear() &&
    firstDate.getMonth() === secondDate.getMonth() &&
    firstDate.getDate() === secondDate.getDate()
  );
}

function formatLastSeen(lastSeenAt: string | null) {
  if (!lastSeenAt) {
    return "Last seen unknown";
  }

  const lastSeenDate = new Date(lastSeenAt);

  if (Number.isNaN(lastSeenDate.getTime())) {
    return "Last seen unknown";
  }

  const now = new Date();
  const diffInMs = now.getTime() - lastSeenDate.getTime();
  const diffInMinutes = Math.floor(diffInMs / 60000);
  const diffInHours = Math.floor(diffInMinutes / 60);

  if (diffInMinutes < 1) {
    return "Last seen just now";
  }

  if (diffInMinutes < 60) {
    return `Last seen ${diffInMinutes} ${
      diffInMinutes === 1 ? "minute" : "minutes"
    } ago`;
  }

  if (diffInHours < 24) {
    return `Last seen ${diffInHours} ${
      diffInHours === 1 ? "hour" : "hours"
    } ago`;
  }

  if (isSameDate(lastSeenDate, now)) {
    return `Last seen today at ${formatTime(lastSeenDate)}`;
  }

  const yesterday = new Date(now);
  yesterday.setDate(now.getDate() - 1);

  if (isSameDate(lastSeenDate, yesterday)) {
    return `Last seen yesterday at ${formatTime(lastSeenDate)}`;
  }

  return `Last seen ${lastSeenDate.toLocaleDateString()} at ${formatTime(
    lastSeenDate
  )}`;
}

function ChatHeader({
  selectedConversation,
  selectedConversationName,
  selectedConversationUser,
  selectedConversationUserIsOnline,
  messageSearchQuery,
  setMessageSearchQuery,
  isSearchingMessages,
  handleSearchMessages,
  handleClearMessageSearch,
}: ChatHeaderProps) {
  const isSelectedUserBlockedByMe = Boolean(
    selectedConversationUser?.is_blocked_by_me
  );
  const hasSelectedUserBlockedMe = Boolean(
    selectedConversationUser?.has_blocked_me
  );
  const isChatBlocked = isSelectedUserBlockedByMe || hasSelectedUserBlockedMe;

  return (
    <header className="chat-header">
      <div className="chat-header-user">
        {selectedConversationUser && (
          <UserAvatar
            user={selectedConversationUser}
            isOnline={selectedConversationUserIsOnline}
          />
        )}

        <div>
          <h2>{selectedConversationName}</h2>

          {selectedConversationUser && (
            <p
              className={
                isChatBlocked
                  ? "chat-user-status blocked"
                  : selectedConversationUserIsOnline
                    ? "chat-user-status online"
                    : "chat-user-status offline"
              }
            >
              {isSelectedUserBlockedByMe
                ? "You blocked this user"
                : hasSelectedUserBlockedMe
                  ? "This user blocked you"
                  : selectedConversationUserIsOnline
                    ? "Online"
                    : formatLastSeen(selectedConversationUser.last_seen_at)}
            </p>
          )}
        </div>
      </div>

      <form className="message-search-form" onSubmit={handleSearchMessages}>
        <input
          type="text"
          value={messageSearchQuery}
          onChange={(event) => setMessageSearchQuery(event.target.value)}
          placeholder="Search messages..."
          disabled={!selectedConversation || isSearchingMessages}
        />

        {messageSearchQuery && (
          <button
            type="button"
            onClick={handleClearMessageSearch}
            disabled={isSearchingMessages}
          >
            Clear
          </button>
        )}

        <button
          type="submit"
          disabled={!selectedConversation || isSearchingMessages}
        >
          {isSearchingMessages ? "Searching..." : "Search"}
        </button>
      </form>
    </header>
  );
}

export default ChatHeader;
