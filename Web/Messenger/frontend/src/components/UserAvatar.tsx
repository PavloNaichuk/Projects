import type { User } from "../api/auth";

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

export default UserAvatar;
