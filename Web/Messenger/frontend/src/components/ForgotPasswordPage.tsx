import { type FormEvent, useState } from "react";

import { requestPasswordReset } from "../api/auth";

function ForgotPasswordPage() {
  const [email, setEmail] = useState("");
  const [message, setMessage] = useState("");
  const [error, setError] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    setMessage("");
    setError("");
    setIsSubmitting(true);

    try {
      const response = await requestPasswordReset(email);
      setMessage(response.detail);
    } catch (error) {
      setError(
        error instanceof Error
          ? error.message
          : "Failed to request password reset."
      );
    } finally {
      setIsSubmitting(false);
    }
  }

  return (
    <div className="auth-page">
      <form className="auth-card" onSubmit={handleSubmit}>
        <h1>Messenger</h1>
        <p>Reset your password</p>

        <label>
          Email
          <input
            type="email"
            value={email}
            onChange={(event) => setEmail(event.target.value)}
            placeholder="Email"
          />
        </label>

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

        <button type="submit" disabled={isSubmitting}>
          {isSubmitting ? "Sending..." : "Send reset link"}
        </button>

        <a className="auth-secondary-link" href="/">
            Back to sign in
        </a>
      </form>
    </div>
  );
}

export default ForgotPasswordPage;
