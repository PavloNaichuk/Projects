import { API_BASE_URL } from "./config";

export type User = {
  id: number;
  username: string;
  email: string;
};

export type LoginResponse = {
  access: string;
  refresh: string;
};

export type RefreshTokenResponse = {
  access: string;
};

export async function login(
  username: string,
  password: string
): Promise<LoginResponse> {
  const response = await fetch(`${API_BASE_URL}/auth/token/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ username, password }),
  });

  if (!response.ok) {
    throw new Error("Invalid username or password.");
  }

  return response.json();
}

export async function refreshAccessToken(
  refreshToken: string
): Promise<RefreshTokenResponse> {
  const response = await fetch(`${API_BASE_URL}/auth/token/refresh/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ refresh: refreshToken }),
  });

  if (!response.ok) {
    throw new Error("Failed to refresh token.");
  }

  return response.json();
}

export async function getCurrentUser(accessToken: string): Promise<User> {
  const response = await fetch(`${API_BASE_URL}/auth/me/`, {
    headers: {
      Authorization: `Bearer ${accessToken}`,
    },
  });

  if (!response.ok) {
    throw new Error("Failed to load current user.");
  }

  return response.json();
}