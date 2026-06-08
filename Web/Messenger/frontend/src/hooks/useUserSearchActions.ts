import {
  useCallback,
  type Dispatch,
  type FormEvent,
  type SetStateAction,
} from "react";

import type { User } from "../api/auth";
import type { Message } from "../api/conversations";
import { blockUser, searchUsers, unblockUser } from "../api/users";

type UseUserSearchActionsParams = {
  accessToken: string | null;
  userSearchQuery: string;
  selectedConversationUser: User | null;
  updateUserInState: (user: User) => void;
  refreshConversations: (token: string) => Promise<void>;
  sendTypingStatus: (isTyping: boolean) => void;
  clearTypingTimeout: () => void;
  setSearchResults: Dispatch<SetStateAction<User[]>>;
  setIsSearchingUsers: Dispatch<SetStateAction<boolean>>;
  setUserSearchError: Dispatch<SetStateAction<string>>;
  setIsBlockingUserId: Dispatch<SetStateAction<number | null>>;
  setMessageError: Dispatch<SetStateAction<string>>;
  setNewMessage: Dispatch<SetStateAction<string>>;
  setSelectedAttachment: Dispatch<SetStateAction<File | null>>;
  setReplyToMessage: Dispatch<SetStateAction<Message | null>>;
  setTypingUser: Dispatch<SetStateAction<User | null>>;
};

export function useUserSearchActions({
  accessToken,
  userSearchQuery,
  selectedConversationUser,
  updateUserInState,
  refreshConversations,
  sendTypingStatus,
  clearTypingTimeout,
  setSearchResults,
  setIsSearchingUsers,
  setUserSearchError,
  setIsBlockingUserId,
  setMessageError,
  setNewMessage,
  setSelectedAttachment,
  setReplyToMessage,
  setTypingUser,
}: UseUserSearchActionsParams) {
  const handleSearchUsers = useCallback(
    async (event: FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (!accessToken) {
        return;
      }

      const query = userSearchQuery.trim();

      if (!query) {
        setSearchResults([]);
        setUserSearchError("");
        return;
      }

      setIsSearchingUsers(true);
      setUserSearchError("");

      try {
        const users = await searchUsers(accessToken, query);
        setSearchResults(users);
      } catch {
        setSearchResults([]);
        setUserSearchError("Failed to search users.");
      } finally {
        setIsSearchingUsers(false);
      }
    },
    [
      accessToken,
      setIsSearchingUsers,
      setSearchResults,
      setUserSearchError,
      userSearchQuery,
    ]
  );

  const handleBlockUser = useCallback(
    async (user: User) => {
      if (!accessToken) {
        return;
      }

      setIsBlockingUserId(user.id);
      setMessageError("");
      setUserSearchError("");

      try {
        const response = await blockUser(accessToken, user.id);

        updateUserInState(response.user);
        await refreshConversations(accessToken);

        if (selectedConversationUser?.id === user.id) {
          setNewMessage("");
          setSelectedAttachment(null);
          setReplyToMessage(null);
          setTypingUser(null);
          sendTypingStatus(false);
          clearTypingTimeout();
        }
      } catch (error) {
        const errorMessage =
          error instanceof Error ? error.message : "Failed to block user.";

        if (selectedConversationUser?.id === user.id) {
          setMessageError(errorMessage);
        } else {
          setUserSearchError(errorMessage);
        }
      } finally {
        setIsBlockingUserId(null);
      }
    },
    [
      accessToken,
      clearTypingTimeout,
      refreshConversations,
      selectedConversationUser,
      sendTypingStatus,
      setIsBlockingUserId,
      setMessageError,
      setNewMessage,
      setReplyToMessage,
      setSelectedAttachment,
      setTypingUser,
      setUserSearchError,
      updateUserInState,
    ]
  );

  const handleUnblockUser = useCallback(
    async (user: User) => {
      if (!accessToken) {
        return;
      }

      setIsBlockingUserId(user.id);
      setMessageError("");
      setUserSearchError("");

      try {
        const response = await unblockUser(accessToken, user.id);

        updateUserInState(response.user);
        await refreshConversations(accessToken);
      } catch (error) {
        const errorMessage =
          error instanceof Error ? error.message : "Failed to unblock user.";

        if (selectedConversationUser?.id === user.id) {
          setMessageError(errorMessage);
        } else {
          setUserSearchError(errorMessage);
        }
      } finally {
        setIsBlockingUserId(null);
      }
    },
    [
      accessToken,
      refreshConversations,
      selectedConversationUser,
      setIsBlockingUserId,
      setMessageError,
      setUserSearchError,
      updateUserInState,
    ]
  );

  return {
    handleSearchUsers,
    handleBlockUser,
    handleUnblockUser,
  };
}
