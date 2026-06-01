import { API_BASE_URL } from "./config";
import type { User } from "./auth";

export async function searchUsers(
  accessToken: string,
  query: string
): Promise<User[]> {
  const params = new URLSearchParams();

  if (query.trim()) {
    params.set("q", query.trim());
  }

  const response = await fetch(`${API_BASE_URL}/users/search/?${params}`, {
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  });

  if (!response.ok) {
    throw new Error("Failed to search users.");
  }

  return response.json();
}

export type BlockUserResponse = {
  detail: string;
  user: User;
};

function getApiErrorMessage(errorData: unknown) {
  if (!errorData || typeof errorData !== "object") {
    return "Request failed.";
  }

  const detail = (errorData as { detail?: unknown }).detail;

  if (detail) {
    return String(detail);
  }

  return "Request failed.";
}

async function updateUserBlockStatus(
  accessToken: string,
  userId: number,
  action: "block" | "unblock"
): Promise<BlockUserResponse> {
  const response = await fetch(`${API_BASE_URL}/users/${userId}/${action}/`, {
    method: "POST",
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  });

  if (!response.ok) {
    const errorData = await response.json().catch(() => null);
    throw new Error(getApiErrorMessage(errorData));
  }

  return response.json();
}

export function blockUser(
  accessToken: string,
  userId: number
): Promise<BlockUserResponse> {
  return updateUserBlockStatus(accessToken, userId, "block");
}

export function unblockUser(
  accessToken: string,
  userId: number
): Promise<BlockUserResponse> {
  return updateUserBlockStatus(accessToken, userId, "unblock");
}