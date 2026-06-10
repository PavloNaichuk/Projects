import { render, screen, waitFor } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import { requestPasswordReset } from "../api/auth";
import ForgotPasswordPage from "./ForgotPasswordPage";

vi.mock("../api/auth", () => ({
  requestPasswordReset: vi.fn(),
}));

const requestPasswordResetMock = vi.mocked(requestPasswordReset);

describe("ForgotPasswordPage", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    window.history.pushState({}, "", "/forgot-password");
  });

  it("renders password reset request form", () => {
    render(<ForgotPasswordPage />);

    expect(screen.getByRole("heading", { name: "Messenger" })).toBeTruthy();
    expect(screen.getByText("Reset your password")).toBeTruthy();
    expect(screen.getByLabelText("Email")).toBeTruthy();
    expect(
      screen.getByRole("button", { name: "Send reset link" })
    ).toBeTruthy();
    expect(
      screen.getByRole("link", { name: "Back to sign in" })
    ).toHaveAttribute("href", "/");
  });

  it("requests password reset and shows success message", async () => {
    const user = userEvent.setup();

    requestPasswordResetMock.mockResolvedValue({
      detail: "If an account with this email exists, a password reset email was sent.",
    });

    render(<ForgotPasswordPage />);

    await user.type(screen.getByLabelText("Email"), "pavlo@test.ua");
    await user.click(screen.getByRole("button", { name: "Send reset link" }));

    expect(requestPasswordResetMock).toHaveBeenCalledWith("pavlo@test.ua");

    await waitFor(() => {
      expect(
        screen.getByText(
          "If an account with this email exists, a password reset email was sent."
        )
      ).toBeTruthy();
    });
  });

  it("shows loading state while request is submitting", async () => {
    const user = userEvent.setup();

    requestPasswordResetMock.mockReturnValue(new Promise(() => undefined));

    render(<ForgotPasswordPage />);

    await user.type(screen.getByLabelText("Email"), "pavlo@test.ua");
    await user.click(screen.getByRole("button", { name: "Send reset link" }));

    const button = screen.getByRole("button", { name: "Sending..." });

    expect(button).toHaveAttribute("disabled");
  });

  it("shows API error message", async () => {
    const user = userEvent.setup();

    requestPasswordResetMock.mockRejectedValue(
      new Error("Email: Enter a valid email.")
    );

    render(<ForgotPasswordPage />);

    await user.type(screen.getByLabelText("Email"), "pavlo@test.ua");
    await user.click(screen.getByRole("button", { name: "Send reset link" }));

    await waitFor(() => {
      expect(screen.getByText("Please check the form:")).toBeTruthy();
      expect(screen.getByText("Email: Enter a valid email.")).toBeTruthy();
    });
  });

  it("shows fallback error message for unknown errors", async () => {
    const user = userEvent.setup();

    requestPasswordResetMock.mockRejectedValue("Unknown error");

    render(<ForgotPasswordPage />);

    await user.type(screen.getByLabelText("Email"), "pavlo@test.ua");
    await user.click(screen.getByRole("button", { name: "Send reset link" }));

    await waitFor(() => {
      expect(screen.getByText("Failed to request password reset.")).toBeTruthy();
    });
  });
});
