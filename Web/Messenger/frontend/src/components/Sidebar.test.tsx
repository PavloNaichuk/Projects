import { render, screen, waitFor } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "../api/auth";
import type {
  Conversation,
  DeleteConversationMode,
} from "../api/conversations";
import Sidebar from "./Sidebar";

const currentUser = {
  id: 1,
  username: "pavlo",
  email: "pavlo@test.ua",
  display_name: "Pavlo",
  avatar_url: null,
  is_online: true,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

const otherUser = {
  id: 2,
  username: "user44",
  email: "user44@test.ua",
  display_name: "User 44",
  avatar_url: null,
  is_online: false,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

const blockedUser = {
  ...otherUser,
  is_blocked_by_me: true,
} as User;

function createConversation(overrides = {}) {
  return {
    id: 10,
    participants: [
      {
        id: 1,
        user: currentUser,
        joined_at: "2026-06-05T10:00:00Z",
      },
      {
        id: 2,
        user: otherUser,
        joined_at: "2026-06-05T10:00:00Z",
      },
    ],
    last_message: {
      id: 100,
      conversation: 10,
      sender: otherUser,
      text: "Hello from user44",
      attachment: null,
      attachment_name: "",
      attachment_content_type: "",
      attachment_size: null,
      reply_to: null,
      forwarded_from: null,
      reactions: [],
      is_read: false,
      is_delivered: false,
      is_deleted: false,
      edited_at: null,
      deleted_at: null,
      delivered_at: null,
      created_at: "2026-06-05T10:00:00Z",
      updated_at: "2026-06-05T10:00:00Z",
    },
    unread_count: 2,
    is_muted: false,
    is_pinned: false,
    is_marked_unread: false,
    created_at: "2026-06-05T10:00:00Z",
    updated_at: "2026-06-05T10:00:00Z",
    ...overrides,
  } as unknown as Conversation;
}

function renderSidebar(overrides = {}) {
  const props = {
    currentUser,
    onlineUserIds: [],
    conversations: [createConversation()],
    selectedConversation: null,
    userSearchQuery: "",
    setUserSearchQuery: vi.fn(),
    searchResults: [],
    isSearchingUsers: false,
    userSearchError: "",
    isDeletingConversationId: null,
    isBlockingUserId: null,
    handleLogout: vi.fn().mockResolvedValue(undefined),
    handleOpenProfileSettings: vi.fn(),
    handleSearchUsers: vi.fn().mockResolvedValue(undefined),
    handleStartConversation: vi.fn().mockResolvedValue(undefined),
    handleSelectConversation: vi.fn().mockResolvedValue(undefined),
    handleDeleteConversation: vi.fn().mockResolvedValue(undefined),
    handleMuteConversation: vi.fn().mockResolvedValue(undefined),
    handlePinConversation: vi.fn().mockResolvedValue(undefined),
    handleMarkConversationAsUnread: vi.fn().mockResolvedValue(undefined),
    handleClearConversationHistory: vi.fn().mockResolvedValue(undefined),
    handleOpenContactNicknameModal: vi.fn(),
    handleBlockUser: vi.fn().mockResolvedValue(undefined),
    handleUnblockUser: vi.fn().mockResolvedValue(undefined),
    ...overrides,
  };

  render(<Sidebar {...props} />);

  return props;
}

async function openConversationMenu(user: ReturnType<typeof userEvent.setup>) {
  await user.click(screen.getByTitle("Conversation actions"));
}

describe("Sidebar", () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it("renders profile, search, conversations and logout button", () => {
    renderSidebar();

    expect(screen.getByText("Messenger")).toBeTruthy();
    expect(screen.getByText("Pavlo")).toBeTruthy();
    expect(screen.getByText("Profile settings")).toBeTruthy();
    expect(screen.getByPlaceholderText("Search users...")).toBeTruthy();
    expect(screen.getByText("User 44")).toBeTruthy();
    expect(screen.getByText("Hello from user44")).toBeTruthy();
    expect(screen.getByText("2")).toBeTruthy();
    expect(screen.getByRole("button", { name: "Logout" })).toBeTruthy();
  });

  it("calls profile settings and logout handlers", async () => {
    const user = userEvent.setup();
    const props = renderSidebar();

    await user.click(screen.getByText("Profile settings"));
    await user.click(screen.getByRole("button", { name: "Logout" }));

    expect(props.handleOpenProfileSettings).toHaveBeenCalledTimes(1);
    expect(props.handleLogout).toHaveBeenCalledTimes(1);
  });

  it("updates search query and submits user search", async () => {
    const user = userEvent.setup();
    const props = renderSidebar();

    await user.type(screen.getByPlaceholderText("Search users..."), "user44");
    await user.click(screen.getByRole("button", { name: "Search" }));

    expect(props.setUserSearchQuery).toHaveBeenCalled();
    expect(props.handleSearchUsers).toHaveBeenCalledTimes(1);
  });

  it("renders search results and starts conversation", async () => {
    const user = userEvent.setup();
    const props = renderSidebar({
      conversations: [],
      searchResults: [otherUser],
    });

    await user.click(screen.getByRole("button", { name: /User 44/i }));

    expect(screen.getByText("Users")).toBeTruthy();
    expect(props.handleStartConversation).toHaveBeenCalledWith(otherUser);
  });

  it("selects a conversation", async () => {
    const user = userEvent.setup();
    const conversation = createConversation();
    const props = renderSidebar({
      conversations: [conversation],
    });

    await user.click(screen.getByRole("button", { name: /User 44/i }));

    expect(props.handleSelectConversation).toHaveBeenCalledWith(conversation);
  });

  it("handles pin, mark unread, mute and rename actions from menu", async () => {
    const user = userEvent.setup();
    const conversation = createConversation();
    const props = renderSidebar({
      conversations: [conversation],
    });

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Pin chat" }));
    expect(props.handlePinConversation).toHaveBeenCalledWith(conversation);

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Mark as unread" }));
    expect(props.handleMarkConversationAsUnread).toHaveBeenCalledWith(
      conversation
    );

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Mute sound" }));
    expect(props.handleMuteConversation).toHaveBeenCalledWith(conversation);

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Rename contact" }));
    expect(props.handleOpenContactNicknameModal).toHaveBeenCalledWith(
      conversation
    );
  });

  it("shows unpin, unmute and marked unread states", async () => {
    const user = userEvent.setup();

    renderSidebar({
      conversations: [
        createConversation({
          is_pinned: true,
          is_muted: true,
          is_marked_unread: true,
          unread_count: 0,
        }),
      ],
    });

    await openConversationMenu(user);

    expect(screen.getByRole("button", { name: "Unpin chat" })).toBeTruthy();
    expect(screen.getByRole("button", { name: "Unmute sound" })).toBeTruthy();
    expect(screen.getByRole("button", { name: "Marked as unread" })).toHaveAttribute(
      "disabled"
    );
  });

  it("confirms blocking a user", async () => {
    const user = userEvent.setup();
    const props = renderSidebar();

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Block user" }));

    expect(screen.getByText("Block User 44?")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "Block user" }));

    await waitFor(() => {
      expect(props.handleBlockUser).toHaveBeenCalledWith(otherUser);
    });
  });

  it("confirms unblocking a user", async () => {
    const user = userEvent.setup();
    const conversation = createConversation({
      participants: [
        {
          id: 1,
          user: currentUser,
          joined_at: "2026-06-05T10:00:00Z",
        },
        {
          id: 2,
          user: blockedUser,
          joined_at: "2026-06-05T10:00:00Z",
        },
      ],
    });

    const props = renderSidebar({
      conversations: [conversation],
    });

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Unblock user" }));

    expect(screen.getByText("Unblock User 44?")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "Unblock user" }));

    await waitFor(() => {
      expect(props.handleUnblockUser).toHaveBeenCalledWith(blockedUser);
    });
  });

  it("cancels block confirmation", async () => {
    const user = userEvent.setup();
    const props = renderSidebar();

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Block user" }));
    await user.click(screen.getByRole("button", { name: "Cancel" }));

    expect(screen.queryByText("Block User 44?")).toBeNull();
    expect(props.handleBlockUser).not.toHaveBeenCalled();
  });

  it("confirms clearing chat history", async () => {
    const user = userEvent.setup();
    const conversation = createConversation();
    const props = renderSidebar({
      conversations: [conversation],
    });

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Clear chat history" }));

    expect(screen.getByText("Clear chat history?")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "Clear history" }));

    await waitFor(() => {
      expect(props.handleClearConversationHistory).toHaveBeenCalledWith(
        conversation
      );
    });
  });

  it("confirms deleting conversation for current user", async () => {
    const user = userEvent.setup();
    const conversation = createConversation();
    const props = renderSidebar({
      conversations: [conversation],
    });

    await openConversationMenu(user);
    await user.click(screen.getByRole("button", { name: "Delete for me" }));

    expect(screen.getByText("Delete conversation for you?")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "Delete for me" }));

    await waitFor(() => {
      expect(props.handleDeleteConversation).toHaveBeenCalledWith(
        conversation,
        "for_me" satisfies DeleteConversationMode
      );
    });
  });

  it("confirms deleting conversation for everyone", async () => {
    const user = userEvent.setup();
    const conversation = createConversation();
    const props = renderSidebar({
      conversations: [conversation],
    });

    await openConversationMenu(user);
    await user.click(
      screen.getByRole("button", { name: "Delete for everyone" })
    );

    expect(screen.getByText("Delete conversation for everyone?")).toBeTruthy();

    await user.click(
      screen.getByRole("button", { name: "Delete for everyone" })
    );

    await waitFor(() => {
      expect(props.handleDeleteConversation).toHaveBeenCalledWith(
        conversation,
        "for_everyone" satisfies DeleteConversationMode
      );
    });
  });

  it("shows empty state when there are no conversations", () => {
    renderSidebar({
      conversations: [],
    });

    expect(screen.getByText("No conversations yet.")).toBeTruthy();
  });

  it("shows search error", () => {
    renderSidebar({
      userSearchError: "Search failed.",
    });

    expect(screen.getByText("Search failed.")).toBeTruthy();
  });
});
