import type { User } from "./auth";
import { apiRequest } from "./client";

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

async function parseApiError(response: Response) {
  const errorData = await response.json().catch(() => null);

  return new Error(getApiErrorMessage(errorData));
}

export async function searchUsers(
  accessToken: string,
  query: string
): Promise<User[]> {
  const params = new URLSearchParams();

  if (query.trim()) {
    params.set("q", query.trim());
  }

  return apiRequest<User[]>(`/users/search/?${params.toString()}`, {
    accessToken,
    errorMessage: "Failed to search users.",
  });
}

export async function getBlockedUsers(accessToken: string): Promise<User[]> {
  return apiRequest<User[]>("/users/blocked/", {
    accessToken,
    errorMessage: "Failed to load blocked users.",
  });
}

async function updateUserBlockStatus(
  accessToken: string,
  userId: number,
  action: "block" | "unblock"
): Promise<BlockUserResponse> {
  return apiRequest<BlockUserResponse>(`/users/${userId}/${action}/`, {
    accessToken,
    method: "POST",
    errorMessage: "Request failed.",
    parseError: parseApiError,
  });
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
