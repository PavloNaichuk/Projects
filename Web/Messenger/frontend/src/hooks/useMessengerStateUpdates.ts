import { useCallback, type Dispatch, type SetStateAction } from "react";

import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";
import { sortConversationsByUpdatedAt } from "../utils/chat";

type UseMessengerStateUpdatesParams = {
  editingMessageId: number | null;
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
};

export function useMessengerStateUpdates({
  editingMessageId,
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
}: UseMessengerStateUpdatesParams) {
  const markConversationReadInState = useCallback(
    (conversationId: number) => {
      setConversations((previousConversations) =>
        previousConversations.map((conversation) =>
          conversation.id === conversationId
            ? {
                ...conversation,
                unread_count: 0,
                is_marked_unread: false,
              }
            : conversation
        )
      );

      setSelectedConversation((previousConversation) =>
        previousConversation?.id === conversationId
          ? {
              ...previousConversation,
              unread_count: 0,
              is_marked_unread: false,
            }
          : previousConversation
      );
    },
    [setConversations, setSelectedConversation]
  );

  const addMessageToState = useCallback(
    (receivedMessage: Message) => {
      setMessages((previousMessages) => {
        const alreadyExists = previousMessages.some(
          (message) => message.id === receivedMessage.id
        );

        if (alreadyExists) {
          return previousMessages;
        }

        return [...previousMessages, receivedMessage];
      });

      setConversations((previousConversations) =>
        sortConversationsByUpdatedAt(
          previousConversations.map((conversation) =>
            conversation.id === receivedMessage.conversation
              ? {
                  ...conversation,
                  last_message: receivedMessage,
                  updated_at: receivedMessage.created_at,
                }
              : conversation
          )
        )
      );

      setSelectedConversation((previousConversation) =>
        previousConversation?.id === receivedMessage.conversation
          ? {
              ...previousConversation,
              last_message: receivedMessage,
              updated_at: receivedMessage.created_at,
              unread_count: 0,
              is_marked_unread: false,
            }
          : previousConversation
      );
    },
    [setConversations, setMessages, setSelectedConversation]
  );

  const removeConversationFromState = useCallback(
    (conversationId: number) => {
      setConversations((previousConversations) =>
        previousConversations.filter(
          (conversation) => conversation.id !== conversationId
        )
      );

      setSelectedConversation((previousConversation) => {
        if (previousConversation?.id !== conversationId) {
          return previousConversation;
        }

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

        return null;
      });
    },
    [
      setConversations,
      setEditingMessageId,
      setEditingMessageText,
      setForwardingMessage,
      setHasMoreMessages,
      setIsMessageSearchActive,
      setIsOlderMessagesLoading,
      setIsSearchingMessages,
      setMessageError,
      setMessages,
      setMessageSearchQuery,
      setReplyToMessage,
      setSelectedAttachment,
      setSelectedConversation,
      setTypingUser,
    ]
  );

  const updateConversationInState = useCallback(
    (updatedConversation: Conversation) => {
      setConversations((previousConversations) =>
        sortConversationsByUpdatedAt(
          previousConversations.map((conversation) =>
            conversation.id === updatedConversation.id
              ? updatedConversation
              : conversation
          )
        )
      );

      setSelectedConversation((previousConversation) =>
        previousConversation?.id === updatedConversation.id
          ? updatedConversation
          : previousConversation
      );
    },
    [setConversations, setSelectedConversation]
  );

  const removeMessageFromState = useCallback(
    (messageId: number) => {
      setMessages((previousMessages) =>
        previousMessages.filter((message) => message.id !== messageId)
      );

      setEditingMessageId((previousEditingMessageId) =>
        previousEditingMessageId === messageId ? null : previousEditingMessageId
      );

      setEditingMessageText((previousEditingMessageText) => {
        if (editingMessageId === messageId) {
          return "";
        }

        return previousEditingMessageText;
      });

      setReplyToMessage((previousReplyToMessage) =>
        previousReplyToMessage?.id === messageId ? null : previousReplyToMessage
      );

      setForwardingMessage((previousForwardingMessage) =>
        previousForwardingMessage?.id === messageId
          ? null
          : previousForwardingMessage
      );
    },
    [
      editingMessageId,
      setEditingMessageId,
      setEditingMessageText,
      setForwardingMessage,
      setMessages,
      setReplyToMessage,
    ]
  );

  const updateMessageInState = useCallback(
    (updatedMessage: Message) => {
      setMessages((previousMessages) =>
        previousMessages.map((message) =>
          message.id === updatedMessage.id ? updatedMessage : message
        )
      );

      setConversations((previousConversations) =>
        sortConversationsByUpdatedAt(
          previousConversations.map((conversation) =>
            conversation.last_message?.id === updatedMessage.id
              ? {
                  ...conversation,
                  last_message: updatedMessage,
                  updated_at: updatedMessage.updated_at,
                }
              : conversation
          )
        )
      );

      setSelectedConversation((previousConversation) =>
        previousConversation?.last_message?.id === updatedMessage.id
          ? {
              ...previousConversation,
              last_message: updatedMessage,
              updated_at: updatedMessage.updated_at,
            }
          : previousConversation
      );
    },
    [setConversations, setMessages, setSelectedConversation]
  );

  const markMessagesAsDeliveredInState = useCallback(
    (messageIds: number[]) => {
      const deliveredAt = new Date().toISOString();

      function markMessageAsDelivered(message: Message): Message {
        if (!messageIds.includes(message.id)) {
          return message;
        }

        return {
          ...message,
          is_delivered: true,
          delivered_at: message.delivered_at ?? deliveredAt,
        };
      }

      setMessages((previousMessages) =>
        previousMessages.map((message) => markMessageAsDelivered(message))
      );

      setConversations((previousConversations) =>
        previousConversations.map((conversation) => ({
          ...conversation,
          last_message: conversation.last_message
            ? markMessageAsDelivered(conversation.last_message)
            : null,
        }))
      );

      setSelectedConversation((previousConversation) =>
        previousConversation
          ? {
              ...previousConversation,
              last_message: previousConversation.last_message
                ? markMessageAsDelivered(previousConversation.last_message)
                : null,
            }
          : previousConversation
      );
    },
    [setConversations, setMessages, setSelectedConversation]
  );

  return {
    markConversationReadInState,
    addMessageToState,
    removeConversationFromState,
    updateConversationInState,
    removeMessageFromState,
    updateMessageInState,
    markMessagesAsDeliveredInState,
  };
}
