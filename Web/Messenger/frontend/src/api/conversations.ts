import { API_BASE_URL } from "./config";
import type { User } from "./auth";

export type MessageReply = {
  id: number;
  sender: User;
  text: string;
  attachment_name: string;
  attachment_content_type: string;
  attachment_is_image: boolean;
  is_deleted: boolean;
  created_at: string;
};

export type MessageForwarded = {
  id: number;
  sender: User;
  is_deleted: boolean;
  created_at: string;
};

export type MessageReaction = {
  emoji: string;
  count: number;
  reacted_by_me: boolean;
  users: User[];
};

export type Message = {
  id: number;
  conversation: number;
  sender: User;

  reply_to_message: MessageReply | null;
  forwarded_from_message: MessageForwarded | null;

  text: string;

  attachment_url: string | null;
  attachment_name: string;
  attachment_content_type: string;
  attachment_size: number | null;
  attachment_is_image: boolean;

  reactions: MessageReaction[];

  created_at: string;
  updated_at: string;
  edited_at: string | null;
  is_deleted: boolean;
  deleted_at: string | null;
  is_delivered: boolean;
  delivered_at: string | null;
  is_read: boolean;
};

export type ConversationParticipant = {
  id: number;
  user: User;
  joined_at: string;
};

export type Conversation = {
  id: number;
  participants: ConversationParticipant[];
  last_message: Message | null;
  unread_count: number;
  is_muted: boolean;
  is_pinned: boolean;
  created_at: string;
  updated_at: string;
};

export type MessagesPage = {
  results: Message[];
  has_more: boolean;
  next_before: number | null;
};

export type ToggleMessageReactionResponse = {
  action: "added" | "removed";
  message: Message;
};

export type DeleteConversationMode = "for_me" | "for_everyone";

export type DeleteConversationResponse = {
  detail: string;
  conversation_id: number;
  mode: DeleteConversationMode;
};

export type MuteConversationResponse = {
  detail: string;
  conversation: Conversation;
};

export type PinConversationResponse = {
  detail: string;
  conversation: Conversation;
};

export type MarkConversationAsReadResponse = {
  updated_count: number;
};

export async function getConversations(
  accessToken: string
): Promise<Conversation[]> {
  const response = await fetch(`${API_BASE_URL}/conversations/`, {
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  });

  if (!response.ok) {
    throw new Error("Failed to load conversations.");
  }

  return response.json();
}

export async function getConversationMessages(
  accessToken: string,
  conversationId: number
): Promise<Message[]> {
  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/messages/`,
    {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    }
  );

  if (!response.ok) {
    throw new Error("Failed to load messages.");
  }

  return response.json();
}

export async function getConversationMessagesPage(
  accessToken: string,
  conversationId: number,
  beforeMessageId?: number,
  limit = 20,
  searchQuery = ""
): Promise<MessagesPage> {
  const searchParams = new URLSearchParams({
    limit: String(limit),
  });

  if (beforeMessageId) {
    searchParams.set("before", String(beforeMessageId));
  }

  if (searchQuery.trim()) {
    searchParams.set("search", searchQuery.trim());
  }

  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/messages/?${searchParams.toString()}`,
    {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    }
  );

  if (!response.ok) {
    throw new Error("Failed to load messages.");
  }

  return response.json();
}

export async function createMessageWithAttachment(
  accessToken: string,
  conversationId: number,
  text: string,
  attachment: File | null,
  replyToMessageId: number | null = null
): Promise<Message> {
  const formData = new FormData();

  if (text.trim()) {
    formData.append("text", text.trim());
  }

  if (attachment) {
    formData.append("attachment", attachment);
  }

  if (replyToMessageId) {
    formData.append("reply_to", String(replyToMessageId));
  }

  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/messages/`,
    {
      method: "POST",
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
      body: formData,
    }
  );

  if (!response.ok) {
    throw new Error("Failed to send message.");
  }

  return response.json();
}

export async function createConversation(
  accessToken: string,
  userId: number
): Promise<Conversation> {
  const response = await fetch(`${API_BASE_URL}/conversations/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${accessToken}`,
    },
    body: JSON.stringify({ user_id: userId }),
  });

  if (!response.ok) {
    throw new Error("Failed to create conversation.");
  }

  return response.json();
}

export async function deleteConversation(
  accessToken: string,
  conversationId: number,
  mode: DeleteConversationMode
): Promise<DeleteConversationResponse> {
  const searchParams = new URLSearchParams({ mode });

  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/?${searchParams.toString()}`,
    {
      method: "DELETE",
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    }
  );

  if (!response.ok) {
    throw new Error("Failed to delete conversation.");
  }

  return response.json();
}

export async function muteConversation(
  accessToken: string,
  conversationId: number,
  isMuted: boolean
): Promise<MuteConversationResponse> {
  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/mute/`,
    {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ is_muted: isMuted }),
    }
  );

  if (!response.ok) {
    throw new Error("Failed to update conversation mute status.");
  }

  return response.json();
}

export async function pinConversation(
  accessToken: string,
  conversationId: number,
  isPinned: boolean
): Promise<PinConversationResponse> {
  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/pin/`,
    {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ is_pinned: isPinned }),
    }
  );

  if (!response.ok) {
    throw new Error("Failed to update conversation pin status.");
  }

  return response.json();
}

export async function editMessage(
  accessToken: string,
  messageId: number,
  text: string
): Promise<Message> {
  const response = await fetch(`${API_BASE_URL}/messages/${messageId}/`, {
    method: "PATCH",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${accessToken}`,
    },
    body: JSON.stringify({ text }),
  });

  if (!response.ok) {
    throw new Error("Failed to edit message.");
  }

  return response.json();
}

export async function deleteMessage(
  accessToken: string,
  messageId: number
): Promise<Message> {
  const response = await fetch(`${API_BASE_URL}/messages/${messageId}/`, {
    method: "DELETE",
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  });

  if (!response.ok) {
    throw new Error("Failed to delete message.");
  }

  return response.json();
}

export async function removeMessageAttachment(
  accessToken: string,
  messageId: number
): Promise<Message> {
  const response = await fetch(`${API_BASE_URL}/messages/${messageId}/`, {
    method: "PATCH",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${accessToken}`,
    },
    body: JSON.stringify({ remove_attachment: true }),
  });

  if (!response.ok) {
    throw new Error("Failed to delete attachment.");
  }

  return response.json();
}

export async function toggleMessageReaction(
  accessToken: string,
  messageId: number,
  emoji: string
): Promise<ToggleMessageReactionResponse> {
  const response = await fetch(
    `${API_BASE_URL}/messages/${messageId}/reactions/`,
    {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({ emoji }),
    }
  );

  if (!response.ok) {
    throw new Error("Failed to update reaction.");
  }

  return response.json();
}

export async function forwardMessage(
  accessToken: string,
  messageId: number,
  conversationId: number
): Promise<Message> {
  const response = await fetch(`${API_BASE_URL}/messages/${messageId}/forward/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${accessToken}`,
    },
    body: JSON.stringify({ conversation_id: conversationId }),
  });

  if (!response.ok) {
    throw new Error("Failed to forward message.");
  }

  return response.json();
}

export async function markConversationAsRead(
  accessToken: string,
  conversationId: number
): Promise<MarkConversationAsReadResponse> {
  const response = await fetch(
    `${API_BASE_URL}/conversations/${conversationId}/mark-read/`,
    {
      method: "POST",
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    }
  );

  if (!response.ok) {
    throw new Error("Failed to mark conversation as read.");
  }

  return response.json();
}