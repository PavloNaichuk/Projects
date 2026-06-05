import { fireEvent, render, screen } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";
import ChatWindow from "./ChatWindow";

vi.mock("./MessageBubble", () => ({
  default: ({
    message,
    searchQuery,
    handleStartReplyMessage,
    handleStartForwardMessage,
    handleStartEditMessage,
    handleDeleteMessage,
    handleToggleMessageReaction,
  }: {
    message: Message;
    searchQuery?: string;
    handleStartReplyMessage: (message: Message) => void;
    handleStartForwardMessage: (message: Message) => void;
    handleStartEditMessage: (message: Message) => void;
    handleDeleteMessage: (messageId: number, mode: string) => void;
    handleToggleMessageReaction: (messageId: number, emoji: string) => void;
  }) => (
    <div data-testid={`message-${message.id}`}>
      <span>{message.text}</span>
      <span data-testid={`message-search-${message.id}`}>{searchQuery}</span>
      <button type="button" onClick={() => handleStartReplyMessage(message)}>
        Mock reply
      </button>
      <button type="button" onClick={() => handleStartForwardMessage(message)}>
        Mock forward
      </button>
      <button type="button" onClick={() => handleStartEditMessage(message)}>
        Mock edit
      </button>
      <button
        type="button"
        onClick={() => handleDeleteMessage(message.id, "for_me")}
      >
        Mock delete for me
      </button>
      <button
        type="button"
        onClick={() => handleToggleMessageReaction(message.id, "👍")}
      >
        Mock reaction
      </button>
    </div>
  ),
}));

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
  last_seen_at: "2026-06-05T10:00:00Z",
  is_blocked_by_me: false,
  has_blocked_me: false,
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
    last_message: null,
    unread_count: 0,
    is_muted: false,
    is_pinned: false,
    is_marked_unread: false,
    created_at: "2026-06-05T10:00:00Z",
    updated_at: "2026-06-05T10:00:00Z",
    ...overrides,
  } as unknown as Conversation;
}

function createMessage(overrides = {}) {
  return {
    id: 100,
    conversation: 10,
    sender: currentUser,
    text: "Hello from chat window",
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

function createFile(name = "avatar.png") {
  return new File(["file content"], name, {
    type: "image/png",
  });
}

function renderChatWindow(overrides = {}) {
  const props = {
    currentUser,
    selectedConversation: createConversation(),
    selectedConversationName: "User 44",
    selectedConversationUser: otherUser,
    selectedConversationUserIsOnline: false,
    messages: [createMessage()],
    isMessagesLoading: false,
    isOlderMessagesLoading: false,
    hasMoreMessages: false,
    messageSearchQuery: "",
    setMessageSearchQuery: vi.fn(),
    isMessageSearchActive: false,
    isSearchingMessages: false,
    typingUser: null,
    messageError: "",
    newMessage: "",
    selectedAttachment: null,
    replyToMessage: null,
    isSending: false,
    editingMessageId: null,
    editingMessageText: "",
    setEditingMessageText: vi.fn(),
    isEditingMessage: false,
    isDeletingMessageId: null,
    isDeletingConversationId: null,
    isBlockingUserId: null,
    messagesContainerRef: { current: null },
    messagesEndRef: { current: null },
    handleMessagesScroll: vi.fn(),
    handleSearchMessages: vi.fn().mockResolvedValue(undefined),
    handleClearMessageSearch: vi.fn().mockResolvedValue(undefined),
    handleNewMessageChange: vi.fn(),
    handleAttachmentChange: vi.fn(),
    handleRemoveAttachment: vi.fn(),
    handleStartReplyMessage: vi.fn(),
    handleCancelReplyMessage: vi.fn(),
    handleStartForwardMessage: vi.fn(),
    handleMessageKeyDown: vi.fn(),
    handleSendMessage: vi.fn().mockResolvedValue(undefined),
    handleStartEditMessage: vi.fn(),
    handleCancelEditMessage: vi.fn(),
    handleSaveEditedMessage: vi.fn().mockResolvedValue(undefined),
    handleDeleteMessage: vi.fn().mockResolvedValue(undefined),
    handleRemoveMessageAttachment: vi.fn().mockResolvedValue(undefined),
    handleToggleMessageReaction: vi.fn().mockResolvedValue(undefined),
    handleMuteConversation: vi.fn().mockResolvedValue(undefined),
    handlePinConversation: vi.fn().mockResolvedValue(undefined),
    handleOpenContactNicknameModal: vi.fn(),
    handleDeleteConversation: vi.fn().mockResolvedValue(undefined),
    handleBlockUser: vi.fn().mockResolvedValue(undefined),
    handleUnblockUser: vi.fn().mockResolvedValue(undefined),
    ...overrides,
  };

  const result = render(<ChatWindow {...props} />);

  return {
    props,
    ...result,
  };
}

describe("ChatWindow", () => {
  beforeEach(() => {
    vi.clearAllMocks();

    document.execCommand = vi.fn(() => true);
    window.HTMLElement.prototype.scrollIntoView = vi.fn();

    Object.defineProperty(window, "requestAnimationFrame", {
      writable: true,
      value: (callback: FrameRequestCallback) => {
        callback(0);
        return 0;
      },
    });
  });

  it("renders empty state when no conversation is selected", () => {
    renderChatWindow({
      selectedConversation: null,
      selectedConversationName: "",
      selectedConversationUser: null,
      messages: [],
    });

    expect(screen.getByText("Select a conversation.")).toBeTruthy();
    expect(screen.getByPlaceholderText("Search messages...")).toHaveAttribute(
      "disabled"
    );
    expect(screen.getByPlaceholderText("Type a message...")).toHaveAttribute(
      "disabled"
    );
    expect(screen.getByRole("button", { name: "Send" })).toHaveAttribute(
      "disabled"
    );
  });

  it("renders selected conversation header and messages", () => {
    renderChatWindow();

    expect(screen.getByRole("heading", { name: "User 44" })).toBeTruthy();
    expect(screen.getByText("Last seen", { exact: false })).toBeTruthy();
    expect(screen.getByTestId("message-100")).toBeTruthy();
    expect(screen.getByText("Hello from chat window")).toBeTruthy();
  });

  it("shows online status", () => {
    renderChatWindow({
      selectedConversationUserIsOnline: true,
    });

    expect(screen.getByText("Online")).toBeTruthy();
  });

  it("shows loading and empty message states", () => {
    const { rerender, props } = renderChatWindow({
      isMessagesLoading: true,
      messages: [],
    });

    expect(screen.getByText("Loading messages...")).toBeTruthy();

    rerender(
      <ChatWindow
        {...props}
        isMessagesLoading={false}
        messages={[]}
        isMessageSearchActive={false}
      />
    );

    expect(screen.getByText("No messages yet.")).toBeTruthy();

    rerender(
      <ChatWindow
        {...props}
        isMessagesLoading={false}
        messages={[]}
        isMessageSearchActive={true}
      />
    );

    expect(screen.getByText("No messages found.")).toBeTruthy();
  });

  it("shows older messages loader and search mode label", () => {
    const { rerender, props } = renderChatWindow({
      hasMoreMessages: true,
      isOlderMessagesLoading: true,
      messages: [createMessage()],
    });

    expect(screen.getByText("Loading older messages...")).toBeTruthy();

    rerender(
      <ChatWindow
        {...props}
        isMessageSearchActive={true}
        messageSearchQuery="hello"
        messages={[createMessage()]}
      />
    );

    expect(screen.getByText("Search results")).toBeTruthy();
    expect(screen.getByTestId("message-search-100").textContent).toBe("hello");
  });

  it("submits message search and clears search", async () => {
    const user = userEvent.setup();
    const { props } = renderChatWindow({
      messageSearchQuery: "hello",
    });

    await user.type(screen.getByPlaceholderText("Search messages..."), "!");
    await user.click(screen.getByRole("button", { name: "Search" }));

    expect(props.setMessageSearchQuery).toHaveBeenCalled();
    expect(props.handleSearchMessages).toHaveBeenCalledTimes(1);

    await user.click(screen.getByRole("button", { name: "Clear" }));

    expect(props.handleClearMessageSearch).toHaveBeenCalledTimes(1);
  });

  it("updates message input and sends message", async () => {
    const user = userEvent.setup();
    const { props } = renderChatWindow({
      newMessage: "Hello",
    });

    await user.type(screen.getByPlaceholderText("Type a message..."), "!");
    await user.click(screen.getByRole("button", { name: "Send" }));

    expect(props.handleNewMessageChange).toHaveBeenCalled();
    expect(props.handleSendMessage).toHaveBeenCalledTimes(1);
  });

  it("calls keydown handler from message input", () => {
    const { props } = renderChatWindow();

    fireEvent.keyDown(screen.getByPlaceholderText("Type a message..."), {
      key: "Enter",
      code: "Enter",
    });

    expect(props.handleMessageKeyDown).toHaveBeenCalledTimes(1);
  });

  it("shows reply preview and cancels reply", async () => {
    const user = userEvent.setup();
    const replyMessage = createMessage({
      id: 200,
      sender: otherUser,
      text: "Original message",
    });

    const { props } = renderChatWindow({
      replyToMessage: replyMessage,
    });

    expect(screen.getByText("Replying to User 44")).toBeTruthy();
    expect(screen.getByText("Original message")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "×" }));

    expect(props.handleCancelReplyMessage).toHaveBeenCalledTimes(1);
  });

  it("shows selected attachment and removes it", async () => {
    const user = userEvent.setup();
    const file = createFile("photo.png");
    const { props } = renderChatWindow({
      selectedAttachment: file,
    });

    expect(screen.getByText("photo.png")).toBeTruthy();

    await user.click(screen.getByRole("button", { name: "Remove" }));

    expect(props.handleRemoveAttachment).toHaveBeenCalledTimes(1);
  });

  it("calls attachment change handler", () => {
    const file = createFile("document.png");
    const { props, container } = renderChatWindow();

    const input = container.querySelector('input[type="file"]');

    expect(input).not.toBeNull();

    fireEvent.change(input as HTMLInputElement, {
      target: {
        files: [file],
      },
    });

    expect(props.handleAttachmentChange).toHaveBeenCalledTimes(1);
  });

  it("shows typing indicator and message error", () => {
    renderChatWindow({
      typingUser: otherUser,
      messageError: "Failed to send message.",
    });

    expect(screen.getByText("User 44 is typing...")).toBeTruthy();
    expect(screen.getByText("Failed to send message.")).toBeTruthy();
  });

  it("disables sending when current user blocked selected user", () => {
    renderChatWindow({
      selectedConversationUser: {
        ...otherUser,
        is_blocked_by_me: true,
      },
    });

    expect(screen.getByText("You blocked this user")).toBeTruthy();
    expect(
      screen.getByText("You blocked this user. Unblock them to send messages.")
    ).toBeTruthy();
    expect(screen.getByPlaceholderText("Messaging is blocked")).toHaveAttribute(
      "disabled"
    );
    expect(screen.getByRole("button", { name: "Send" })).toHaveAttribute(
      "disabled"
    );
  });

  it("disables sending when selected user blocked current user", () => {
    renderChatWindow({
      selectedConversationUser: {
        ...otherUser,
        has_blocked_me: true,
      },
    });

    expect(screen.getByText("This user blocked you")).toBeTruthy();
    expect(screen.getByText("You cannot send messages to this user.")).toBeTruthy();
    expect(screen.getByPlaceholderText("Messaging is blocked")).toHaveAttribute(
      "disabled"
    );
  });

  it("passes message actions to MessageBubble", async () => {
    const user = userEvent.setup();
    const { props } = renderChatWindow();

    await user.click(screen.getByRole("button", { name: "Mock reply" }));
    await user.click(screen.getByRole("button", { name: "Mock forward" }));
    await user.click(screen.getByRole("button", { name: "Mock edit" }));
    await user.click(screen.getByRole("button", { name: "Mock delete for me" }));
    await user.click(screen.getByRole("button", { name: "Mock reaction" }));

    expect(props.handleStartReplyMessage).toHaveBeenCalledWith(props.messages[0]);
    expect(props.handleStartForwardMessage).toHaveBeenCalledWith(
      props.messages[0]
    );
    expect(props.handleStartEditMessage).toHaveBeenCalledWith(props.messages[0]);
    expect(props.handleDeleteMessage).toHaveBeenCalledWith(100, "for_me");
    expect(props.handleToggleMessageReaction).toHaveBeenCalledWith(100, "👍");
  });

  it("opens text context menu and inserts emoji", async () => {
    const user = userEvent.setup();
    const { props } = renderChatWindow({
      newMessage: "Hello ",
    });

    const textarea = screen.getByPlaceholderText("Type a message...");

    fireEvent.contextMenu(textarea, {
      clientX: 100,
      clientY: 100,
    });

    expect(screen.getByText("Emoji")).toBeTruthy();

    await user.click(screen.getByText("Emoji"));
    await user.click(screen.getByRole("button", { name: "😀" }));

    expect(props.handleNewMessageChange).toHaveBeenCalledWith("😀Hello ");
  });

  it("opens text context menu and selects all text", async () => {
    const user = userEvent.setup();

    renderChatWindow({
      newMessage: "Hello",
    });

    const textarea = screen.getByPlaceholderText(
      "Type a message..."
    ) as HTMLTextAreaElement;

    fireEvent.contextMenu(textarea, {
      clientX: 100,
      clientY: 100,
    });

    await user.click(screen.getByText("Select all"));

    expect(textarea.selectionStart).toBe(0);
    expect(textarea.selectionEnd).toBe(textarea.value.length);
  });
});
