import { apiRequest } from "./client";

export type User = {
  id: number;
  username: string;
  display_name: string;
  email: string;
  avatar_url: string | null;
  last_seen_at: string | null;
  email_verified_at: string | null;
  is_email_verified: boolean;
  is_blocked_by_me: boolean;
  has_blocked_me: boolean;
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

export type VerifyEmailResponse = {
  detail: string;
  user: User;
};

export type ResendEmailVerificationResponse = {
  detail: string;
};

export type UpdateCurrentUserProfileData = {
  username: string;
  email: string;
};

export type UpdateContactNicknameResponse = {
  detail: string;
  user: User;
};

function getApiErrorMessage(errorData: unknown) {
  if (!errorData || typeof errorData !== "object") {
    return "Request failed.";
  }

  const fieldLabels: Record<string, string> = {
    username: "Username",
    email: "Email",
    nickname: "Nickname",
    password: "Password",
    password_confirm: "Confirm password",
    token: "Token",
    non_field_errors: "Error",
    detail: "Error",
  };

  return Object.entries(errorData)
    .map(([field, value]) => {
      const label = fieldLabels[field] ?? field;

      if (Array.isArray(value)) {
        return `${label}: ${value.join(", ")}`;
      }

      return `${label}: ${String(value)}`;
    })
    .join("\n");
}

async function parseApiError(response: Response, fallbackMessage: string) {
  const errorData = await response.json().catch(() => null);

  if (errorData) {
    return new Error(getApiErrorMessage(errorData));
  }

  return new Error(fallbackMessage);
}

export async function login(
  username: string,
  password: string
): Promise<LoginResponse> {
  return apiRequest<LoginResponse>("/auth/token/", {
    method: "POST",
    json: { username, password },
    errorMessage: "Invalid username or password.",
  });
}

export async function register(
  username: string,
  email: string,
  password: string,
  passwordConfirm: string
): Promise<RegisterResponse> {
  return apiRequest<RegisterResponse>("/auth/register/", {
    method: "POST",
    json: {
      username,
      email,
      password,
      password_confirm: passwordConfirm,
    },
    errorMessage: "Failed to create account.",
    parseError: (response) => parseApiError(response, "Failed to create account."),
  });
}

export async function verifyEmail(token: string): Promise<VerifyEmailResponse> {
  return apiRequest<VerifyEmailResponse>("/auth/email/verify/", {
    method: "POST",
    json: { token },
    errorMessage: "Failed to verify email.",
    parseError: (response) => parseApiError(response, "Failed to verify email."),
  });
}

export async function resendEmailVerification(
  accessToken: string
): Promise<ResendEmailVerificationResponse> {
  return apiRequest<ResendEmailVerificationResponse>("/auth/email/resend/", {
    accessToken,
    method: "POST",
    errorMessage: "Failed to resend verification email.",
    parseError: (response) =>
      parseApiError(response, "Failed to resend verification email."),
  });
}

export async function refreshAccessToken(
  refreshToken: string
): Promise<RefreshTokenResponse> {
  return apiRequest<RefreshTokenResponse>("/auth/token/refresh/", {
    method: "POST",
    json: { refresh: refreshToken },
    errorMessage: "Failed to refresh token.",
  });
}

export async function getCurrentUser(accessToken: string): Promise<User> {
  return apiRequest<User>("/auth/me/", {
    accessToken,
    errorMessage: "Failed to load current user.",
  });
}

export async function updateCurrentUserProfile(
  accessToken: string,
  data: UpdateCurrentUserProfileData
): Promise<User> {
  return apiRequest<User>("/auth/me/profile/", {
    accessToken,
    method: "PATCH",
    json: data,
    errorMessage: "Failed to update profile.",
    parseError: (response) => parseApiError(response, "Failed to update profile."),
  });
}

export async function updateContactNickname(
  accessToken: string,
  userId: number,
  nickname: string
): Promise<UpdateContactNicknameResponse> {
  return apiRequest<UpdateContactNicknameResponse>(`/users/${userId}/nickname/`, {
    accessToken,
    method: "PATCH",
    json: { nickname },
    errorMessage: "Failed to update contact nickname.",
    parseError: (response) =>
      parseApiError(response, "Failed to update contact nickname."),
  });
}

export async function updateCurrentUserAvatar(
  accessToken: string,
  avatar: File
): Promise<User> {
  const formData = new FormData();
  formData.append("avatar", avatar);

  return apiRequest<User>("/auth/me/avatar/", {
    accessToken,
    method: "PATCH",
    body: formData,
    errorMessage: "Failed to update avatar.",
  });
}

export async function deleteCurrentUserAvatar(
  accessToken: string
): Promise<User> {
  return apiRequest<User>("/auth/me/avatar/", {
    accessToken,
    method: "DELETE",
    errorMessage: "Failed to delete avatar.",
  });
}

export async function logout(
  accessToken: string,
  refreshToken: string
): Promise<void> {
  return apiRequest<void>("/auth/logout/", {
    accessToken,
    method: "POST",
    json: { refresh: refreshToken },
    errorMessage: "Failed to logout.",
    parseJson: false,
  });
}
