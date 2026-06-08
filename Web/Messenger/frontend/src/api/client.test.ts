import { describe, expect, it, vi, beforeEach, afterEach } from "vitest";

import { apiRequest } from "./client";
import { API_BASE_URL } from "./config";

describe("apiRequest", () => {
  const fetchMock = vi.fn();

  beforeEach(() => {
    fetchMock.mockReset();
    vi.stubGlobal("fetch", fetchMock);
  });

  afterEach(() => {
    vi.unstubAllGlobals();
  });

  it("adds authorization header when access token is provided", async () => {
    fetchMock.mockResolvedValue({
      ok: true,
      json: vi.fn().mockResolvedValue([{ id: 1 }]),
    } as unknown as Response);

    const result = await apiRequest<{ id: number }[]>("/users/", {
      accessToken: "access-token",
      errorMessage: "Failed to load users.",
    });

    expect(result).toEqual([{ id: 1 }]);
    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/users/`, {
      headers: {
        Authorization: "Bearer access-token",
      },
    });
  });

  it("adds content type header and stringifies json body", async () => {
    fetchMock.mockResolvedValue({
      ok: true,
      json: vi.fn().mockResolvedValue({ id: 1 }),
    } as unknown as Response);

    await apiRequest<{ id: number }>("/messages/", {
      accessToken: "access-token",
      method: "POST",
      json: { text: "Hello" },
      errorMessage: "Failed to create message.",
    });

    expect(fetchMock).toHaveBeenCalledWith(`${API_BASE_URL}/messages/`, {
      headers: {
        "Content-Type": "application/json",
        Authorization: "Bearer access-token",
      },
      method: "POST",
      body: JSON.stringify({ text: "Hello" }),
    });
  });

  it("does not add content type header for form data", async () => {
    const formData = new FormData();
    formData.append("avatar", new File(["avatar"], "avatar.png"));

    fetchMock.mockResolvedValue({
      ok: true,
      json: vi.fn().mockResolvedValue({ id: 1 }),
    } as unknown as Response);

    await apiRequest<{ id: number }>("/avatar/", {
      accessToken: "access-token",
      method: "PATCH",
      body: formData,
      errorMessage: "Failed to update avatar.",
    });

    const requestInit = fetchMock.mock.calls[0][1] as RequestInit;

    expect(fetchMock.mock.calls[0][0]).toBe(`${API_BASE_URL}/avatar/`);
    expect(requestInit.headers).toEqual({
      Authorization: "Bearer access-token",
    });
    expect(requestInit.body).toBe(formData);
  });

  it("throws fallback error message when request fails", async () => {
    fetchMock.mockResolvedValue({
      ok: false,
      json: vi.fn(),
    } as unknown as Response);

    await expect(
      apiRequest("/messages/", {
        accessToken: "access-token",
        errorMessage: "Failed to load messages.",
      })
    ).rejects.toThrow("Failed to load messages.");
  });

  it("uses custom parseError when request fails", async () => {
    const response = {
      ok: false,
      json: vi.fn().mockResolvedValue({ detail: "Blocked." }),
    } as unknown as Response;

    const parseError = vi.fn(async () => new Error("Blocked."));

    fetchMock.mockResolvedValue(response);

    await expect(
      apiRequest("/users/1/block/", {
        accessToken: "access-token",
        method: "POST",
        errorMessage: "Request failed.",
        parseError,
      })
    ).rejects.toThrow("Blocked.");

    expect(parseError).toHaveBeenCalledWith(response);
  });

  it("does not parse json when parseJson is false", async () => {
    const json = vi.fn();

    fetchMock.mockResolvedValue({
      ok: true,
      json,
    } as unknown as Response);

    const result = await apiRequest<void>("/auth/logout/", {
      accessToken: "access-token",
      method: "POST",
      json: { refresh: "refresh-token" },
      errorMessage: "Failed to logout.",
      parseJson: false,
    });

    expect(result).toBeUndefined();
    expect(json).not.toHaveBeenCalled();
  });
});
