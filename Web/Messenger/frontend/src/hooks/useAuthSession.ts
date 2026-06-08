import {
  useCallback,
  useEffect,
  useState,
  type Dispatch,
  type FormEvent,
  type SetStateAction,
} from "react";

import {
  getCurrentUser,
  login,
  logout,
  refreshAccessToken,
  register,
  type User,
} from "../api/auth";
import { getConversations, type Conversation, type Message } from "../api/conversations";
import { sortConversationsByUpdatedAt } from "../utils/chat";

type UseAuthSessionParams = {
  setConversations: Dispatch<SetStateAction<Conversation[]>>;
  setSelectedConversation: Dispatch<SetStateAction<Conversation | null>>;
  setMessages: Dispatch<SetStateAction<Message[]>>;
  setHasMoreMessages: Dispatch<SetStateAction<boolean>>;
  resetSessionState: () => void;
};

export function useAuthSession({
  setConversations,
  setSelectedConversation,
  setMessages,
  setHasMoreMessages,
  resetSessionState,
}: UseAuthSessionParams) {
  const [currentUser, setCurrentUser] = useState<User | null>(null);
  const [accessToken, setAccessToken] = useState<string | null>(null);

  const [authMode, setAuthMode] = useState<"login" | "register">("login");

  const [username, setUsername] = useState("pavlo");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [passwordConfirm, setPasswordConfirm] = useState("");

  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [isAuthChecking, setIsAuthChecking] = useState(true);

  const loadUserData = useCallback(
    async (token: string) => {
      const user = await getCurrentUser(token);
      setCurrentUser(user);
      setAccessToken(token);

      const conversationsData = await getConversations(token);
      setConversations(sortConversationsByUpdatedAt(conversationsData));

      setSelectedConversation(null);
      setMessages([]);
      setHasMoreMessages(false);
    },
    [
      setConversations,
      setHasMoreMessages,
      setMessages,
      setSelectedConversation,
    ]
  );

  const clearSession = useCallback(() => {
    localStorage.removeItem("accessToken");
    localStorage.removeItem("refreshToken");

    setCurrentUser(null);
    setAccessToken(null);
    resetSessionState();
  }, [resetSessionState]);

  const handleLogout = useCallback(async () => {
    const savedRefreshToken = localStorage.getItem("refreshToken");

    if (accessToken && savedRefreshToken) {
      try {
        await logout(accessToken, savedRefreshToken);
      } catch {
        console.error("Logout request failed.");
      }
    }

    clearSession();
  }, [accessToken, clearSession]);

  const handleLogin = useCallback(
    async (event: FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      setError("");
      setIsLoading(true);

      try {
        const tokens = await login(username, password);

        localStorage.setItem("accessToken", tokens.access);
        localStorage.setItem("refreshToken", tokens.refresh);

        await loadUserData(tokens.access);
      } catch {
        setError("Invalid username or password.");
      } finally {
        setIsLoading(false);
      }
    },
    [loadUserData, password, username]
  );

  const handleRegister = useCallback(
    async (event: FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      const trimmedUsername = username.trim();
      const trimmedEmail = email.trim();
      const trimmedPassword = password.trim();
      const trimmedPasswordConfirm = passwordConfirm.trim();

      if (!trimmedUsername || !trimmedEmail || !trimmedPassword) {
        setError("Username, email and password are required.");
        return;
      }

      if (trimmedPassword !== trimmedPasswordConfirm) {
        setError("Passwords do not match.");
        return;
      }

      setError("");
      setIsLoading(true);

      try {
        const tokens = await register(
          trimmedUsername,
          trimmedEmail,
          trimmedPassword,
          trimmedPasswordConfirm
        );

        localStorage.setItem("accessToken", tokens.access);
        localStorage.setItem("refreshToken", tokens.refresh);

        await loadUserData(tokens.access);
      } catch (error) {
        if (error instanceof Error) {
          setError(error.message);
        } else {
          setError("Failed to create account.");
        }
      } finally {
        setIsLoading(false);
      }
    },
    [email, loadUserData, password, passwordConfirm, username]
  );

  useEffect(() => {
    async function restoreSession() {
      const savedAccessToken = localStorage.getItem("accessToken");
      const savedRefreshToken = localStorage.getItem("refreshToken");

      if (!savedAccessToken && !savedRefreshToken) {
        setIsAuthChecking(false);
        return;
      }

      try {
        if (savedAccessToken) {
          await loadUserData(savedAccessToken);
          return;
        }

        if (savedRefreshToken) {
          const refreshedToken = await refreshAccessToken(savedRefreshToken);
          localStorage.setItem("accessToken", refreshedToken.access);
          await loadUserData(refreshedToken.access);
        }
      } catch {
        if (!savedRefreshToken) {
          clearSession();
          return;
        }

        try {
          const refreshedToken = await refreshAccessToken(savedRefreshToken);
          localStorage.setItem("accessToken", refreshedToken.access);
          await loadUserData(refreshedToken.access);
        } catch {
          clearSession();
        }
      } finally {
        setIsAuthChecking(false);
      }
    }

    restoreSession();
  }, [clearSession, loadUserData]);

  return {
    currentUser,
    setCurrentUser,
    accessToken,
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
    isAuthChecking,
    loadUserData,
    handleLogin,
    handleRegister,
    handleLogout,
  };
}
