import { render, screen, within } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "../api/auth";
import type { DeleteMessageMode, Message } from "../api/conversations";
import MessageBubble from "./MessageBubble";

const currentUser = {
  id: 1,
  username: "pavlo",
  email: "pavlo@test.ua",
  display_name: "Pavlo",
  avatar_url: null,
  is_online: true,
  last_seen_at: null,
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
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

function createMessage(overrides = {}) {
  return {
    id: 100,
    conversation: 10,
    sender: currentUser,
    text: "Hello from message bubble",
    attachment: null,
    attachment_url: "",
    attachment_name: "",
    attachment_content_type: "",
    attachment_size: null,
    attachment_is_image: false,
    reply_to: null,
    reply_to_message: null,
    forwarded_from: null,
    forwarded_from_message: null,
    reactions: [],
    is_read: true,
    is_delivered: true,
    is_deleted: false,
    edited_at: null,
    deleted_at: null,
    delivered_at: "2026-06-05T10:01:00Z",
    created_at: "2026-06-05T10:00:00Z",
    updated_at: "2026-06-05T10:00:00Z",
    ...overrides,
  } as unknown as Message;
}

function renderMessageBubble(overrides = {}, propOverrides = {}) {
  const message = createMessage(overrides);

  const props = {
    message,
    previousMessage: undefined,
    currentUser,
    searchQuery: "",
    editingMessageId: null,
    editingMessageText: "",
    setEditingMessageText: vi.fn(),
    isEditingMessage: false,
    isDeletingMessageId: null,
    handleStartReplyMessage: vi.fn(),
    handleStartForwardMessage: vi.fn(),
    handleStartEditMessage: vi.fn(),
    handleCancelEditMessage: vi.fn(),
    handleSaveEditedMessage: vi.fn().mockResolvedValue(undefined),
    handleDeleteMessage: vi.fn().mockResolvedValue(undefined),
    handleRemoveMessageAttachment: vi.fn().mockResolvedValue(undefined),
    handleToggleMessageReaction: vi.fn().mockResolvedValue(undefined),
    ...propOverrides,
  };

  render(<MessageBubble {...props} />);

  return props;
}

async function openMessageActions(user: ReturnType<typeof userEvent.setup>) {
  await user.click(screen.getByLabelText("Message actions"));
}

describe("MessageBubble", () => {
  beforeEach(() => {
    vi.clearAllMocks();

    window.HTMLElement.prototype.scrollIntoView = vi.fn();
    document.execCommand = vi.fn(() => true);
  });

  it("renders outgoing message text, date separator, time and read status", () => {
    renderMessageBubble();

    expect(screen.getByText("Hello from message bubble")).toBeTruthy();
    expect(screen.getByText("Read")).toBeTruthy();
    expect(screen.getByText(/2026/)).toBeTruthy();
  });

  it("renders incoming message without own-message status", () => {
    renderMessageBubble({
      sender: otherUser,
      is_read: false,
      is_delivered: false,
    });

    expect(screen.getByText("Hello from message bubble")).toBeTruthy();
    expect(screen.queryByText("Read")).toBeNull();
    expect(screen.queryByText("Delivered")).toBeNull();
    expect(screen.queryByText("Sent")).toBeNull();
  });

  it("renders deleted message placeholder", () => {
    renderMessageBubble({
      text: "",
      is_deleted: true,
    });

    expect(screen.getByText("This message was deleted")).toBeTruthy();
    expect(screen.queryByLabelText("Message actions")).toBeNull();
  });

  it("highlights search query matches", () => {
    renderMessageBubble(
      {
        text: "Find this important message",
      },
      {
        searchQuery: "important",
      }
    );

    expect(screen.getByText("important").tagName.toLowerCase()).toBe("mark");
  });

  it("renders reply preview", () => {
    renderMessageBubble({
      reply_to_message: {
        id: 50,
        sender: otherUser,
        text: "Original message",
        attachment_name: "",
        attachment_is_image: false,
        is_deleted: false,
      },
    });

    expect(screen.getByText("Reply to User 44")).toBeTruthy();
    expect(screen.getByText("Original message")).toBeTruthy();
  });

  it("renders forwarded label", () => {
    renderMessageBubble({
      forwarded_from_message: {
        id: 70,
        sender: otherUser,
      },
    });

    expect(
        screen.getByText((content) =>
            content.includes("Forwarded from") && content.includes("User 44")
        )
        ).toBeTruthy();
  });

  it("toggles reaction from toolbar", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble();

    await user.click(screen.getByRole("button", { name: "👍" }));

    expect(props.handleToggleMessageReaction).toHaveBeenCalledWith(100, "👍");
  });

  it("toggles existing reaction", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble({
      reactions: [
        {
          emoji: "❤️",
          count: 2,
          reacted_by_me: true,
        },
      ],
    });

    const reactionButton = screen
        .getAllByText("❤️")[0]
        .closest("button");

    expect(reactionButton).not.toBeNull();

    await user.click(reactionButton as HTMLButtonElement);

    expect(props.handleToggleMessageReaction).toHaveBeenCalledWith(100, "❤️");
  });

  it("opens menu and starts reply and forward actions", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Reply" }));

    expect(props.handleStartReplyMessage).toHaveBeenCalledWith(props.message);

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Forward" }));

    expect(props.handleStartForwardMessage).toHaveBeenCalledWith(props.message);
  });

  it("opens message info modal and closes it", async () => {
    const user = userEvent.setup();

    renderMessageBubble();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Message info" }));

    expect(screen.getByRole("dialog")).toBeTruthy();
    expect(screen.getByText("Sender")).toBeTruthy();
    expect(screen.getByText("@pavlo")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "×" }));

    expect(screen.queryByRole("dialog")).toBeNull();
  });

  it("copies message text from menu", async () => {
    const user = userEvent.setup();

    renderMessageBubble();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Copy text" }));

    expect(document.execCommand).toHaveBeenCalledWith("copy");
  });

  it("starts editing own text message", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Edit" }));

    expect(props.handleStartEditMessage).toHaveBeenCalledWith(props.message);
  });

  it("does not show edit and delete for everyone for incoming message", async () => {
    const user = userEvent.setup();

    renderMessageBubble({
      sender: otherUser,
    });

    await openMessageActions(user);

    expect(screen.queryByRole("button", { name: "Edit" })).toBeNull();
    expect(
      screen.queryByRole("button", { name: "Delete for everyone" })
    ).toBeNull();
    expect(screen.getByRole("button", { name: "Delete for me" })).toBeTruthy();
  });

  it("deletes message for current user", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Delete for me" }));

    expect(props.handleDeleteMessage).toHaveBeenCalledWith(
      100,
      "for_me" satisfies DeleteMessageMode
    );
  });

  it("deletes own message for everyone", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble();

    await openMessageActions(user);
    await user.click(
      screen.getByRole("button", { name: "Delete for everyone" })
    );

    expect(props.handleDeleteMessage).toHaveBeenCalledWith(
      100,
      "for_everyone" satisfies DeleteMessageMode
    );
  });

  it("renders edit form and saves edited message", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble(
      {},
      {
        editingMessageId: 100,
        editingMessageText: "Edited text",
      }
    );

    const input = screen.getByDisplayValue("Edited text");

    await user.clear(input);
    await user.type(input, "New text");
    await user.click(screen.getByRole("button", { name: "Save" }));

    expect(props.setEditingMessageText).toHaveBeenCalled();
    expect(props.handleSaveEditedMessage).toHaveBeenCalledWith(100);
  });

  it("cancels edit mode", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble(
      {},
      {
        editingMessageId: 100,
        editingMessageText: "Edited text",
      }
    );

    await user.click(screen.getByRole("button", { name: "Cancel" }));

    expect(props.handleCancelEditMessage).toHaveBeenCalledTimes(1);
  });

  it("renders attachment and removes own attachment", async () => {
    const user = userEvent.setup();
    const props = renderMessageBubble({
      text: "Message with file",
      attachment_url: "https://example.com/file.pdf",
      attachment_name: "file.pdf",
      attachment_content_type: "application/pdf",
      attachment_size: 2048,
      attachment_is_image: false,
    });

    expect(screen.getByText("file.pdf")).toBeTruthy();
    expect(screen.getByText("2.0 KB")).toBeTruthy();

    await openMessageActions(user);
    await user.click(screen.getByRole("button", { name: "Delete file" }));

    expect(props.handleRemoveMessageAttachment).toHaveBeenCalledWith(100);
  });

  it("renders image attachment", () => {
    renderMessageBubble({
      text: "",
      attachment_url: "https://example.com/image.png",
      attachment_name: "image.png",
      attachment_content_type: "image/png",
      attachment_size: 4096,
      attachment_is_image: true,
    });

    const link = screen.getByRole("link");
    const image = within(link).getByRole("img");

    expect(link).toHaveAttribute("href", "https://example.com/image.png");
    expect(image).toHaveAttribute("alt", "image.png");
  });
});
