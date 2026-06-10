import { type FormEvent, useMemo, useState } from "react";

import { confirmPasswordReset } from "../api/auth";

function ResetPasswordPage() {
  const queryParams = useMemo(
    () => new URLSearchParams(window.location.search),
    []
  );

  const uid = queryParams.get("uid");
  const token = queryParams.get("token");

  const [password, setPassword] = useState("");
  const [passwordConfirm, setPasswordConfirm] = useState("");
  const [message, setMessage] = useState("");
  const [error, setError] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);
  const isResetSuccessful = Boolean(message);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    if (!uid || !token) {
      setError("Password reset link is invalid.");
      return;
    }

    setMessage("");
    setError("");
    setIsSubmitting(true);

    try {
      const response = await confirmPasswordReset(
        uid,
        token,
        password,
        passwordConfirm
      );

      setMessage(response.detail);
      setPassword("");
      setPasswordConfirm("");
    } catch (error) {
      setError(
        error instanceof Error
          ? error.message
          : "Failed to reset password."
      );
    } finally {
      setIsSubmitting(false);
    }
  }

  return (
    <div className="auth-page">
      <form className="auth-card" onSubmit={handleSubmit}>
        <h1>Messenger</h1>
        <p>Create a new password</p>

        {!uid || !token ? (
          <div className="auth-error" role="alert">
            <strong>Password reset link is invalid.</strong>
          </div>
        ) : (
          !isResetSuccessful && (
            <>
              <label>
                New password
                <input
                  type="password"
                  value={password}
                  onChange={(event) => setPassword(event.target.value)}
                  placeholder="New password"
                />
              </label>

              <label>
                Confirm new password
                <input
                  type="password"
                  value={passwordConfirm}
                  onChange={(event) => setPasswordConfirm(event.target.value)}
                  placeholder="Confirm new password"
                />
              </label>
            </>
          )
        )}

        {message && (
          <div className="auth-success" role="status">
            {message}
          </div>
        )}

        {error && (
          <div className="auth-error" role="alert">
            <strong>Please check the form:</strong>

            <ul>
              {error.split("\n").map((message) => (
                <li key={message}>{message}</li>
              ))}
            </ul>
          </div>
        )}

        {!isResetSuccessful && (
          <button type="submit" disabled={isSubmitting || !uid || !token}>
            {isSubmitting ? "Saving..." : "Reset password"}
          </button>
        )}

        <a
          className={
            isResetSuccessful ? "auth-primary-link" : "auth-secondary-link"
          }
          href="/"
        >
          {isResetSuccessful ? "Sign in" : "Back to sign in"}
        </a>
      </form>
    </div>
  );
}

export default ResetPasswordPage;
