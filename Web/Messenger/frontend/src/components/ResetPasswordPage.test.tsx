import { render, screen, waitFor } from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import { beforeEach, describe, expect, it, vi } from "vitest";

import { confirmPasswordReset } from "../api/auth";
import ResetPasswordPage from "./ResetPasswordPage";

vi.mock("../api/auth", () => ({
  confirmPasswordReset: vi.fn(),
}));

const confirmPasswordResetMock = vi.mocked(confirmPasswordReset);

describe("ResetPasswordPage", () => {
  beforeEach(() => {
    vi.clearAllMocks();
    window.history.pushState(
      {},
      "",
      "/reset-password?uid=Ng&token=reset-token"
    );
  });

  it("renders password reset confirm form", () => {
    render(<ResetPasswordPage />);

    expect(screen.getByRole("heading", { name: "Messenger" })).toBeTruthy();
    expect(screen.getByText("Create a new password")).toBeTruthy();
    expect(screen.getByLabelText("New password")).toBeTruthy();
    expect(screen.getByLabelText("Confirm new password")).toBeTruthy();
    expect(screen.getByRole("button", { name: "Reset password" })).toBeTruthy();
    expect(
      screen.getByRole("link", { name: "Back to sign in" })
    ).toHaveAttribute("href", "/");
  });

  it("confirms password reset and shows success message", async () => {
    const user = userEvent.setup();

    confirmPasswordResetMock.mockResolvedValue({
      detail: "Password reset successfully.",
    });

    render(<ResetPasswordPage />);

    await user.type(screen.getByLabelText("New password"), "newpassword123");
    await user.type(
      screen.getByLabelText("Confirm new password"),
      "newpassword123"
    );
    await user.click(screen.getByRole("button", { name: "Reset password" }));

    expect(confirmPasswordResetMock).toHaveBeenCalledWith(
      "Ng",
      "reset-token",
      "newpassword123",
      "newpassword123"
    );

    await waitFor(() => {
      expect(screen.getByText("Password reset successfully.")).toBeTruthy();
    });

    expect(screen.queryByLabelText("New password")).toBeNull();
    expect(screen.queryByLabelText("Confirm new password")).toBeNull();
    expect(screen.queryByRole("button", { name: "Reset password" })).toBeNull();

    expect(screen.getByRole("link", { name: "Sign in" })).toHaveAttribute(
      "href",
      "/"
    );
  });

  it("shows loading state while password reset is submitting", async () => {
    const user = userEvent.setup();

    confirmPasswordResetMock.mockReturnValue(new Promise(() => undefined));

    render(<ResetPasswordPage />);

    await user.type(screen.getByLabelText("New password"), "newpassword123");
    await user.type(
      screen.getByLabelText("Confirm new password"),
      "newpassword123"
    );
    await user.click(screen.getByRole("button", { name: "Reset password" }));

    const button = screen.getByRole("button", { name: "Saving..." });

    expect(button).toHaveAttribute("disabled");
  });

  it("shows API error message", async () => {
    const user = userEvent.setup();

    confirmPasswordResetMock.mockRejectedValue(
      new Error("Password: This password is too common.")
    );

    render(<ResetPasswordPage />);

    await user.type(screen.getByLabelText("New password"), "password");
    await user.type(screen.getByLabelText("Confirm new password"), "password");
    await user.click(screen.getByRole("button", { name: "Reset password" }));

    await waitFor(() => {
      expect(screen.getByText("Please check the form:")).toBeTruthy();
      expect(
        screen.getByText("Password: This password is too common.")
      ).toBeTruthy();
    });
  });

  it("shows fallback error message for unknown errors", async () => {
    const user = userEvent.setup();

    confirmPasswordResetMock.mockRejectedValue("Unknown error");

    render(<ResetPasswordPage />);

    await user.type(screen.getByLabelText("New password"), "newpassword123");
    await user.type(
      screen.getByLabelText("Confirm new password"),
      "newpassword123"
    );
    await user.click(screen.getByRole("button", { name: "Reset password" }));

    await waitFor(() => {
      expect(screen.getByText("Failed to reset password.")).toBeTruthy();
    });
  });

  it("shows invalid link error when uid or token is missing", () => {
    window.history.pushState({}, "", "/reset-password");

    render(<ResetPasswordPage />);

    expect(screen.getByText("Password reset link is invalid.")).toBeTruthy();
    expect(screen.queryByLabelText("New password")).toBeNull();
    expect(screen.queryByLabelText("Confirm new password")).toBeNull();

    expect(
      screen.getByRole("button", { name: "Reset password" })
    ).toHaveAttribute("disabled");

    expect(
      screen.getByRole("link", { name: "Back to sign in" })
    ).toHaveAttribute("href", "/");
  });
});
