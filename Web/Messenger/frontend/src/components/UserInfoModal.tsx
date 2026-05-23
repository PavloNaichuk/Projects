import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import { getUserDisplayName } from "../utils/chat";

type UserInfoModalProps = {
  user: User;
  conversation: Conversation;
  isOnline: boolean;
  isDeletingConversation: boolean;
  handleClose: () => void;
  handleMuteConversation: (conversation: Conversation) => Promise<void>;
  handlePinConversation: (conversation: Conversation) => Promise<void>;
  handleOpenContactNicknameModal: (conversation: Conversation) => void;
  handleDeleteConversation: (
    conversation: Conversation,
    mode: DeleteConversationMode
  ) => Promise<void>;
};

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

function UserInfoModal({ user, isOnline, handleClose }: UserInfoModalProps) {
  const displayName = getUserDisplayName(user);
  const initial = displayName.trim().charAt(0).toUpperCase() || "?";
  const shouldShowUsername = user.display_name !== user.username;

  return (
    <div className="modal-backdrop">
      <div className="user-info-modal">
        <div className="user-info-header">
          <h3>Contact info</h3>

          <button type="button" onClick={handleClose}>
            ×
          </button>
        </div>

        <div className="user-info-profile">
          <span className="user-avatar large">
            {user.avatar_url ? (
              <img src={user.avatar_url} alt={displayName} />
            ) : (
              <span>{initial}</span>
            )}

            {isOnline && <span className="avatar-online-dot" />}
          </span>

          <div className="user-info-profile-text">
            <h4>{displayName}</h4>

            {shouldShowUsername && <span>@{user.username}</span>}

            <p
              className={
                isOnline ? "chat-user-status online" : "chat-user-status offline"
              }
            >
              {isOnline ? "Online" : formatLastSeen(user.last_seen_at)}
            </p>
          </div>
        </div>
      </div>
    </div>
  );
}

export default UserInfoModal;