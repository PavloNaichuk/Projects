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