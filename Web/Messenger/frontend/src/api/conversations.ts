import { API_BASE_URL } from "./config";
import type { User } from "./auth";

export type Message = {
  id: number;
  conversation: number;
  sender: User;
  text: string;
  created_at: string;
  updated_at: string;
  edited_at: string | null;
  is_deleted: boolean;
  deleted_at: string | null;
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
  created_at: string;
  updated_at: string;
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