import type { User } from "../api/auth";
import UserAvatar from "./UserAvatar";

type SidebarHeaderProps = {
  currentUser: User;
  handleOpenProfileSettings: () => void;
  handleLogout: () => Promise<void>;
};

function SidebarHeader({
  currentUser,
  handleOpenProfileSettings,
  handleLogout,
}: SidebarHeaderProps) {
  return (
    <div className="sidebar-header">
      <h1>Messenger</h1>

      <button
        type="button"
        className="sidebar-profile-button"
        onClick={handleOpenProfileSettings}
      >
        <UserAvatar user={currentUser} size="large" />

        <div className="sidebar-profile-info">
          <p>{currentUser.display_name}</p>
          <span>Profile settings</span>
        </div>
      </button>

      <button className="logout-button" type="button" onClick={handleLogout}>
        Logout
      </button>
    </div>
  );
}

export default SidebarHeader;
