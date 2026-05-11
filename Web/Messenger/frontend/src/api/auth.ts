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

export type RegisterResponse = {
  user: User;
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

export async function register(
  username: string,
  email: string,
  password: string,
  passwordConfirm: string
): Promise<RegisterResponse> {
  const response = await fetch(`${API_BASE_URL}/auth/register/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      username,
      email,
      password,
      password_confirm: passwordConfirm,
    }),
  });

  if (!response.ok) {
    const errorData = await response.json().catch(() => null);

    if (errorData) {
      const fieldLabels: Record<string, string> = {
        username: "Username",
        email: "Email",
        password: "Password",
        password_confirm: "Confirm password",
        non_field_errors: "Error",
      };

      const errorMessages = Object.entries(errorData)
        .map(([field, value]) => {
          const label = fieldLabels[field] ?? field;

          if (Array.isArray(value)) {
            return `${label}: ${value.join(", ")}`;
          }

          return `${label}: ${String(value)}`;
        })
        .join("\n");

      throw new Error(errorMessages);
    }

    throw new Error("Failed to create account.");
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

export async function logout(
  accessToken: string,
  refreshToken: string
): Promise<void> {
  const response = await fetch(`${API_BASE_URL}/auth/logout/`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${accessToken}`,
    },
    body: JSON.stringify({ refresh: refreshToken }),
  });

  if (!response.ok) {
    throw new Error("Failed to logout.");
  }
}