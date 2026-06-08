import type { User } from "./auth";
import { apiRequest } from "./client";

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
  is_marked_unread: boolean;
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

export type DeleteMessageMode = "for_me" | "for_everyone";

export type DeleteMessageForMeResponse = {
  detail: string;
  message_id: number;
  conversation_id: number;
  mode: "for_me";
};

export type DeleteMessageForEveryoneResponse = {
  detail: string;
  message: Message;
  mode: "for_everyone";
};

export type DeleteMessageResponse =
  | DeleteMessageForMeResponse
  | DeleteMessageForEveryoneResponse;

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

export type MarkConversationAsUnreadResponse = {
  detail: string;
  conversation: Conversation;
};

export type ClearConversationHistoryResponse = {
  detail: string;
  conversation: Conversation;
};

export async function getConversations(
  accessToken: string
): Promise<Conversation[]> {
  return apiRequest<Conversation[]>("/conversations/", {
    accessToken,
    errorMessage: "Failed to load conversations.",
  });
}

export async function getConversationMessages(
  accessToken: string,
  conversationId: number
): Promise<Message[]> {
  return apiRequest<Message[]>(
    `/conversations/${conversationId}/messages/`,
    {
      accessToken,
      errorMessage: "Failed to load messages.",
    }
  );
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

  return apiRequest<MessagesPage>(
    `/conversations/${conversationId}/messages/?${searchParams.toString()}`,
    {
      accessToken,
      errorMessage: "Failed to load messages.",
    }
  );
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

  return apiRequest<Message>(`/conversations/${conversationId}/messages/`, {
    accessToken,
    method: "POST",
    body: formData,
    errorMessage: "Failed to send message.",
  });
}

export async function createConversation(
  accessToken: string,
  userId: number
): Promise<Conversation> {
  return apiRequest<Conversation>("/conversations/", {
    accessToken,
    method: "POST",
    json: { user_id: userId },
    errorMessage: "Failed to create conversation.",
  });
}

export async function deleteConversation(
  accessToken: string,
  conversationId: number,
  mode: DeleteConversationMode
): Promise<DeleteConversationResponse> {
  const searchParams = new URLSearchParams({ mode });

  return apiRequest<DeleteConversationResponse>(
    `/conversations/${conversationId}/?${searchParams.toString()}`,
    {
      accessToken,
      method: "DELETE",
      errorMessage: "Failed to delete conversation.",
    }
  );
}

export async function muteConversation(
  accessToken: string,
  conversationId: number,
  isMuted: boolean
): Promise<MuteConversationResponse> {
  return apiRequest<MuteConversationResponse>(
    `/conversations/${conversationId}/mute/`,
    {
      accessToken,
      method: "POST",
      json: { is_muted: isMuted },
      errorMessage: "Failed to update conversation mute status.",
    }
  );
}

export async function pinConversation(
  accessToken: string,
  conversationId: number,
  isPinned: boolean
): Promise<PinConversationResponse> {
  return apiRequest<PinConversationResponse>(
    `/conversations/${conversationId}/pin/`,
    {
      accessToken,
      method: "POST",
      json: { is_pinned: isPinned },
      errorMessage: "Failed to update conversation pin status.",
    }
  );
}

export async function editMessage(
  accessToken: string,
  messageId: number,
  text: string
): Promise<Message> {
  return apiRequest<Message>(`/messages/${messageId}/`, {
    accessToken,
    method: "PATCH",
    json: { text },
    errorMessage: "Failed to edit message.",
  });
}

export async function deleteMessage(
  accessToken: string,
  messageId: number,
  mode: DeleteMessageMode
): Promise<DeleteMessageResponse> {
  const searchParams = new URLSearchParams({ mode });

  return apiRequest<DeleteMessageResponse>(
    `/messages/${messageId}/?${searchParams.toString()}`,
    {
      accessToken,
      method: "DELETE",
      errorMessage: "Failed to delete message.",
    }
  );
}

export async function removeMessageAttachment(
  accessToken: string,
  messageId: number
): Promise<Message> {
  return apiRequest<Message>(`/messages/${messageId}/`, {
    accessToken,
    method: "PATCH",
    json: { remove_attachment: true },
    errorMessage: "Failed to delete attachment.",
  });
}

export async function toggleMessageReaction(
  accessToken: string,
  messageId: number,
  emoji: string
): Promise<ToggleMessageReactionResponse> {
  return apiRequest<ToggleMessageReactionResponse>(
    `/messages/${messageId}/reactions/`,
    {
      accessToken,
      method: "POST",
      json: { emoji },
      errorMessage: "Failed to update reaction.",
    }
  );
}

export async function forwardMessage(
  accessToken: string,
  messageId: number,
  conversationId: number
): Promise<Message> {
  return apiRequest<Message>(`/messages/${messageId}/forward/`, {
    accessToken,
    method: "POST",
    json: { conversation_id: conversationId },
    errorMessage: "Failed to forward message.",
  });
}

export async function markConversationAsRead(
  accessToken: string,
  conversationId: number
): Promise<MarkConversationAsReadResponse> {
  return apiRequest<MarkConversationAsReadResponse>(
    `/conversations/${conversationId}/mark-read/`,
    {
      accessToken,
      method: "POST",
      errorMessage: "Failed to mark conversation as read.",
    }
  );
}

export async function markConversationAsUnread(
  accessToken: string,
  conversationId: number
): Promise<MarkConversationAsUnreadResponse> {
  return apiRequest<MarkConversationAsUnreadResponse>(
    `/conversations/${conversationId}/mark-unread/`,
    {
      accessToken,
      method: "POST",
      errorMessage: "Failed to mark conversation as unread.",
    }
  );
}

export async function clearConversationHistory(
  accessToken: string,
  conversationId: number
): Promise<ClearConversationHistoryResponse> {
  return apiRequest<ClearConversationHistoryResponse>(
    `/conversations/${conversationId}/clear-history/`,
    {
      accessToken,
      method: "POST",
      errorMessage: "Failed to clear chat history.",
    }
  );
}
