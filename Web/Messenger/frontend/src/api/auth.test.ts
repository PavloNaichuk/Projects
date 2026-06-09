import { beforeEach, describe, expect, it, vi } from "vitest";

import {
  deleteCurrentUserAvatar,
  getCurrentUser,
  login,
  logout,
  refreshAccessToken,
  register,
  resendEmailVerification,
  updateContactNickname,
  updateCurrentUserAvatar,
  updateCurrentUserProfile,
  verifyEmail,
  type User,
} from "./auth";
import { API_BASE_URL } from "./config";

const accessToken = "access-token";
const refreshToken = "refresh-token";

const user = {
  id: 1,
  username: "pavlo",
  display_name: "Pavlo",
  email: "pavlo@test.ua",
  avatar_url: null,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

const fetchMock = vi.fn();

function createJsonResponse(data: unknown, ok = true) {
  return {
    ok,
    json: vi.fn().mockResolvedValue(data),
  } as unknown as Response;
}

describe("auth api", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    vi.stubGlobal("fetch", fetchMock);
  });

  it("logs user in", async () => {
    const responseData = {
      access: accessToken,
      refresh: refreshToken,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await login("pavlo", "testpassword123");

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/token/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        username: "pavlo",
        password: "testpassword123",
      }),
    });
    expect(result).toEqual(responseData);
  });

  it("throws when login fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(login("pavlo", "wrong-password")).rejects.toThrow(
      "Invalid username or password."
    );
  });

  it("registers user", async () => {
    const responseData = {
      user,
      access: accessToken,
      refresh: refreshToken,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await register(
      "pavlo",
      "pavlo@test.ua",
      "testpassword123",
      "testpassword123"
    );

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/register/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        username: "pavlo",
        email: "pavlo@test.ua",
        password: "testpassword123",
        password_confirm: "testpassword123",
      }),
    });
    expect(result).toEqual(responseData);
  });

  it("formats register validation errors", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          username: ["User with this username already exists."],
          password_confirm: ["Passwords do not match."],
        },
        false
      )
    );

    await expect(
      register("pavlo", "pavlo@test.ua", "testpassword123", "wrong")
    ).rejects.toThrow(
      [
        "Username: User with this username already exists.",
        "Confirm password: Passwords do not match.",
      ].join("\n")
    );
  });

  it("uses fallback register error when error response is not json", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(
      register("pavlo", "pavlo@test.ua", "testpassword123", "testpassword123")
    ).rejects.toThrow("Failed to create account.");
  });


  it("verifies email", async () => {
    const verifiedUser = {
      ...user,
      email_verified_at: "2026-06-09T10:00:00Z",
      is_email_verified: true,
    };
    const responseData = {
      detail: "Email verified successfully.",
      user: verifiedUser,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await verifyEmail("verification-token");

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/auth/email/verify/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          token: "verification-token",
        }),
      }
    );
    expect(result).toEqual(responseData);
  });

  it("formats email verification validation errors", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          token: ["Verification token has expired."],
        },
        false
      )
    );

    await expect(verifyEmail("expired-token")).rejects.toThrow(
      "Token: Verification token has expired."
    );
  });

  it("uses fallback email verification error for non-json response", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(verifyEmail("verification-token")).rejects.toThrow(
      "Failed to verify email."
    );
  });

  it("resends email verification", async () => {
    const responseData = {
      detail: "Verification email sent.",
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await resendEmailVerification(accessToken);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/auth/email/resend/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("formats resend email verification errors", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          detail: "Email is already verified.",
        },
        false
      )
    );

    await expect(resendEmailVerification(accessToken)).rejects.toThrow(
      "Error: Email is already verified."
    );
  });

  it("uses fallback resend verification error for non-json response", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(resendEmailVerification(accessToken)).rejects.toThrow(
      "Failed to resend verification email."
    );
  });

  it("refreshes access token", async () => {
    const responseData = {
      access: "new-access-token",
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await refreshAccessToken(refreshToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/token/refresh/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        refresh: refreshToken,
      }),
    });
    expect(result).toEqual(responseData);
  });

  it("throws when token refresh fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(refreshAccessToken(refreshToken)).rejects.toThrow(
      "Failed to refresh token."
    );
  });

  it("loads current user", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(user));

    const result = await getCurrentUser(accessToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/me/`, {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual(user);
  });

  it("throws when loading current user fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(getCurrentUser(accessToken)).rejects.toThrow(
      "Failed to load current user."
    );
  });

  it("updates current user profile", async () => {
    const updatedUser = {
      ...user,
      username: "newpavlo",
      email: "newpavlo@test.ua",
    };

    fetchMock.mockResolvedValue(createJsonResponse(updatedUser));

    const result = await updateCurrentUserProfile(accessToken, {
      username: "newpavlo",
      email: "newpavlo@test.ua",
    });

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/me/profile/`, {
      method: "PATCH",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({
        username: "newpavlo",
        email: "newpavlo@test.ua",
      }),
    });
    expect(result).toEqual(updatedUser);
  });

  it("formats profile update validation errors", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          email: ["User with this email already exists."],
          non_field_errors: ["Profile update failed."],
        },
        false
      )
    );

    await expect(
      updateCurrentUserProfile(accessToken, {
        username: "pavlo",
        email: "taken@test.ua",
      })
    ).rejects.toThrow(
      "Email: User with this email already exists.\nError: Profile update failed."
    );
  });

  it("uses fallback profile update error when error response is not json", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(
      updateCurrentUserProfile(accessToken, {
        username: "pavlo",
        email: "pavlo@test.ua",
      })
    ).rejects.toThrow("Failed to update profile.");
  });

  it("updates contact nickname", async () => {
    const responseData = {
      detail: "Contact nickname updated.",
      user: {
        ...user,
        display_name: "Friend",
      },
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await updateContactNickname(accessToken, user.id, "Friend");

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/users/${user.id}/nickname/`,
      {
        method: "PATCH",
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${accessToken}`,
        },
        body: JSON.stringify({
          nickname: "Friend",
        }),
      }
    );
    expect(result).toEqual(responseData);
  });

  it("formats contact nickname validation errors", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          nickname: ["Nickname must be 50 characters or less."],
        },
        false
      )
    );

    await expect(
      updateContactNickname(accessToken, user.id, "Very long nickname")
    ).rejects.toThrow("Nickname: Nickname must be 50 characters or less.");
  });

  it("uses fallback nickname update error for non-json response", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(
      updateContactNickname(accessToken, user.id, "Friend")
    ).rejects.toThrow("Failed to update contact nickname.");
  });

  it("updates current user avatar with FormData", async () => {
    const avatar = new File(["avatar content"], "avatar.png", {
      type: "image/png",
    });

    const updatedUser = {
      ...user,
      avatar_url: "http://localhost:8000/media/users/avatars/avatar.png",
    };

    fetchMock.mockResolvedValue(createJsonResponse(updatedUser));

    const result = await updateCurrentUserAvatar(accessToken, avatar);

    expect(fetchMock).toHaveBeenCalledTimes(1);

    const [url, requestInit] = fetchMock.mock.calls[0] as [
      string,
      RequestInit,
    ];

    expect(url).toBe(`${API_BASE_URL}/auth/me/avatar/`);
    expect(requestInit.method).toBe("PATCH");
    expect(requestInit.headers).toEqual({
      Authorization: `Bearer ${accessToken}`,
    });
    expect(requestInit.body).toBeInstanceOf(FormData);
    expect((requestInit.body as FormData).get("avatar")).toBe(avatar);
    expect(result).toEqual(updatedUser);
  });

  it("throws when avatar update fails", async () => {
    const avatar = new File(["avatar content"], "avatar.png", {
      type: "image/png",
    });

    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(updateCurrentUserAvatar(accessToken, avatar)).rejects.toThrow(
      "Failed to update avatar."
    );
  });

  it("deletes current user avatar", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(user));

    const result = await deleteCurrentUserAvatar(accessToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/me/avatar/`, {
      method: "DELETE",
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual(user);
  });

  it("throws when avatar delete fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(deleteCurrentUserAvatar(accessToken)).rejects.toThrow(
      "Failed to delete avatar."
    );
  });

  it("logs user out", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse({ detail: "Successfully logged out." })
    );

    const result = await logout(accessToken, refreshToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/auth/logout/`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${accessToken}`,
      },
      body: JSON.stringify({
        refresh: refreshToken,
      }),
    });
    expect(result).toBeUndefined();
  });

  it("throws when logout fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(logout(accessToken, refreshToken)).rejects.toThrow(
      "Failed to logout."
    );
  });
});
