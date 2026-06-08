import {
  useCallback,
  useEffect,
  useRef,
  type Dispatch,
  type SetStateAction,
} from "react";

import { WS_BASE_URL } from "../api/config";
import type { User } from "../api/auth";
import {
  markConversationAsRead,
  type Message,
} from "../api/conversations";
import { addAvatarCacheBust } from "../utils/users";

type UseMessengerSocketsParams = {
  accessToken: string | null;
  currentUserId: number | null;
  selectedConversationId: number | null;
  playIncomingMessageSound: (message: Message) => void;
  refreshConversations: (token: string) => Promise<void>;
  removeConversationFromState: (conversationId: number) => void;
  updateUserInState: (user: User) => void;
  updateUserLastSeenInState: (user: User) => void;
  addMessageToState: (message: Message) => void;
  markConversationReadInState: (conversationId: number) => void;
  updateMessageInState: (message: Message) => void;
  markMessagesAsDeliveredInState: (messageIds: number[]) => void;
  setMessages: Dispatch<SetStateAction<Message[]>>;
  setMessageError: Dispatch<SetStateAction<string>>;
  setOnlineUserIds: Dispatch<SetStateAction<number[]>>;
  setTypingUser: Dispatch<SetStateAction<User | null>>;
};

type MessengerSocketHandlers = Omit<
  UseMessengerSocketsParams,
  "accessToken" | "currentUserId" | "selectedConversationId"
>;

export function useMessengerSockets({
  accessToken,
  currentUserId,
  selectedConversationId,
  playIncomingMessageSound,
  refreshConversations,
  removeConversationFromState,
  updateUserInState,
  updateUserLastSeenInState,
  addMessageToState,
  markConversationReadInState,
  updateMessageInState,
  markMessagesAsDeliveredInState,
  setMessages,
  setMessageError,
  setOnlineUserIds,
  setTypingUser,
}: UseMessengerSocketsParams) {
  const chatSocketRef = useRef<WebSocket | null>(null);
  const notificationSocketRef = useRef<WebSocket | null>(null);
  const typingTimeoutRef = useRef<number | null>(null);

  const handlersRef = useRef<MessengerSocketHandlers>({
    playIncomingMessageSound,
    refreshConversations,
    removeConversationFromState,
    updateUserInState,
    updateUserLastSeenInState,
    addMessageToState,
    markConversationReadInState,
    updateMessageInState,
    markMessagesAsDeliveredInState,
    setMessages,
    setMessageError,
    setOnlineUserIds,
    setTypingUser,
  });

  useEffect(() => {
    handlersRef.current = {
      playIncomingMessageSound,
      refreshConversations,
      removeConversationFromState,
      updateUserInState,
      updateUserLastSeenInState,
      addMessageToState,
      markConversationReadInState,
      updateMessageInState,
      markMessagesAsDeliveredInState,
      setMessages,
      setMessageError,
      setOnlineUserIds,
      setTypingUser,
    };
  }, [
    playIncomingMessageSound,
    refreshConversations,
    removeConversationFromState,
    updateUserInState,
    updateUserLastSeenInState,
    addMessageToState,
    markConversationReadInState,
    updateMessageInState,
    markMessagesAsDeliveredInState,
    setMessages,
    setMessageError,
    setOnlineUserIds,
    setTypingUser,
  ]);

  const clearTypingTimeout = useCallback(() => {
    if (!typingTimeoutRef.current) {
      return;
    }

    window.clearTimeout(typingTimeoutRef.current);
    typingTimeoutRef.current = null;
  }, []);

  const getChatSocket = useCallback(() => {
    return chatSocketRef.current;
  }, []);

  const closeSockets = useCallback(() => {
    clearTypingTimeout();

    if (chatSocketRef.current) {
      chatSocketRef.current.close();
      chatSocketRef.current = null;
    }

    if (notificationSocketRef.current) {
      notificationSocketRef.current.close();
      notificationSocketRef.current = null;
    }
  }, [clearTypingTimeout]);

  const sendReadStatus = useCallback(() => {
    const socket = chatSocketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "read",
      })
    );
  }, []);

  const sendTypingStatus = useCallback((isTyping: boolean) => {
    const socket = chatSocketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "typing",
        is_typing: isTyping,
      })
    );
  }, []);

  const notifyTypingActivity = useCallback(() => {
    sendTypingStatus(true);
    clearTypingTimeout();

    typingTimeoutRef.current = window.setTimeout(() => {
      sendTypingStatus(false);
    }, 1200);
  }, [clearTypingTimeout, sendTypingStatus]);

  useEffect(() => {
    if (!accessToken || !currentUserId) {
      return;
    }

    const notificationSocket = new WebSocket(
      `${WS_BASE_URL}/notifications/?token=${accessToken}`
    );

    notificationSocketRef.current = notificationSocket;

    notificationSocket.onmessage = async (event) => {
      const data = JSON.parse(event.data);
      const handlers = handlersRef.current;

      if (data.type === "sidebar_message") {
        const receivedMessage = data.message as Message | undefined;

        if (receivedMessage) {
          handlers.playIncomingMessageSound(receivedMessage);
        }

        await handlers.refreshConversations(accessToken);
        return;
      }

      if (data.type === "conversation_deleted") {
        handlers.removeConversationFromState(data.conversation_id);
        return;
      }

      if (data.type === "user_profile_updated") {
        const updatedUser = addAvatarCacheBust(data.user as User);

        handlers.updateUserInState(updatedUser);
        return;
      }

      if (data.type === "online_users") {
        handlers.setOnlineUserIds(data.user_ids);
        return;
      }

      if (data.type === "online_status") {
        const user = data.user as User;

        handlers.updateUserLastSeenInState(user);

        handlers.setOnlineUserIds((previousOnlineUserIds) => {
          if (data.is_online) {
            if (previousOnlineUserIds.includes(user.id)) {
              return previousOnlineUserIds;
            }

            return [...previousOnlineUserIds, user.id];
          }

          return previousOnlineUserIds.filter((userId) => userId !== user.id);
        });
      }
    };

    notificationSocket.onerror = () => {
      console.error("Notification WebSocket connection error.");
    };

    return () => {
      notificationSocket.close();

      if (notificationSocketRef.current === notificationSocket) {
        notificationSocketRef.current = null;
      }
    };
  }, [accessToken, currentUserId]);

  useEffect(() => {
    if (!accessToken || !selectedConversationId) {
      return;
    }

    const websocketUrl = `${WS_BASE_URL}/conversations/${selectedConversationId}/?token=${accessToken}`;
    const socket = new WebSocket(websocketUrl);

    chatSocketRef.current = socket;

    socket.onopen = () => {
      handlersRef.current.setMessageError("");
      sendReadStatus();
    };

    socket.onmessage = async (event) => {
      const data = JSON.parse(event.data);
      const handlers = handlersRef.current;

      if (data.type === "message") {
        const receivedMessage = data.message as Message;

        handlers.playIncomingMessageSound(receivedMessage);
        handlers.addMessageToState(receivedMessage);

        if (
          selectedConversationId === receivedMessage.conversation &&
          receivedMessage.sender.id !== currentUserId
        ) {
          try {
            await markConversationAsRead(accessToken, selectedConversationId);
            handlers.markConversationReadInState(selectedConversationId);
            sendReadStatus();
          } catch {
            console.error("Failed to mark WebSocket message as read.");
          }
        }

        return;
      }

      if (data.type === "message_updated") {
        const updatedMessage = data.message as Message;

        handlers.updateMessageInState(updatedMessage);
        return;
      }

      if (data.type === "delivered") {
        const messageIds = data.message_ids as number[];

        handlers.markMessagesAsDeliveredInState(messageIds);
        return;
      }

      if (data.type === "read") {
        const reader = data.user as User;

        if (reader.id === currentUserId) {
          return;
        }

        handlers.setMessages((previousMessages) =>
          previousMessages.map((message) =>
            message.sender.id === currentUserId && !message.is_deleted
              ? {
                  ...message,
                  is_delivered: true,
                  delivered_at: message.delivered_at ?? new Date().toISOString(),
                  is_read: true,
                }
              : message
          )
        );

        return;
      }

      if (data.type === "typing") {
        const receivedTypingUser = data.user as User;

        if (receivedTypingUser.id === currentUserId) {
          return;
        }

        if (data.is_typing) {
          handlers.setTypingUser(receivedTypingUser);
        } else {
          handlers.setTypingUser(null);
        }

        return;
      }

      if (data.type === "error") {
        handlers.setMessageError(data.detail);
      }
    };

    socket.onerror = () => {
      if (chatSocketRef.current === socket) {
        handlersRef.current.setMessageError("WebSocket connection error.");
      }

      console.error("Chat WebSocket connection error.");
    };

    return () => {
      clearTypingTimeout();
      handlersRef.current.setTypingUser(null);

      socket.close();

      if (chatSocketRef.current === socket) {
        chatSocketRef.current = null;
      }
    };
  }, [
    accessToken,
    clearTypingTimeout,
    currentUserId,
    selectedConversationId,
    sendReadStatus,
  ]);

  return {
    sendTypingStatus,
    notifyTypingActivity,
    clearTypingTimeout,
    closeSockets,
    getChatSocket,
  };
}
