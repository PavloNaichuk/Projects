import { useState, type FormEvent } from "react";
import type { User } from "../api/auth";
import { getUserDisplayName } from "../utils/chat";

type ContactNicknameModalProps = {
  user: User;
  isUpdating: boolean;
  error: string;
  handleClose: () => void;
  handleSave: (nickname: string) => Promise<void>;
};

function ContactNicknameModal({
  user,
  isUpdating,
  error,
  handleClose,
  handleSave,
}: ContactNicknameModalProps) {
  const [nickname, setNickname] = useState(
    user.display_name !== user.username ? user.display_name : ""
  );

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    await handleSave(nickname);
  }

  return (
    <div className="modal-backdrop">
      <div className="contact-nickname-modal">
        <div className="contact-nickname-modal-header">
          <h3>Edit contact name</h3>

          <button type="button" onClick={handleClose}>
            ×
          </button>
        </div>

        <p>
          Real username: <strong>{user.username}</strong>
        </p>

        <form className="contact-nickname-form" onSubmit={handleSubmit}>
          <label>
            Local name
            <input
              type="text"
              value={nickname}
              onChange={(event) => setNickname(event.target.value)}
              placeholder={getUserDisplayName(user)}
              disabled={isUpdating}
              maxLength={50}
            />
          </label>

          <span className="contact-nickname-help">
            Leave empty to show the real username again.
          </span>

          {error && <div className="profile-modal-error">{error}</div>}

          <div className="contact-nickname-actions">
            <button
              type="button"
              className="contact-nickname-cancel"
              onClick={handleClose}
              disabled={isUpdating}
            >
              Cancel
            </button>

            <button type="submit" disabled={isUpdating}>
              {isUpdating ? "Saving..." : "Save"}
            </button>
          </div>
        </form>
      </div>
    </div>
  );
}

export default ContactNicknameModal;