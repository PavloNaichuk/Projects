import { useState, type ChangeEvent, type FormEvent } from "react";
import type { User } from "../api/auth";

type ProfileSettingsModalProps = {
  currentUser: User;
  avatarError: string;
  profileError: string;
  isAvatarUpdating: boolean;
  isProfileUpdating: boolean;
  handleClose: () => void;
  handleCurrentUserAvatarChange: (
    event: ChangeEvent<HTMLInputElement>
  ) => Promise<void>;
  handleDeleteCurrentUserAvatar: () => Promise<void>;
  handleUpdateCurrentUserProfile: (
    username: string,
    email: string
  ) => Promise<void>;
};

function ProfileSettingsModal({
  currentUser,
  avatarError,
  profileError,
  isAvatarUpdating,
  isProfileUpdating,
  handleClose,
  handleCurrentUserAvatarChange,
  handleDeleteCurrentUserAvatar,
  handleUpdateCurrentUserProfile,
}: ProfileSettingsModalProps) {
  const [username, setUsername] = useState(currentUser.username);
  const [email, setEmail] = useState(currentUser.email);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    await handleUpdateCurrentUserProfile(username, email);
  }

  const initial = currentUser.username.trim().charAt(0).toUpperCase() || "?";

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
      </div>
    </div>
  );
}

export default ProfileSettingsModal;