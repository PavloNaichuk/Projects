import { useCallback, type ChangeEvent, type Dispatch, type SetStateAction } from "react";

import {
  deleteCurrentUserAvatar,
  updateContactNickname,
  updateCurrentUserAvatar,
  updateCurrentUserProfile,
  type User,
} from "../api/auth";
import type { Conversation } from "../api/conversations";
import { getBlockedUsers, unblockUser } from "../api/users";
import { getOtherParticipant } from "../utils/chat";
import { addAvatarCacheBust } from "../utils/users";

type UseProfileActionsParams = {
  accessToken: string | null;
  currentUser: User | null;
  contactNicknameUser: User | null;
  setAvatarError: Dispatch<SetStateAction<string>>;
  setIsAvatarUpdating: Dispatch<SetStateAction<boolean>>;
  setIsProfileSettingsOpen: Dispatch<SetStateAction<boolean>>;
  setProfileError: Dispatch<SetStateAction<string>>;
  setIsProfileUpdating: Dispatch<SetStateAction<boolean>>;
  setBlockedUsers: Dispatch<SetStateAction<User[]>>;
  setBlockedUsersError: Dispatch<SetStateAction<string>>;
  setIsBlockedUsersLoading: Dispatch<SetStateAction<boolean>>;
  setIsUnblockingUserId: Dispatch<SetStateAction<number | null>>;
  setContactNicknameUser: Dispatch<SetStateAction<User | null>>;
  setContactNicknameError: Dispatch<SetStateAction<string>>;
  setIsContactNicknameUpdating: Dispatch<SetStateAction<boolean>>;
  updateUserInState: (user: User) => void;
  refreshConversations: (token: string) => Promise<void>;
};

export function useProfileActions({
  accessToken,
  currentUser,
  contactNicknameUser,
  setAvatarError,
  setIsAvatarUpdating,
  setIsProfileSettingsOpen,
  setProfileError,
  setIsProfileUpdating,
  setBlockedUsers,
  setBlockedUsersError,
  setIsBlockedUsersLoading,
  setIsUnblockingUserId,
  setContactNicknameUser,
  setContactNicknameError,
  setIsContactNicknameUpdating,
  updateUserInState,
  refreshConversations,
}: UseProfileActionsParams) {
  const handleCurrentUserAvatarChange = useCallback(
    async (event: ChangeEvent<HTMLInputElement>) => {
      const file = event.target.files?.[0];
      event.target.value = "";

      if (!file || !accessToken) {
        return;
      }

      if (!file.type.startsWith("image/")) {
        setAvatarError("Avatar must be an image.");
        return;
      }

      if (file.size > 2 * 1024 * 1024) {
        setAvatarError("Avatar file size must be 2 MB or less.");
        return;
      }

      setIsAvatarUpdating(true);
      setAvatarError("");

      try {
        const updatedUser = await updateCurrentUserAvatar(accessToken, file);
        updateUserInState(addAvatarCacheBust(updatedUser));
        await refreshConversations(accessToken);
      } catch {
        setAvatarError("Failed to update avatar.");
      } finally {
        setIsAvatarUpdating(false);
      }
    },
    [
      accessToken,
      refreshConversations,
      setAvatarError,
      setIsAvatarUpdating,
      updateUserInState,
    ]
  );

  const handleDeleteCurrentUserAvatar = useCallback(async () => {
    if (!accessToken) {
      return;
    }

    setIsAvatarUpdating(true);
    setAvatarError("");

    try {
      const updatedUser = await deleteCurrentUserAvatar(accessToken);
      updateUserInState(updatedUser);
      await refreshConversations(accessToken);
    } catch {
      setAvatarError("Failed to delete avatar.");
    } finally {
      setIsAvatarUpdating(false);
    }
  }, [
    accessToken,
    refreshConversations,
    setAvatarError,
    setIsAvatarUpdating,
    updateUserInState,
  ]);

  const handleUpdateCurrentUserProfile = useCallback(
    async (username: string, email: string) => {
      if (!accessToken) {
        return;
      }

      const trimmedUsername = username.trim();
      const trimmedEmail = email.trim();

      if (!trimmedUsername || !trimmedEmail) {
        setProfileError("Username and email are required.");
        return;
      }

      setIsProfileUpdating(true);
      setProfileError("");

      try {
        const updatedUser = await updateCurrentUserProfile(accessToken, {
          username: trimmedUsername,
          email: trimmedEmail,
        });

        updateUserInState(addAvatarCacheBust(updatedUser));
        await refreshConversations(accessToken);
        setIsProfileSettingsOpen(false);
      } catch (error) {
        if (error instanceof Error) {
          setProfileError(error.message);
        } else {
          setProfileError("Failed to update profile.");
        }
      } finally {
        setIsProfileUpdating(false);
      }
    },
    [
      accessToken,
      refreshConversations,
      setIsProfileSettingsOpen,
      setIsProfileUpdating,
      setProfileError,
      updateUserInState,
    ]
  );

  const handleLoadBlockedUsers = useCallback(async () => {
    if (!accessToken) {
      return;
    }

    setIsBlockedUsersLoading(true);
    setBlockedUsersError("");

    try {
      const users = await getBlockedUsers(accessToken);
      setBlockedUsers(users);
    } catch {
      setBlockedUsers([]);
      setBlockedUsersError("Failed to load blocked users.");
    } finally {
      setIsBlockedUsersLoading(false);
    }
  }, [
    accessToken,
    setBlockedUsers,
    setBlockedUsersError,
    setIsBlockedUsersLoading,
  ]);

  const handleUnblockBlockedUser = useCallback(
    async (user: User) => {
      if (!accessToken) {
        return;
      }

      setIsUnblockingUserId(user.id);
      setBlockedUsersError("");

      try {
        const response = await unblockUser(accessToken, user.id);

        updateUserInState(response.user);
        setBlockedUsers((previousUsers) =>
          previousUsers.filter((blockedUser) => blockedUser.id !== user.id)
        );

        await refreshConversations(accessToken);
      } catch (error) {
        setBlockedUsersError(
          error instanceof Error ? error.message : "Failed to unblock user."
        );
      } finally {
        setIsUnblockingUserId(null);
      }
    },
    [
      accessToken,
      refreshConversations,
      setBlockedUsers,
      setBlockedUsersError,
      setIsUnblockingUserId,
      updateUserInState,
    ]
  );

  const handleOpenContactNicknameModal = useCallback(
    (conversation: Conversation) => {
      if (!currentUser) {
        return;
      }

      const user = getOtherParticipant(conversation, currentUser);

      if (!user) {
        return;
      }

      setContactNicknameUser(user);
      setContactNicknameError("");
    },
    [currentUser, setContactNicknameError, setContactNicknameUser]
  );

  const handleUpdateContactNickname = useCallback(
    async (nickname: string) => {
      if (!accessToken || !contactNicknameUser) {
        return;
      }

      setIsContactNicknameUpdating(true);
      setContactNicknameError("");

      try {
        const response = await updateContactNickname(
          accessToken,
          contactNicknameUser.id,
          nickname.trim()
        );

        updateUserInState(response.user);
        await refreshConversations(accessToken);
        setContactNicknameUser(null);
      } catch (error) {
        if (error instanceof Error) {
          setContactNicknameError(error.message);
        } else {
          setContactNicknameError("Failed to update contact name.");
        }
      } finally {
        setIsContactNicknameUpdating(false);
      }
    },
    [
      accessToken,
      contactNicknameUser,
      refreshConversations,
      setContactNicknameError,
      setContactNicknameUser,
      setIsContactNicknameUpdating,
      updateUserInState,
    ]
  );

  return {
    handleCurrentUserAvatarChange,
    handleDeleteCurrentUserAvatar,
    handleUpdateCurrentUserProfile,
    handleLoadBlockedUsers,
    handleUnblockBlockedUser,
    handleOpenContactNicknameModal,
    handleUpdateContactNickname,
  };
}
