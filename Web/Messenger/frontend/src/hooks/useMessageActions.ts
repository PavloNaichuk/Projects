import {
  useCallback,
  type ChangeEvent,
  type Dispatch,
  type FormEvent,
  type KeyboardEvent,
  type SetStateAction,
} from "react";

import {
  createMessageWithAttachment,
  deleteMessage,
  editMessage,
  forwardMessage,
  getConversationMessagesPage,
  removeMessageAttachment,
  toggleMessageReaction,
  type Conversation,
  type DeleteMessageMode,
  type Message,
} from "../api/conversations";
import type { User } from "../api/auth";

type UseMessageActionsParams = {
  accessToken: string | null;
  selectedConversation: Conversation | null;
  selectedConversationUser: User | null;
  messageSearchQuery: string;
  newMessage: string;
  selectedAttachment: File | null;
  replyToMessage: Message | null;
  forwardingMessage: Message | null;
  editingMessageId: number | null;
  editingMessageText: string;
  loadMessages: (
    token: string,
    conversationId: number,
    shouldMarkAsRead?: boolean
  ) => Promise<void>;
  refreshConversations: (token: string) => Promise<void>;
  addMessageToState: (message: Message) => void;
  removeMessageFromState: (messageId: number) => void;
  updateMessageInState: (message: Message) => void;
  getChatSocket: () => WebSocket | null;
  sendTypingStatus: (isTyping: boolean) => void;
  clearTypingTimeout: () => void;
  notifyTypingActivity: () => void;
  scrollToBottomOnNextRender: () => void;
  setMessages: Dispatch<SetStateAction<Message[]>>;
  setHasMoreMessages: Dispatch<SetStateAction<boolean>>;
  setNewMessage: Dispatch<SetStateAction<string>>;
  setSelectedAttachment: Dispatch<SetStateAction<File | null>>;
  setReplyToMessage: Dispatch<SetStateAction<Message | null>>;
  setForwardingMessage: Dispatch<SetStateAction<Message | null>>;
  setMessageError: Dispatch<SetStateAction<string>>;
  setIsForwardingMessage: Dispatch<SetStateAction<boolean>>;
  setIsSending: Dispatch<SetStateAction<boolean>>;
  setMessageSearchQuery: Dispatch<SetStateAction<string>>;
  setIsMessageSearchActive: Dispatch<SetStateAction<boolean>>;
  setIsSearchingMessages: Dispatch<SetStateAction<boolean>>;
  setEditingMessageId: Dispatch<SetStateAction<number | null>>;
  setEditingMessageText: Dispatch<SetStateAction<string>>;
  setIsEditingMessage: Dispatch<SetStateAction<boolean>>;
  setIsDeletingMessageId: Dispatch<SetStateAction<number | null>>;
};

export function useMessageActions({
  accessToken,
  selectedConversation,
  selectedConversationUser,
  messageSearchQuery,
  newMessage,
  selectedAttachment,
  replyToMessage,
  forwardingMessage,
  editingMessageId,
  editingMessageText,
  loadMessages,
  refreshConversations,
  addMessageToState,
  removeMessageFromState,
  updateMessageInState,
  getChatSocket,
  sendTypingStatus,
  clearTypingTimeout,
  notifyTypingActivity,
  scrollToBottomOnNextRender,
  setMessages,
  setHasMoreMessages,
  setNewMessage,
  setSelectedAttachment,
  setReplyToMessage,
  setForwardingMessage,
  setMessageError,
  setIsForwardingMessage,
  setIsSending,
  setMessageSearchQuery,
  setIsMessageSearchActive,
  setIsSearchingMessages,
  setEditingMessageId,
  setEditingMessageText,
  setIsEditingMessage,
  setIsDeletingMessageId,
}: UseMessageActionsParams) {
  const handleClearMessageSearch = useCallback(async () => {
    if (!accessToken || !selectedConversation) {
      setMessageSearchQuery("");
      setIsMessageSearchActive(false);
      return;
    }

    setMessageSearchQuery("");
    setIsMessageSearchActive(false);
    setIsSearchingMessages(false);

    await loadMessages(accessToken, selectedConversation.id);
  }, [
    accessToken,
    loadMessages,
    selectedConversation,
    setIsMessageSearchActive,
    setIsSearchingMessages,
    setMessageSearchQuery,
  ]);

  const handleSearchMessages = useCallback(
    async (event: FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (!accessToken || !selectedConversation) {
        return;
      }

      const searchQuery = messageSearchQuery.trim();

      if (!searchQuery) {
        await handleClearMessageSearch();
        return;
      }

      setIsSearchingMessages(true);
      setIsMessageSearchActive(true);
      setMessageError("");
      setHasMoreMessages(false);
      scrollToBottomOnNextRender();

      try {
        const messagesPage = await getConversationMessagesPage(
          accessToken,
          selectedConversation.id,
          undefined,
          50,
          searchQuery
        );

        setMessages(messagesPage.results);
        setHasMoreMessages(false);
      } catch {
        setMessageError("Failed to search messages.");
      } finally {
        setIsSearchingMessages(false);
      }
    },
    [
      accessToken,
      handleClearMessageSearch,
      messageSearchQuery,
      scrollToBottomOnNextRender,
      selectedConversation,
      setHasMoreMessages,
      setIsMessageSearchActive,
      setIsSearchingMessages,
      setMessageError,
      setMessages,
    ]
  );

  const handleNewMessageChange = useCallback(
    (value: string) => {
      setNewMessage(value);

      if (!selectedConversation) {
        return;
      }

      notifyTypingActivity();
    },
    [notifyTypingActivity, selectedConversation, setNewMessage]
  );

  const handleAttachmentChange = useCallback(
    (event: ChangeEvent<HTMLInputElement>) => {
      const file = event.target.files?.[0];

      if (!file) {
        return;
      }

      setSelectedAttachment(file);
      setMessageError("");
    },
    [setMessageError, setSelectedAttachment]
  );

  const handleRemoveAttachment = useCallback(() => {
    setSelectedAttachment(null);
  }, [setSelectedAttachment]);

  const handleStartReplyMessage = useCallback(
    (message: Message) => {
      if (message.is_deleted) {
        return;
      }

      setReplyToMessage(message);
      setMessageError("");
    },
    [setMessageError, setReplyToMessage]
  );

  const handleCancelReplyMessage = useCallback(() => {
    setReplyToMessage(null);
  }, [setReplyToMessage]);

  const handleStartForwardMessage = useCallback(
    (message: Message) => {
      if (message.is_deleted) {
        return;
      }

      setForwardingMessage(message);
      setMessageError("");
    },
    [setForwardingMessage, setMessageError]
  );

  const handleCancelForwardMessage = useCallback(() => {
    setForwardingMessage(null);
  }, [setForwardingMessage]);

  const handleForwardMessageToConversation = useCallback(
    async (conversationId: number) => {
      if (!accessToken || !forwardingMessage) {
        return;
      }

      setIsForwardingMessage(true);
      setMessageError("");

      try {
        const forwardedMessage = await forwardMessage(
          accessToken,
          forwardingMessage.id,
          conversationId
        );

        if (selectedConversation?.id === conversationId) {
          addMessageToState(forwardedMessage);
        }

        await refreshConversations(accessToken);
        setForwardingMessage(null);
      } catch {
        setMessageError("Failed to forward message.");
      } finally {
        setIsForwardingMessage(false);
      }
    },
    [
      accessToken,
      addMessageToState,
      forwardingMessage,
      refreshConversations,
      selectedConversation,
      setForwardingMessage,
      setIsForwardingMessage,
      setMessageError,
    ]
  );

  const handleSendMessage = useCallback(
    async (event: FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (!accessToken || !selectedConversation) {
        return;
      }

      if (selectedConversationUser?.is_blocked_by_me) {
        setMessageError("You blocked this user. Unblock them to send messages.");
        return;
      }

      if (selectedConversationUser?.has_blocked_me) {
        setMessageError("You cannot send messages to this user.");
        return;
      }

      const text = newMessage.trim();

      if (!text && !selectedAttachment) {
        setMessageError("Message text or attachment is required.");
        return;
      }

      const socket = getChatSocket();
      const replyToMessageId = replyToMessage?.id ?? null;

      if (
        !selectedAttachment &&
        (!socket || socket.readyState !== WebSocket.OPEN)
      ) {
        setMessageError("WebSocket is not connected.");
        return;
      }

      setMessageError("");
      setIsSending(true);

      try {
        if (selectedAttachment) {
          const createdMessage = await createMessageWithAttachment(
            accessToken,
            selectedConversation.id,
            text,
            selectedAttachment,
            replyToMessageId
          );

          addMessageToState(createdMessage);
        } else if (socket) {
          socket.send(
            JSON.stringify({
              text,
              reply_to: replyToMessageId,
            })
          );
        }

        sendTypingStatus(false);
        clearTypingTimeout();

        setNewMessage("");
        setSelectedAttachment(null);
        setReplyToMessage(null);
      } catch {
        setMessageError("Failed to send message.");
      } finally {
        setIsSending(false);
      }
    },
    [
      accessToken,
      addMessageToState,
      clearTypingTimeout,
      getChatSocket,
      newMessage,
      replyToMessage,
      selectedAttachment,
      selectedConversation,
      selectedConversationUser,
      sendTypingStatus,
      setIsSending,
      setMessageError,
      setNewMessage,
      setReplyToMessage,
      setSelectedAttachment,
    ]
  );

  const handleMessageKeyDown = useCallback(
    (event: KeyboardEvent<HTMLTextAreaElement>) => {
      if (event.key === "Enter" && !event.shiftKey) {
        event.preventDefault();

        const form = event.currentTarget.form;

        if (form) {
          form.requestSubmit();
        }
      }
    },
    []
  );

  const handleStartEditMessage = useCallback(
    (message: Message) => {
      setEditingMessageId(message.id);
      setEditingMessageText(message.text);
      setMessageError("");
    },
    [setEditingMessageId, setEditingMessageText, setMessageError]
  );

  const handleCancelEditMessage = useCallback(() => {
    setEditingMessageId(null);
    setEditingMessageText("");
  }, [setEditingMessageId, setEditingMessageText]);

  const handleSaveEditedMessage = useCallback(
    async (messageId: number) => {
      if (!accessToken) {
        return;
      }

      const text = editingMessageText.trim();

      if (!text) {
        setMessageError("Message text cannot be empty.");
        return;
      }

      setIsEditingMessage(true);
      setMessageError("");

      try {
        const updatedMessage = await editMessage(accessToken, messageId, text);
        updateMessageInState(updatedMessage);
        setEditingMessageId(null);
        setEditingMessageText("");
      } catch {
        setMessageError("Failed to edit message.");
      } finally {
        setIsEditingMessage(false);
      }
    },
    [
      accessToken,
      editingMessageText,
      setEditingMessageId,
      setEditingMessageText,
      setIsEditingMessage,
      setMessageError,
      updateMessageInState,
    ]
  );

  const handleDeleteMessage = useCallback(
    async (messageId: number, mode: DeleteMessageMode) => {
      if (!accessToken) {
        return;
      }

      setIsDeletingMessageId(messageId);
      setMessageError("");

      try {
        const response = await deleteMessage(accessToken, messageId, mode);

        if (response.mode === "for_me") {
          removeMessageFromState(messageId);
          await refreshConversations(accessToken);
          return;
        }

        updateMessageInState(response.message);

        if (editingMessageId === messageId) {
          setEditingMessageId(null);
          setEditingMessageText("");
        }

        await refreshConversations(accessToken);
      } catch {
        setMessageError("Failed to delete message.");
      } finally {
        setIsDeletingMessageId(null);
      }
    },
    [
      accessToken,
      editingMessageId,
      refreshConversations,
      removeMessageFromState,
      setEditingMessageId,
      setEditingMessageText,
      setIsDeletingMessageId,
      setMessageError,
      updateMessageInState,
    ]
  );

  const handleRemoveMessageAttachment = useCallback(
    async (messageId: number) => {
      if (!accessToken) {
        return;
      }

      setIsDeletingMessageId(messageId);
      setMessageError("");

      try {
        const updatedMessage = await removeMessageAttachment(
          accessToken,
          messageId
        );
        updateMessageInState(updatedMessage);

        if (editingMessageId === messageId && updatedMessage.is_deleted) {
          setEditingMessageId(null);
          setEditingMessageText("");
        }
      } catch {
        setMessageError("Failed to delete attachment.");
      } finally {
        setIsDeletingMessageId(null);
      }
    },
    [
      accessToken,
      editingMessageId,
      setEditingMessageId,
      setEditingMessageText,
      setIsDeletingMessageId,
      setMessageError,
      updateMessageInState,
    ]
  );

  const handleToggleMessageReaction = useCallback(
    async (messageId: number, emoji: string) => {
      if (!accessToken) {
        return;
      }

      setMessageError("");

      try {
        const response = await toggleMessageReaction(
          accessToken,
          messageId,
          emoji
        );
        updateMessageInState(response.message);
      } catch {
        setMessageError("Failed to update reaction.");
      }
    },
    [accessToken, setMessageError, updateMessageInState]
  );

  return {
    handleSearchMessages,
    handleClearMessageSearch,
    handleNewMessageChange,
    handleAttachmentChange,
    handleRemoveAttachment,
    handleStartReplyMessage,
    handleCancelReplyMessage,
    handleStartForwardMessage,
    handleCancelForwardMessage,
    handleForwardMessageToConversation,
    handleSendMessage,
    handleMessageKeyDown,
    handleStartEditMessage,
    handleCancelEditMessage,
    handleSaveEditedMessage,
    handleDeleteMessage,
    handleRemoveMessageAttachment,
    handleToggleMessageReaction,
  };
}
