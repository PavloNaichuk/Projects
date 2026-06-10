import { render, screen, waitFor } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "../api/auth";
import ProfileSettingsModal from "./ProfileSettingsModal";

const currentUser = {
  id: 1,
  username: "pavlo",
  email: "pavlo@test.ua",
  display_name: "pavlo",
  avatar_url: null,
  is_online: true,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

const blockedUser = {
  id: 2,
  username: "user44",
  email: "user44@test.ua",
  display_name: "Blocked User",
  avatar_url: null,
  is_online: false,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: true,
  has_blocked_me: false,
} as User;

function renderProfileSettingsModal(overrides = {}) {
  const props = {
    currentUser,
    avatarError: "",
    profileError: "",
    isAvatarUpdating: false,
    isProfileUpdating: false,
    blockedUsers: [],
    blockedUsersError: "",
    isBlockedUsersLoading: false,
    isUnblockingUserId: null,
    handleClose: vi.fn(),
    handleCurrentUserAvatarChange: vi.fn().mockResolvedValue(undefined),
    handleDeleteCurrentUserAvatar: vi.fn().mockResolvedValue(undefined),
    handleUpdateCurrentUserProfile: vi.fn().mockResolvedValue(undefined),
    handleLoadBlockedUsers: vi.fn().mockResolvedValue(undefined),
    handleUnblockBlockedUser: vi.fn().mockResolvedValue(undefined),
    ...overrides,
  };

  render(<ProfileSettingsModal {...props} />);

  return props;
}

describe("ProfileSettingsModal", () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it("renders current user profile data", () => {
    renderProfileSettingsModal();

    expect(screen.getByText("Profile settings")).toBeTruthy();
    expect(screen.getByLabelText("Username")).toHaveProperty("value", "pavlo");
    expect(screen.getByLabelText("Email")).toHaveProperty(
      "value",
      "pavlo@test.ua"
    );
    expect(screen.getByRole("status")).toHaveTextContent(
      "Email status: Not verified ⚠️"
    );
    expect(screen.getByText("Show blocked users")).toBeTruthy();
  });

  it("shows verified email status", () => {
    renderProfileSettingsModal({
      currentUser: {
        ...currentUser,
        email_verified_at: "2026-06-10T10:00:00Z",
        is_email_verified: true,
      },
    });

    expect(screen.getByRole("status")).toHaveTextContent(
      "Email status: Verified ✅"
    );
  });

  it("shows unsaved email change status when email is changed", async () => {
    const user = userEvent.setup();

    renderProfileSettingsModal({
      currentUser: {
        ...currentUser,
        email_verified_at: "2026-06-10T10:00:00Z",
        is_email_verified: true,
      },
    });

    await user.clear(screen.getByLabelText("Email"));
    await user.type(screen.getByLabelText("Email"), "newemail@test.ua");

    expect(screen.getByRole("status")).toHaveTextContent(
      "Email status: Unsaved email change"
    );
  });

  it("submits updated username and email", async () => {
    const user = userEvent.setup();
    const props = renderProfileSettingsModal();

    await user.clear(screen.getByLabelText("Username"));
    await user.type(screen.getByLabelText("Username"), "newpavlo");

    await user.clear(screen.getByLabelText("Email"));
    await user.type(screen.getByLabelText("Email"), "newpavlo@test.ua");

    await user.click(screen.getByRole("button", { name: "Save changes" }));

    expect(props.handleUpdateCurrentUserProfile).toHaveBeenCalledWith(
      "newpavlo",
      "newpavlo@test.ua"
    );
  });

  it("calls close handler when cancel button is clicked", async () => {
    const user = userEvent.setup();
    const props = renderProfileSettingsModal();

    await user.click(screen.getByRole("button", { name: "Cancel" }));

    expect(props.handleClose).toHaveBeenCalledTimes(1);
  });

  it("loads blocked users when blocked users panel is opened", async () => {
    const user = userEvent.setup();
    const props = renderProfileSettingsModal();

    await user.click(screen.getByRole("button", { name: "Show blocked users" }));

    await waitFor(() => {
      expect(props.handleLoadBlockedUsers).toHaveBeenCalledTimes(1);
    });

    expect(screen.getByText("Hide blocked users")).toBeTruthy();
    expect(screen.getByText("Blocked users")).toBeTruthy();
    expect(screen.getByText("You have no blocked users.")).toBeTruthy();
  });

  it("hides blocked users panel when toggle is clicked twice", async () => {
    const user = userEvent.setup();
    renderProfileSettingsModal();

    await user.click(screen.getByRole("button", { name: "Show blocked users" }));
    await user.click(screen.getByRole("button", { name: "Hide blocked users" }));

    expect(screen.queryByText("Blocked users")).toBeNull();
    expect(screen.getByText("Show blocked users")).toBeTruthy();
  });

  it("renders blocked users and calls unblock handler", async () => {
    const user = userEvent.setup();
    const props = renderProfileSettingsModal({
      blockedUsers: [blockedUser],
    });

    await user.click(screen.getByRole("button", { name: "Show blocked users" }));
    await user.click(screen.getByRole("button", { name: "Unblock" }));

    expect(screen.getByText("Blocked User")).toBeTruthy();
    expect(screen.getByText("@user44")).toBeTruthy();
    expect(props.handleUnblockBlockedUser).toHaveBeenCalledWith(blockedUser);
  });

  it("shows blocked users error", async () => {
    const user = userEvent.setup();

    renderProfileSettingsModal({
      blockedUsersError: "Failed to load blocked users.",
    });

    await user.click(screen.getByRole("button", { name: "Show blocked users" }));

    expect(screen.getByText("Failed to load blocked users.")).toBeTruthy();
  });

  it("disables submit button while profile is updating", () => {
    renderProfileSettingsModal({
      isProfileUpdating: true,
    });

    const button = screen.getByRole("button", { name: "Saving..." });

    expect(button).toHaveAttribute("disabled");
  });

  it("shows unblock loading state for selected blocked user", async () => {
    const user = userEvent.setup();

    renderProfileSettingsModal({
      blockedUsers: [blockedUser],
      isUnblockingUserId: blockedUser.id,
    });

    await user.click(screen.getByRole("button", { name: "Show blocked users" }));

    const button = screen.getByRole("button", { name: "Unblocking..." });

    expect(button).toHaveAttribute("disabled");
  });
});
