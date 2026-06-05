import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "./auth";
import { API_BASE_URL } from "./config";
import {
  clearConversationHistory,
  createConversation,
  createMessageWithAttachment,
  deleteConversation,
  deleteMessage,
  editMessage,
  forwardMessage,
  getConversationMessages,
  getConversationMessagesPage,
  getConversations,
  markConversationAsRead,
  markConversationAsUnread,
  muteConversation,
  pinConversation,
  removeMessageAttachment,
  toggleMessageReaction,
  type Conversation,
  type Message,
} from "./conversations";

const accessToken = "access-token";

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

const message = {
  id: 100,
  conversation: 10,
  sender: currentUser,
  reply_to_message: null,
  forwarded_from_message: null,
  text: "Hello",
  attachment_url: null,
  attachment_name: "",
  attachment_content_type: "",
  attachment_size: null,
  attachment_is_image: false,
  reactions: [],
  created_at: "2026-06-05T10:00:00Z",
  updated_at: "2026-06-05T10:00:00Z",
  edited_at: null,
  is_deleted: false,
  deleted_at: null,
  is_delivered: false,
  delivered_at: null,
  is_read: false,
} as Message;

const conversation = {
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
  last_message: message,
  unread_count: 1,
  is_muted: false,
  is_pinned: false,
  is_marked_unread: false,
  created_at: "2026-06-05T10:00:00Z",
  updated_at: "2026-06-05T10:00:00Z",
} as Conversation;

const fetchMock = vi.fn();

function createJsonResponse(data: unknown, ok = true) {
  return {
    ok,
    json: vi.fn().mockResolvedValue(data),
  } as unknown as Response;
}

function expectAuthorizationHeader(requestInit: RequestInit | undefined) {
  expect(requestInit?.headers).toEqual(
    expect.objectContaining({
      Authorization: `Bearer ${accessToken}`,
    })
  );
}

describe("conversations api", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    vi.stubGlobal("fetch", fetchMock);
  });

  it("loads conversations", async () => {
    fetchMock.mockResolvedValue(createJsonResponse([conversation]));

    const result = await getConversations(accessToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/conversations/`, {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual([conversation]);
  });

  it("throws when loading conversations fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(getConversations(accessToken)).rejects.toThrow(
      "Failed to load conversations."
    );
  });

  it("loads conversation messages", async () => {
    fetchMock.mockResolvedValue(createJsonResponse([message]));

    const result = await getConversationMessages(accessToken, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/messages/`,
      {
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual([message]);
  });

  it("loads paginated conversation messages with before, limit and search query", async () => {
    const page = {
      results: [message],
      has_more: true,
      next_before: 50,
    };

    fetchMock.mockResolvedValue(createJsonResponse(page));

    const result = await getConversationMessagesPage(
      accessToken,
      conversation.id,
      99,
      30,
      "  hello  "
    );

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/messages/?limit=30&before=99&search=hello`,
      {
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(page);
  });

  it("loads paginated conversation messages with default limit only", async () => {
    const page = {
      results: [],
      has_more: false,
      next_before: null,
    };

    fetchMock.mockResolvedValue(createJsonResponse(page));

    const result = await getConversationMessagesPage(accessToken, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/messages/?limit=20`,
      {
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(page);
  });

  it("creates message with text, attachment and reply id using FormData", async () => {
    const attachment = new File(["file content"], "photo.png", {
      type: "image/png",
    });

    fetchMock.mockResolvedValue(createJsonResponse(message));

    const result = await createMessageWithAttachment(
      accessToken,
      conversation.id,
      "  Hello with file  ",
      attachment,
      55
    );

    expect(fetchMock).toHaveBeenCalledTimes(1);

    const [url, requestInit] = fetchMock.mock.calls[0] as [
      string,
      RequestInit,
    ];

    expect(url).toBe(`${API_BASE_URL}/conversations/${conversation.id}/messages/`);
    expect(requestInit.method).toBe("POST");
    expectAuthorizationHeader(requestInit);
    expect(requestInit.headers).not.toEqual(
      expect.objectContaining({
        "Content-Type": "application/json",
      })
    );

    const body = requestInit.body as FormData;

    expect(body).toBeInstanceOf(FormData);
    expect(body.get("text")).toBe("Hello with file");
    expect(body.get("attachment")).toBe(attachment);
    expect(body.get("reply_to")).toBe("55");
    expect(result).toEqual(message);
  });

  it("creates message without empty text, attachment or reply id", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(message));

    await createMessageWithAttachment(accessToken, conversation.id, "   ", null);

    const requestInit = fetchMock.mock.calls[0][1] as RequestInit;
    const body = requestInit.body as FormData;

    expect(body.has("text")).toBe(false);
    expect(body.has("attachment")).toBe(false);
    expect(body.has("reply_to")).toBe(false);
  });

  it("creates conversation", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(conversation));

    const result = await createConversation(accessToken, otherUser.id);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/conversations/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ user_id: otherUser.id }),
    });
    expect(result).toEqual(conversation);
  });

  it("deletes conversation with mode query parameter", async () => {
    const responseData = {
      detail: "Conversation deleted for you.",
      conversation_id: conversation.id,
      mode: "for_me" as const,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await deleteConversation(accessToken, conversation.id, "for_me");

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/?mode=for_me`,
      {
        method: "DELETE",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("mutes conversation", async () => {
    const responseData = {
      detail: "Conversation muted.",
      conversation: {
        ...conversation,
        is_muted: true,
      },
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await muteConversation(accessToken, conversation.id, true);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/mute/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${accessToken}`,
        },
        body: JSON.stringify({ is_muted: true }),
      }
    );
    expect(result).toEqual(responseData);
  });

  it("pins conversation", async () => {
    const responseData = {
      detail: "Conversation pinned.",
      conversation: {
        ...conversation,
        is_pinned: true,
      },
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await pinConversation(accessToken, conversation.id, true);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/pin/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${accessToken}`,
        },
        body: JSON.stringify({ is_pinned: true }),
      }
    );
    expect(result).toEqual(responseData);
  });

  it("edits message", async () => {
    const updatedMessage = {
      ...message,
      text: "Updated text",
    };

    fetchMock.mockResolvedValue(createJsonResponse(updatedMessage));

    const result = await editMessage(accessToken, message.id, "Updated text");

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/messages/${message.id}/`, {
      method: "PATCH",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ text: "Updated text" }),
    });
    expect(result).toEqual(updatedMessage);
  });

  it("deletes message with mode query parameter", async () => {
    const responseData = {
      detail: "Message deleted for everyone.",
      message: {
        ...message,
        text: "",
        is_deleted: true,
      },
      mode: "for_everyone" as const,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await deleteMessage(accessToken, message.id, "for_everyone");

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/messages/${message.id}/?mode=for_everyone`,
      {
        method: "DELETE",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("removes message attachment", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(message));

    const result = await removeMessageAttachment(accessToken, message.id);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/messages/${message.id}/`, {
      method: "PATCH",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ remove_attachment: true }),
    });
    expect(result).toEqual(message);
  });

  it("toggles message reaction", async () => {
    const responseData = {
      action: "added" as const,
      message,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await toggleMessageReaction(accessToken, message.id, "👍");

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/messages/${message.id}/reactions/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${accessToken}`,
        },
        body: JSON.stringify({ emoji: "👍" }),
      }
    );
    expect(result).toEqual(responseData);
  });

  it("forwards message", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(message));

    const result = await forwardMessage(accessToken, message.id, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/messages/${message.id}/forward/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${accessToken}`,
        },
        body: JSON.stringify({ conversation_id: conversation.id }),
      }
    );
    expect(result).toEqual(message);
  });

  it("marks conversation as read", async () => {
    const responseData = {
      updated_count: 2,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await markConversationAsRead(accessToken, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/mark-read/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("marks conversation as unread", async () => {
    const responseData = {
      detail: "Conversation marked as unread.",
      conversation: {
        ...conversation,
        is_marked_unread: true,
      },
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await markConversationAsUnread(accessToken, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/mark-unread/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("clears conversation history", async () => {
    const responseData = {
      detail: "Chat history cleared for you.",
      conversation,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await clearConversationHistory(accessToken, conversation.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/conversations/${conversation.id}/clear-history/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it.each([
    [getConversationMessages, "Failed to load messages."],
    [createConversation, "Failed to create conversation."],
    [deleteConversation, "Failed to delete conversation."],
    [muteConversation, "Failed to update conversation mute status."],
    [pinConversation, "Failed to update conversation pin status."],
    [editMessage, "Failed to edit message."],
    [deleteMessage, "Failed to delete message."],
    [removeMessageAttachment, "Failed to delete attachment."],
    [toggleMessageReaction, "Failed to update reaction."],
    [forwardMessage, "Failed to forward message."],
    [markConversationAsRead, "Failed to mark conversation as read."],
    [markConversationAsUnread, "Failed to mark conversation as unread."],
    [clearConversationHistory, "Failed to clear chat history."],
  ])("throws expected error when request fails", async (apiFunction, messageText) => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    const call = () => {
      if (apiFunction === getConversationMessages) {
        return getConversationMessages(accessToken, conversation.id);
      }

      if (apiFunction === createConversation) {
        return createConversation(accessToken, otherUser.id);
      }

      if (apiFunction === deleteConversation) {
        return deleteConversation(accessToken, conversation.id, "for_me");
      }

      if (apiFunction === muteConversation) {
        return muteConversation(accessToken, conversation.id, true);
      }

      if (apiFunction === pinConversation) {
        return pinConversation(accessToken, conversation.id, true);
      }

      if (apiFunction === editMessage) {
        return editMessage(accessToken, message.id, "Updated text");
      }

      if (apiFunction === deleteMessage) {
        return deleteMessage(accessToken, message.id, "for_me");
      }

      if (apiFunction === removeMessageAttachment) {
        return removeMessageAttachment(accessToken, message.id);
      }

      if (apiFunction === toggleMessageReaction) {
        return toggleMessageReaction(accessToken, message.id, "👍");
      }

      if (apiFunction === forwardMessage) {
        return forwardMessage(accessToken, message.id, conversation.id);
      }

      if (apiFunction === markConversationAsRead) {
        return markConversationAsRead(accessToken, conversation.id);
      }

      if (apiFunction === markConversationAsUnread) {
        return markConversationAsUnread(accessToken, conversation.id);
      }

      return clearConversationHistory(accessToken, conversation.id);
    };

    await expect(call()).rejects.toThrow(messageText);
  });

  it("throws when sending message with attachment fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(
      createMessageWithAttachment(accessToken, conversation.id, "Hello", null)
    ).rejects.toThrow("Failed to send message.");
  });

  it("throws when paginated messages request fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(
      getConversationMessagesPage(accessToken, conversation.id)
    ).rejects.toThrow("Failed to load messages.");
  });
});
