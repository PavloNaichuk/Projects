import { useState, type ChangeEvent, type FormEvent } from "react";
import type { User } from "../api/auth";

type ProfileSettingsModalProps = {
  currentUser: User;
  avatarError: string;
  profileError: string;
  isAvatarUpdating: boolean;
  isProfileUpdating: boolean;
  blockedUsers: User[];
  blockedUsersError: string;
  isBlockedUsersLoading: boolean;
  isUnblockingUserId: number | null;
  handleClose: () => void;
  handleCurrentUserAvatarChange: (
    event: ChangeEvent<HTMLInputElement>
  ) => Promise<void>;
  handleDeleteCurrentUserAvatar: () => Promise<void>;
  handleUpdateCurrentUserProfile: (
    username: string,
    email: string
  ) => Promise<void>;
  handleLoadBlockedUsers: () => Promise<void>;
  handleUnblockBlockedUser: (user: User) => Promise<void>;
};

function ProfileSettingsModal({
  currentUser,
  avatarError,
  profileError,
  isAvatarUpdating,
  isProfileUpdating,
  blockedUsers,
  blockedUsersError,
  isBlockedUsersLoading,
  isUnblockingUserId,
  handleClose,
  handleCurrentUserAvatarChange,
  handleDeleteCurrentUserAvatar,
  handleUpdateCurrentUserProfile,
  handleLoadBlockedUsers,
  handleUnblockBlockedUser,
}: ProfileSettingsModalProps) {
  const [username, setUsername] = useState(currentUser.username);
  const [email, setEmail] = useState(currentUser.email);
  const [isBlockedUsersOpen, setIsBlockedUsersOpen] = useState(false);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    await handleUpdateCurrentUserProfile(username, email);
  }

  async function handleToggleBlockedUsers() {
    const shouldOpen = !isBlockedUsersOpen;

    setIsBlockedUsersOpen(shouldOpen);

    if (shouldOpen) {
      await handleLoadBlockedUsers();
    }
  }

  const initial = currentUser.username.trim().charAt(0).toUpperCase() || "?";
  const normalizedEmail = email.trim().toLowerCase();
  const savedEmail = currentUser.email.trim().toLowerCase();
  const isEmailChanged = normalizedEmail !== savedEmail;

  const emailStatusClass = isEmailChanged
    ? "changed"
    : currentUser.is_email_verified
      ? "verified"
      : "unverified";

  const emailStatusText = isEmailChanged
    ? "Unsaved email change"
    : currentUser.is_email_verified
      ? "Verified ✅"
      : "Not verified ⚠️";

  return (
    <div className="modal-backdrop">
      <div className="profile-modal">
        <div className="profile-modal-header">
          <h3>Profile settings</h3>

          <button type="button" onClick={handleClose}>
            ×
          </button>
        </div>

        <div className="profile-modal-avatar">
          <span className="user-avatar large">
            {currentUser.avatar_url ? (
              <img src={currentUser.avatar_url} alt={currentUser.username} />
            ) : (
              <span>{initial}</span>
            )}
          </span>

          <div className="profile-avatar-actions">
            <label className="avatar-upload-button">
              {isAvatarUpdating ? "Uploading..." : "Change avatar"}
              <input
                type="file"
                accept="image/png,image/jpeg,image/webp"
                onChange={handleCurrentUserAvatarChange}
                disabled={isAvatarUpdating}
              />
            </label>

            {currentUser.avatar_url && (
              <button
                type="button"
                className="avatar-delete-button"
                onClick={handleDeleteCurrentUserAvatar}
                disabled={isAvatarUpdating}
              >
                Remove avatar
              </button>
            )}
          </div>
        </div>

        {avatarError && <div className="profile-modal-error">{avatarError}</div>}

        <form className="profile-modal-form" onSubmit={handleSubmit}>
          <label>
            Username
            <input
              type="text"
              value={username}
              onChange={(event) => setUsername(event.target.value)}
              disabled={isProfileUpdating}
            />
          </label>

          <label>
            Email
            <input
              type="email"
              value={email}
              onChange={(event) => setEmail(event.target.value)}
              disabled={isProfileUpdating}
            />
          </label>

        <div
          className={`profile-email-status ${emailStatusClass}`}
          role="status"
        >
          Email status: {emailStatusText}
        </div>

          {profileError && (
            <div className="profile-modal-error">{profileError}</div>
          )}

          <div className="profile-modal-actions">
            <button
              type="button"
              className="profile-modal-cancel"
              onClick={handleClose}
              disabled={isProfileUpdating}
            >
              Cancel
            </button>

            <button type="submit" disabled={isProfileUpdating}>
              {isProfileUpdating ? "Saving..." : "Save changes"}
            </button>
          </div>
        </form>

        <div className="profile-blocked-users">
          <button
            type="button"
            className="profile-blocked-users-toggle"
            onClick={handleToggleBlockedUsers}
            disabled={isBlockedUsersLoading}
          >
            {isBlockedUsersOpen ? "Hide blocked users" : "Show blocked users"}
          </button>

          {isBlockedUsersOpen && (
            <div className="profile-blocked-users-panel">
              <div className="profile-blocked-users-header">
                <h4>Blocked users</h4>

                <button
                  type="button"
                  onClick={handleLoadBlockedUsers}
                  disabled={isBlockedUsersLoading}
                >
                  {isBlockedUsersLoading ? "Loading..." : "Refresh"}
                </button>
              </div>

              {blockedUsersError && (
                <div className="profile-modal-error">{blockedUsersError}</div>
              )}

              {!isBlockedUsersLoading && blockedUsers.length === 0 && (
                <div className="profile-blocked-users-empty">
                  You have no blocked users.
                </div>
              )}

              {blockedUsers.length > 0 && (
                <div className="profile-blocked-users-list">
                  {blockedUsers.map((user) => {
                    const userInitial =
                      user.display_name.trim().charAt(0).toUpperCase() || "?";
                    const isUnblocking = isUnblockingUserId === user.id;

                    return (
                      <div key={user.id} className="profile-blocked-user">
                        <span className="user-avatar small">
                          {user.avatar_url ? (
                            <img src={user.avatar_url} alt={user.display_name} />
                          ) : (
                            <span>{userInitial}</span>
                          )}
                        </span>

                        <div className="profile-blocked-user-info">
                          <strong>{user.display_name}</strong>
                          <span>@{user.username}</span>
                        </div>

                        <button
                          type="button"
                          onClick={() => handleUnblockBlockedUser(user)}
                          disabled={isUnblocking}
                        >
                          {isUnblocking ? "Unblocking..." : "Unblock"}
                        </button>
                      </div>
                    );
                  })}
                </div>
              )}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

export default ProfileSettingsModal;
