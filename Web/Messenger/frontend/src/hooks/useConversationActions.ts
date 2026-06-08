import {
  useCallback,
  type Dispatch,
  type SetStateAction,
} from "react";

import type { User } from "../api/auth";
import {
  clearConversationHistory,
  createConversation,
  deleteConversation,
  getConversations,
  markConversationAsUnread,
  muteConversation,
  pinConversation,
  type Conversation,
  type DeleteConversationMode,
  type Message,
} from "../api/conversations";
import { sortConversationsByUpdatedAt } from "../utils/chat";

type UseConversationActionsParams = {
  accessToken: string | null;
  selectedConversation: Conversation | null;
  loadMessages: (
    token: string,
    conversationId: number,
    shouldMarkAsRead?: boolean
  ) => Promise<void>;
  removeConversationFromState: (conversationId: number) => void;
  updateConversationInState: (conversation: Conversation) => void;
  setConversations: Dispatch<SetStateAction<Conversation[]>>;
  setSelectedConversation: Dispatch<SetStateAction<Conversation | null>>;
  setMessages: Dispatch<SetStateAction<Message[]>>;
  setHasMoreMessages: Dispatch<SetStateAction<boolean>>;
  setIsOlderMessagesLoading: Dispatch<SetStateAction<boolean>>;
  setEditingMessageId: Dispatch<SetStateAction<number | null>>;
  setEditingMessageText: Dispatch<SetStateAction<string>>;
  setTypingUser: Dispatch<SetStateAction<User | null>>;
  setMessageError: Dispatch<SetStateAction<string>>;
  setMessageSearchQuery: Dispatch<SetStateAction<string>>;
  setIsMessageSearchActive: Dispatch<SetStateAction<boolean>>;
  setIsSearchingMessages: Dispatch<SetStateAction<boolean>>;
  setSelectedAttachment: Dispatch<SetStateAction<File | null>>;
  setReplyToMessage: Dispatch<SetStateAction<Message | null>>;
  setForwardingMessage: Dispatch<SetStateAction<Message | null>>;
  setUserSearchQuery: Dispatch<SetStateAction<string>>;
  setSearchResults: Dispatch<SetStateAction<User[]>>;
  setUserSearchError: Dispatch<SetStateAction<string>>;
  setIsDeletingConversationId: Dispatch<SetStateAction<number | null>>;
};

export function useConversationActions({
  accessToken,
  selectedConversation,
  loadMessages,
  removeConversationFromState,
  updateConversationInState,
  setConversations,
  setSelectedConversation,
  setMessages,
  setHasMoreMessages,
  setIsOlderMessagesLoading,
  setEditingMessageId,
  setEditingMessageText,
  setTypingUser,
  setMessageError,
  setMessageSearchQuery,
  setIsMessageSearchActive,
  setIsSearchingMessages,
  setSelectedAttachment,
  setReplyToMessage,
  setForwardingMessage,
  setUserSearchQuery,
  setSearchResults,
  setUserSearchError,
  setIsDeletingConversationId,
}: UseConversationActionsParams) {
  const refreshConversations = useCallback(
    async (token: string) => {
      try {
        const conversationsData = await getConversations(token);
        const sortedConversations =
          sortConversationsByUpdatedAt(conversationsData);

        setConversations(sortedConversations);

        setSelectedConversation((previousConversation) => {
          if (!previousConversation) {
            return previousConversation;
          }

          return (
            sortedConversations.find(
              (conversation) => conversation.id === previousConversation.id
            ) ?? previousConversation
          );
        });
      } catch {
        console.error("Failed to refresh conversations.");
      }
    },
    [setConversations, setSelectedConversation]
  );

  const handleSelectConversation = useCallback(
    async (conversation: Conversation) => {
      if (!accessToken) {
        return;
      }

      setSelectedConversation(conversation);
      setEditingMessageId(null);
      setEditingMessageText("");
      setTypingUser(null);
      setMessageError("");

      setMessageSearchQuery("");
      setIsMessageSearchActive(false);
      setIsSearchingMessages(false);
      setSelectedAttachment(null);
      setReplyToMessage(null);

      await loadMessages(accessToken, conversation.id);
      await refreshConversations(accessToken);
    },
    [
      accessToken,
      loadMessages,
      refreshConversations,
      setEditingMessageId,
      setEditingMessageText,
      setIsMessageSearchActive,
      setIsSearchingMessages,
      setMessageError,
      setMessageSearchQuery,
      setReplyToMessage,
      setSelectedAttachment,
      setSelectedConversation,
      setTypingUser,
    ]
  );

  const handleStartConversation = useCallback(
    async (user: User) => {
      if (!accessToken) {
        return;
      }

      setUserSearchError("");

      try {
        const conversation = await createConversation(accessToken, user.id);

        setConversations((previousConversations) => {
          const alreadyExists = previousConversations.some(
            (item) => item.id === conversation.id
          );

          if (alreadyExists) {
            return sortConversationsByUpdatedAt(
              previousConversations.map((item) =>
                item.id === conversation.id ? conversation : item
              )
            );
          }

          return sortConversationsByUpdatedAt([
            conversation,
            ...previousConversations,
          ]);
        });

        setSelectedConversation(conversation);

        setMessageSearchQuery("");
        setIsMessageSearchActive(false);
        setIsSearchingMessages(false);
        setSelectedAttachment(null);

        await loadMessages(accessToken, conversation.id);

        setUserSearchQuery("");
        setSearchResults([]);
      } catch {
        setUserSearchError("Failed to create conversation.");
      }
    },
    [
      accessToken,
      loadMessages,
      setConversations,
      setIsMessageSearchActive,
      setIsSearchingMessages,
      setMessageSearchQuery,
      setSearchResults,
      setSelectedAttachment,
      setSelectedConversation,
      setUserSearchError,
      setUserSearchQuery,
    ]
  );

  const handleDeleteConversation = useCallback(
    async (conversation: Conversation, mode: DeleteConversationMode) => {
      if (!accessToken) {
        return;
      }

      setIsDeletingConversationId(conversation.id);
      setUserSearchError("");

      try {
        await deleteConversation(accessToken, conversation.id, mode);
        removeConversationFromState(conversation.id);
      } catch {
        setUserSearchError("Failed to delete conversation.");
      } finally {
        setIsDeletingConversationId(null);
      }
    },
    [
      accessToken,
      removeConversationFromState,
      setIsDeletingConversationId,
      setUserSearchError,
    ]
  );

  const handleMuteConversation = useCallback(
    async (conversation: Conversation) => {
      if (!accessToken) {
        return;
      }

      setUserSearchError("");

      try {
        const response = await muteConversation(
          accessToken,
          conversation.id,
          !conversation.is_muted
        );

        updateConversationInState(response.conversation);
      } catch {
        setUserSearchError("Failed to update mute status.");
      }
    },
    [accessToken, setUserSearchError, updateConversationInState]
  );

  const handlePinConversation = useCallback(
    async (conversation: Conversation) => {
      if (!accessToken) {
        return;
      }

      setUserSearchError("");

      try {
        const response = await pinConversation(
          accessToken,
          conversation.id,
          !conversation.is_pinned
        );

        updateConversationInState(response.conversation);
      } catch {
        setUserSearchError("Failed to update pin status.");
      }
    },
    [accessToken, setUserSearchError, updateConversationInState]
  );

  const handleMarkConversationAsUnread = useCallback(
    async (conversation: Conversation) => {
      if (!accessToken) {
        return;
      }

      setUserSearchError("");

      try {
        const response = await markConversationAsUnread(
          accessToken,
          conversation.id
        );

        updateConversationInState(response.conversation);
      } catch {
        setUserSearchError("Failed to mark conversation as unread.");
      }
    },
    [accessToken, setUserSearchError, updateConversationInState]
  );

  const handleClearConversationHistory = useCallback(
    async (conversation: Conversation) => {
      if (!accessToken) {
        return;
      }

      setIsDeletingConversationId(conversation.id);
      setUserSearchError("");

      try {
        const response = await clearConversationHistory(
          accessToken,
          conversation.id
        );

        updateConversationInState(response.conversation);

        if (selectedConversation?.id === conversation.id) {
          setMessages([]);
          setHasMoreMessages(false);
          setIsOlderMessagesLoading(false);
          setEditingMessageId(null);
          setEditingMessageText("");
          setTypingUser(null);
          setMessageError("");
          setMessageSearchQuery("");
          setIsMessageSearchActive(false);
          setIsSearchingMessages(false);
          setSelectedAttachment(null);
          setReplyToMessage(null);
          setForwardingMessage(null);
        }

        await refreshConversations(accessToken);
      } catch {
        setUserSearchError("Failed to clear chat history.");
      } finally {
        setIsDeletingConversationId(null);
      }
    },
    [
      accessToken,
      refreshConversations,
      selectedConversation,
      setEditingMessageId,
      setEditingMessageText,
      setForwardingMessage,
      setHasMoreMessages,
      setIsDeletingConversationId,
      setIsMessageSearchActive,
      setIsOlderMessagesLoading,
      setIsSearchingMessages,
      setMessageError,
      setMessages,
      setMessageSearchQuery,
      setReplyToMessage,
      setSelectedAttachment,
      setTypingUser,
      setUserSearchError,
      updateConversationInState,
    ]
  );

  return {
    refreshConversations,
    handleSelectConversation,
    handleStartConversation,
    handleDeleteConversation,
    handleMuteConversation,
    handlePinConversation,
    handleMarkConversationAsUnread,
    handleClearConversationHistory,
  };
}
