import type { FormEvent } from "react";

type AuthMode = "login" | "register";

type AuthPageProps = {
  authMode: AuthMode;
  setAuthMode: (mode: AuthMode) => void;

  username: string;
  setUsername: (value: string) => void;

  email: string;
  setEmail: (value: string) => void;

  password: string;
  setPassword: (value: string) => void;

  passwordConfirm: string;
  setPasswordConfirm: (value: string) => void;

  error: string;
  setError: (value: string) => void;

  isLoading: boolean;

  handleLogin: (event: FormEvent<HTMLFormElement>) => Promise<void>;
  handleRegister: (event: FormEvent<HTMLFormElement>) => Promise<void>;
};

function AuthPage({
  authMode,
  setAuthMode,
  username,
  setUsername,
  email,
  setEmail,
  password,
  setPassword,
  passwordConfirm,
  setPasswordConfirm,
  error,
  setError,
  isLoading,
  handleLogin,
  handleRegister,
}: AuthPageProps) {
  const isRegisterMode = authMode === "register";

  return (
    <div className="auth-page">
      <form
        className="auth-card"
        onSubmit={isRegisterMode ? handleRegister : handleLogin}
      >
        <h1>Messenger</h1>
        <p>{isRegisterMode ? "Create your account" : "Sign in to continue"}</p>

        <label>
          Username
          <input
            type="text"
            value={username}
            onChange={(event) => setUsername(event.target.value)}
            placeholder="Username"
          />
        </label>

        {isRegisterMode && (
          <label>
            Email
            <input
              type="email"
              value={email}
              onChange={(event) => setEmail(event.target.value)}
              placeholder="Email"
            />
          </label>
        )}

        <label>
          Password
          <input
            type="password"
            value={password}
            onChange={(event) => setPassword(event.target.value)}
            placeholder="Password"
          />
        </label>

        {isRegisterMode && (
          <label>
            Confirm password
            <input
              type="password"
              value={passwordConfirm}
              onChange={(event) => setPasswordConfirm(event.target.value)}
              placeholder="Confirm password"
            />
          </label>
        )}

        {error && (
          <div className="auth-error" role="alert">
            <strong>
              {authMode === "register"
                ? "Please check the form:"
                : "Sign in failed:"}
            </strong>

            <ul>
              {error.split("\n").map((message) => (
                <li key={message}>{message}</li>
              ))}
            </ul>
          </div>
        )}

        <button type="submit" disabled={isLoading}>
          {isLoading
            ? isRegisterMode
              ? "Creating account..."
              : "Signing in..."
            : isRegisterMode
            ? "Create account"
            : "Sign in"}
        </button>

        {!isRegisterMode && (
          <a className="auth-secondary-link" href="/forgot-password">
            Forgot password?
          </a>
        )}

        <button
          type="button"
          className="auth-switch-button"
          onClick={() => {
            setError("");
            setAuthMode(isRegisterMode ? "login" : "register");
          }}
        >
          {isRegisterMode
            ? "Already have an account? Sign in"
            : "No account? Create one"}
        </button>
      </form>
    </div>
  );
}

export default AuthPage;
