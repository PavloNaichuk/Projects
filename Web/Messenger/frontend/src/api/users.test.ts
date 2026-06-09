import { beforeEach, describe, expect, it, vi } from "vitest";

import type { User } from "./auth";
import { API_BASE_URL } from "./config";
import {
  blockUser,
  getBlockedUsers,
  searchUsers,
  unblockUser,
} from "./users";

const accessToken = "access-token";

const user = {
  id: 1,
  username: "user1",
  email: "user1@test.ua",
  display_name: "User 1",
  avatar_url: null,
  is_online: false,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: false,
  has_blocked_me: false,
} as User;

const blockedUser = {
  id: 2,
  username: "blocked",
  email: "blocked@test.ua",
  display_name: "Blocked User",
  avatar_url: null,
  is_online: false,
  last_seen_at: null,
  email_verified_at: null,
  is_email_verified: false,
  is_blocked_by_me: true,
  has_blocked_me: false,
} as User;

const fetchMock = vi.fn();

function createJsonResponse(data: unknown, ok = true) {
  return {
    ok,
    json: vi.fn().mockResolvedValue(data),
  } as unknown as Response;
}

describe("users api", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    vi.stubGlobal("fetch", fetchMock);
  });

  it("searches users with trimmed query and authorization header", async () => {
    fetchMock.mockResolvedValue(createJsonResponse([user]));

    const result = await searchUsers(accessToken, "  user  ");

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/users/search/?q=user`, {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual([user]);
  });

  it("searches users without query parameter when query is empty", async () => {
    fetchMock.mockResolvedValue(createJsonResponse([user]));

    const result = await searchUsers(accessToken, "   ");

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/users/search/?`, {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual([user]);
  });

  it("throws when user search fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(searchUsers(accessToken, "user")).rejects.toThrow(
      "Failed to search users."
    );
  });

  it("loads blocked users", async () => {
    fetchMock.mockResolvedValue(createJsonResponse([blockedUser]));

    const result = await getBlockedUsers(accessToken);

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/users/blocked/`, {
      headers: {
        Authorization: `Bearer ${accessToken}`,
      },
    });
    expect(result).toEqual([blockedUser]);
  });

  it("throws when loading blocked users fails", async () => {
    fetchMock.mockResolvedValue(createJsonResponse(null, false));

    await expect(getBlockedUsers(accessToken)).rejects.toThrow(
      "Failed to load blocked users."
    );
  });

  it("blocks user", async () => {
    const responseData = {
      detail: "User blocked.",
      user: blockedUser,
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await blockUser(accessToken, blockedUser.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/users/${blockedUser.id}/block/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("unblocks user", async () => {
    const responseData = {
      detail: "User unblocked.",
      user: {
        ...blockedUser,
        is_blocked_by_me: false,
      },
    };

    fetchMock.mockResolvedValue(createJsonResponse(responseData));

    const result = await unblockUser(accessToken, blockedUser.id);

    expect(fetchMock).toHaveBeenCalledWith(
      `${API_BASE_URL}/users/${blockedUser.id}/unblock/`,
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
        },
      }
    );
    expect(result).toEqual(responseData);
  });

  it("uses backend detail message when block request fails", async () => {
    fetchMock.mockResolvedValue(
      createJsonResponse(
        {
          detail: "You cannot block yourself.",
        },
        false
      )
    );

    await expect(blockUser(accessToken, user.id)).rejects.toThrow(
      "You cannot block yourself."
    );
  });

  it("uses fallback error message when block response has no detail", async () => {
    fetchMock.mockResolvedValue(createJsonResponse({}, false));

    await expect(blockUser(accessToken, user.id)).rejects.toThrow(
      "Request failed."
    );
  });

  it("uses fallback error message when unblock error response is not json", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn().mockRejectedValue(new Error("Invalid JSON")),
    } as unknown as Response);

    await expect(unblockUser(accessToken, user.id)).rejects.toThrow(
      "Request failed."
    );
  });
});
