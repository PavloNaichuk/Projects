import { useEffect, useMemo, useState } from "react";

import { verifyEmail, type User } from "../api/auth";

type VerificationStatus = "loading" | "success" | "error";

type VerifyEmailPageProps = {
  handleVerified?: (user: User) => void;
};

function VerifyEmailPage({ handleVerified }: VerifyEmailPageProps) {
  const token = useMemo(
    () => new URLSearchParams(window.location.search).get("token"),
    []
  );

  const [status, setStatus] = useState<VerificationStatus>(() =>
    token ? "loading" : "error"
  );
  const [message, setMessage] = useState(() =>
    token ? "Verifying your email..." : "Verification token is missing."
  );

  useEffect(() => {
    if (!token) {
      return;
    }

    const verificationToken = token;
    let isMounted = true;

    async function verifyToken() {
      try {
        const response = await verifyEmail(verificationToken);

        if (!isMounted) {
          return;
        }

        handleVerified?.(response.user);
        setStatus("success");
        setMessage(response.detail);
      } catch (error) {
        if (!isMounted) {
          return;
        }

        setStatus("error");
        setMessage(
          error instanceof Error ? error.message : "Failed to verify email."
        );
      }
    }

    verifyToken();

    return () => {
      isMounted = false;
    };
  }, [handleVerified, token]);

  return (
    <div className="auth-page">
      <div className="auth-card">
        <h1>Messenger</h1>
        <h2>Email verification</h2>

        <p>{message}</p>

        {status === "loading" && <p>Please wait...</p>}

        {status !== "loading" && (
          <button
            type="button"
            onClick={() => {
              window.location.href = "/";
            }}
          >
            {status === "success" ? "Open Messenger" : "Back to login"}
          </button>
        )}
      </div>
    </div>
  );
}

export default VerifyEmailPage;
