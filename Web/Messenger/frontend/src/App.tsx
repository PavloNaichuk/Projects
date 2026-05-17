import {
  useCallback,
  useEffect,
  useLayoutEffect,
  useRef,
  useState,
  type ChangeEvent,
  type FormEvent,
  type KeyboardEvent,
} from "react";
import { WS_BASE_URL } from "./api/config";
import "./App.css";
import {
  deleteCurrentUserAvatar,
  getCurrentUser,
  login,
  logout,
  refreshAccessToken,
  register,
  updateCurrentUserAvatar,
  updateCurrentUserProfile,
  type User,
} from "./api/auth";
import {
  createConversation,
  createMessageWithAttachment,
  deleteConversation,
  deleteMessage,
  editMessage,
  forwardMessage,
  removeMessageAttachment,
  toggleMessageReaction,
  getConversationMessagesPage,
  getConversations,
  markConversationAsRead,
  muteConversation,
  type Conversation,
  type DeleteConversationMode,
  type Message,
} from "./api/conversations";
import { searchUsers } from "./api/users";
import AuthPage from "./components/AuthPage";
import ChatWindow from "./components/ChatWindow";
import Sidebar from "./components/Sidebar";
import ProfileSettingsModal from "./components/ProfileSettingsModal";
import {
  getConversationName,
  getOtherParticipant,
  sortConversationsByUpdatedAt,
} from "./utils/chat";

const MESSAGE_PAGE_SIZE = 20;

type ScrollBehavior = "bottom" | "preserve";

function getForwardPreviewText(message: Message) {
  if (message.is_deleted) {
    return "This message was deleted";
  }

  if (message.text.trim()) {
    return message.text;
  }

  if (message.attachment_name) {
    return message.attachment_is_image
      ? `Image: ${message.attachment_name}`
      : `File: ${message.attachment_name}`;
  }

  return "Message";
}

function addAvatarCacheBust(user: User): User {
  if (!user.avatar_url) {
    return user;
  }

  const separator = user.avatar_url.includes("?") ? "&" : "?";

  return {
    ...user,
    avatar_url: `${user.avatar_url}${separator}v=${Date.now()}`,
  };
}

function App() {
  const [currentUser, setCurrentUser] = useState<User | null>(null);
  const [accessToken, setAccessToken] = useState<string | null>(null);

  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] =
    useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);

  const socketRef = useRef<WebSocket | null>(null);
  const notificationSocketRef = useRef<WebSocket | null>(null);
  const messagesContainerRef = useRef<HTMLElement | null>(null);
  const messagesEndRef = useRef<HTMLDivElement | null>(null);
  const typingTimeoutRef = useRef<number | null>(null);
  const playedSoundMessageIdsRef = useRef<Set<number>>(new Set());
  const notificationAudioRef = useRef<HTMLAudioElement | null>(null);
  const scrollBehaviorRef = useRef<ScrollBehavior>("bottom");
  const previousScrollHeightRef = useRef(0);
  const previousScrollTopRef = useRef(0);
  const conversationsRef = useRef<Conversation[]>([]);

  const [authMode, setAuthMode] = useState<"login" | "register">("login");

  const [username, setUsername] = useState("pavlo");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [passwordConfirm, setPasswordConfirm] = useState("");

  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);
  const [isMessagesLoading, setIsMessagesLoading] = useState(false);
  const [isOlderMessagesLoading, setIsOlderMessagesLoading] = useState(false);
  const [hasMoreMessages, setHasMoreMessages] = useState(false);
  const [isAuthChecking, setIsAuthChecking] = useState(true);

  const [newMessage, setNewMessage] = useState("");
  const [selectedAttachment, setSelectedAttachment] = useState<File | null>(
    null
  );
  const [replyToMessage, setReplyToMessage] = useState<Message | null>(null);
  const [forwardingMessage, setForwardingMessage] = useState<Message | null>(null);
  const [messageError, setMessageError] = useState("");
  const [isForwardingMessage, setIsForwardingMessage] = useState(false);
  const [isSending, setIsSending] = useState(false);
  const [typingUser, setTypingUser] = useState<User | null>(null);
  const [onlineUserIds, setOnlineUserIds] = useState<number[]>([]);

  const [userSearchQuery, setUserSearchQuery] = useState("");
  const [searchResults, setSearchResults] = useState<User[]>([]);
  const [isSearchingUsers, setIsSearchingUsers] = useState(false);
  const [userSearchError, setUserSearchError] = useState("");
  const [avatarError, setAvatarError] = useState("");
  const [isAvatarUpdating, setIsAvatarUpdating] = useState(false);
  const [isProfileSettingsOpen, setIsProfileSettingsOpen] = useState(false);
  const [profileError, setProfileError] = useState("");
  const [isProfileUpdating, setIsProfileUpdating] = useState(false);

  const [messageSearchQuery, setMessageSearchQuery] = useState("");
  const [isMessageSearchActive, setIsMessageSearchActive] = useState(false);
  const [isSearchingMessages, setIsSearchingMessages] = useState(false);

  const [editingMessageId, setEditingMessageId] = useState<number | null>(null);
  const [editingMessageText, setEditingMessageText] = useState("");
  const [isEditingMessage, setIsEditingMessage] = useState(false);
  const [isDeletingMessageId, setIsDeletingMessageId] = useState<number | null>(
    null
  );
  const [isDeletingConversationId, setIsDeletingConversationId] = useState<
    number | null
  >(null);
  const currentUserId = currentUser?.id ?? null;
  const selectedConversationId = selectedConversation?.id ?? null;
  useEffect(() => {
    conversationsRef.current = conversations;
  }, [conversations]);

  function markConversationReadInState(conversationId: number) {
    setConversations((previousConversations) =>
      previousConversations.map((conversation) =>
        conversation.id === conversationId
          ? {
              ...conversation,
              unread_count: 0,
            }
          : conversation
      )
    );

    setSelectedConversation((previousConversation) =>
      previousConversation?.id === conversationId
        ? {
            ...previousConversation,
            unread_count: 0,
          }
        : previousConversation
    );
  }

  function addMessageToState(receivedMessage: Message) {
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
          }
        : previousConversation
    );
  }

  function updateUserInMessage(message: Message, updatedUser: User): Message {
    return {
      ...message,
      sender:
        message.sender.id === updatedUser.id ? updatedUser : message.sender,
      reply_to_message: message.reply_to_message
        ? {
            ...message.reply_to_message,
            sender:
              message.reply_to_message.sender.id === updatedUser.id
                ? updatedUser
                : message.reply_to_message.sender,
          }
        : null,
      forwarded_from_message: message.forwarded_from_message
        ? {
            ...message.forwarded_from_message,
            sender:
              message.forwarded_from_message.sender.id === updatedUser.id
                ? updatedUser
                : message.forwarded_from_message.sender,
          }
        : null,
      reactions: message.reactions.map((reaction) => ({
        ...reaction,
        users: reaction.users.map((user) =>
          user.id === updatedUser.id ? updatedUser : user
        ),
      })),
    };
  }

  function updateUserInState(updatedUser: User) {
    setCurrentUser((previousUser) =>
      previousUser?.id === updatedUser.id ? updatedUser : previousUser
    );

    setConversations((previousConversations) =>
      previousConversations.map((conversation) => ({
        ...conversation,
        participants: conversation.participants.map((participant) =>
          participant.user.id === updatedUser.id
            ? {
                ...participant,
                user: updatedUser,
              }
            : participant
        ),
        last_message: conversation.last_message
          ? updateUserInMessage(conversation.last_message, updatedUser)
          : null,
      }))
    );

    setSelectedConversation((previousConversation) =>
      previousConversation
        ? {
            ...previousConversation,
            participants: previousConversation.participants.map((participant) =>
              participant.user.id === updatedUser.id
                ? {
                    ...participant,
                    user: updatedUser,
                  }
                : participant
            ),
            last_message: previousConversation.last_message
              ? updateUserInMessage(previousConversation.last_message, updatedUser)
              : null,
          }
        : previousConversation
    );

    setMessages((previousMessages) =>
      previousMessages.map((message) => updateUserInMessage(message, updatedUser))
    );

    setSearchResults((previousSearchResults) =>
      previousSearchResults.map((user) =>
        user.id === updatedUser.id ? updatedUser : user
      )
    );
  }

  function updateUserLastSeenInMessage(message: Message, updatedUser: User): Message {
    return {
      ...message,
      sender:
        message.sender.id === updatedUser.id
          ? {
              ...message.sender,
              last_seen_at: updatedUser.last_seen_at,
            }
          : message.sender,
      reply_to_message: message.reply_to_message
        ? {
            ...message.reply_to_message,
            sender:
              message.reply_to_message.sender.id === updatedUser.id
                ? {
                    ...message.reply_to_message.sender,
                    last_seen_at: updatedUser.last_seen_at,
                  }
                : message.reply_to_message.sender,
          }
        : null,
      forwarded_from_message: message.forwarded_from_message
        ? {
            ...message.forwarded_from_message,
            sender:
              message.forwarded_from_message.sender.id === updatedUser.id
                ? {
                    ...message.forwarded_from_message.sender,
                    last_seen_at: updatedUser.last_seen_at,
                  }
                : message.forwarded_from_message.sender,
          }
        : null,
      reactions: message.reactions.map((reaction) => ({
        ...reaction,
        users: reaction.users.map((user) =>
          user.id === updatedUser.id
            ? {
                ...user,
                last_seen_at: updatedUser.last_seen_at,
              }
            : user
        ),
      })),
    };
  }

  function updateUserLastSeenInState(updatedUser: User) {
    setCurrentUser((previousUser) =>
      previousUser?.id === updatedUser.id
        ? {
            ...previousUser,
            last_seen_at: updatedUser.last_seen_at,
          }
        : previousUser
    );

    setConversations((previousConversations) =>
      previousConversations.map((conversation) => ({
        ...conversation,
        participants: conversation.participants.map((participant) =>
          participant.user.id === updatedUser.id
            ? {
                ...participant,
                user: {
                  ...participant.user,
                  last_seen_at: updatedUser.last_seen_at,
                },
              }
            : participant
        ),
        last_message: conversation.last_message
          ? updateUserLastSeenInMessage(conversation.last_message, updatedUser)
          : null,
      }))
    );

    setSelectedConversation((previousConversation) =>
      previousConversation
        ? {
            ...previousConversation,
            participants: previousConversation.participants.map((participant) =>
              participant.user.id === updatedUser.id
                ? {
                    ...participant,
                    user: {
                      ...participant.user,
                      last_seen_at: updatedUser.last_seen_at,
                    },
                  }
                : participant
            ),
            last_message: previousConversation.last_message
              ? updateUserLastSeenInMessage(
                  previousConversation.last_message,
                  updatedUser
                )
              : null,
          }
        : previousConversation
    );

    setMessages((previousMessages) =>
      previousMessages.map((message) =>
        updateUserLastSeenInMessage(message, updatedUser)
      )
    );

    setSearchResults((previousSearchResults) =>
      previousSearchResults.map((user) =>
        user.id === updatedUser.id
          ? {
              ...user,
              last_seen_at: updatedUser.last_seen_at,
            }
          : user
      )
    );
  }

  function removeConversationFromState(conversationId: number) {
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
  }

  function updateConversationInState(updatedConversation: Conversation) {
    setConversations((previousConversations) =>
      previousConversations.map((conversation) =>
        conversation.id === updatedConversation.id
          ? updatedConversation
          : conversation
      )
    );

    setSelectedConversation((previousConversation) =>
      previousConversation?.id === updatedConversation.id
        ? updatedConversation
        : previousConversation
    );
  }

  async function loadMessages(
    token: string,
    conversationId: number,
    shouldMarkAsRead = true
  ) {
    setIsMessagesLoading(true);
    setIsOlderMessagesLoading(false);
    setHasMoreMessages(false);
    scrollBehaviorRef.current = "bottom";

    try {
      const messagesPage = await getConversationMessagesPage(
        token,
        conversationId,
        undefined,
        MESSAGE_PAGE_SIZE
      );

      setMessages(messagesPage.results);
      setHasMoreMessages(messagesPage.has_more);

      if (shouldMarkAsRead) {
        await markConversationAsRead(token, conversationId);
        markConversationReadInState(conversationId);
      }
    } catch {
      setMessages([]);
      setHasMoreMessages(false);
    } finally {
      setIsMessagesLoading(false);
    }
  }

  async function loadOlderMessages() {
    if (
      !accessToken ||
      !selectedConversation ||
      !hasMoreMessages ||
      isOlderMessagesLoading ||
      isMessageSearchActive ||
      messages.length === 0
    ) {
      return;
    }

    const container = messagesContainerRef.current;

    if (container) {
      previousScrollHeightRef.current = container.scrollHeight;
      previousScrollTopRef.current = container.scrollTop;
      scrollBehaviorRef.current = "preserve";
    }

    setIsOlderMessagesLoading(true);

    try {
      const oldestMessageId = messages[0].id;

      const messagesPage = await getConversationMessagesPage(
        accessToken,
        selectedConversation.id,
        oldestMessageId,
        MESSAGE_PAGE_SIZE
      );

      setMessages((previousMessages) => {
        const existingMessageIds = new Set(
          previousMessages.map((message) => message.id)
        );

        const olderMessages = messagesPage.results.filter(
          (message) => !existingMessageIds.has(message.id)
        );

        return [...olderMessages, ...previousMessages];
      });

      setHasMoreMessages(messagesPage.has_more);
    } catch {
      scrollBehaviorRef.current = "bottom";
      console.error("Failed to load older messages.");
    } finally {
      setIsOlderMessagesLoading(false);
    }
  }

  async function refreshConversations(token: string) {
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
  }

  async function loadUserData(token: string) {
    const user = await getCurrentUser(token);
    setCurrentUser(user);
    setAccessToken(token);

    const conversationsData = await getConversations(token);
    setConversations(sortConversationsByUpdatedAt(conversationsData));

    setSelectedConversation(null);
    setMessages([]);
    setHasMoreMessages(false);
  }

  function clearSession() {
    localStorage.removeItem("accessToken");
    localStorage.removeItem("refreshToken");

    if (socketRef.current) {
      socketRef.current.close();
      socketRef.current = null;
    }

    if (notificationSocketRef.current) {
      notificationSocketRef.current.close();
      notificationSocketRef.current = null;
    }

    setCurrentUser(null);
    setAccessToken(null);
    setConversations([]);
    setSelectedConversation(null);
    setMessages([]);
    setHasMoreMessages(false);
    setIsOlderMessagesLoading(false);
    setIsDeletingConversationId(null);

    setSearchResults([]);
    setUserSearchQuery("");
    setAvatarError("");
    setIsAvatarUpdating(false);
    setIsProfileSettingsOpen(false);
    setProfileError("");
    setIsProfileUpdating(false);

    setMessageSearchQuery("");
    setIsMessageSearchActive(false);
    setIsSearchingMessages(false);

    setNewMessage("");
    setSelectedAttachment(null);
    setReplyToMessage(null);
    setForwardingMessage(null);
    setMessageError("");
    setIsForwardingMessage(false);
    setTypingUser(null);
    setOnlineUserIds([]);
  }

  async function handleLogout() {
    const savedRefreshToken = localStorage.getItem("refreshToken");

    if (socketRef.current) {
      socketRef.current.close();
      socketRef.current = null;
    }

    if (notificationSocketRef.current) {
      notificationSocketRef.current.close();
      notificationSocketRef.current = null;
    }

    if (accessToken && savedRefreshToken) {
      try {
        await logout(accessToken, savedRefreshToken);
      } catch {
        console.error("Logout request failed.");
      }
    }

    clearSession();
  }

  async function handleCurrentUserAvatarChange(
    event: ChangeEvent<HTMLInputElement>
  ) {
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
  }

  async function handleDeleteCurrentUserAvatar() {
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
  }

  async function handleUpdateCurrentUserProfile(username: string, email: string) {
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
  }

  function updateMessageInState(updatedMessage: Message) {
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
  }

  function markMessagesAsDeliveredInState(messageIds: number[]) {
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
  }

  function sendReadStatus() {
    const socket = socketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "read",
      })
    );
  }

  function sendTypingStatus(isTyping: boolean) {
    const socket = socketRef.current;

    if (!socket || socket.readyState !== WebSocket.OPEN) {
      return;
    }

    socket.send(
      JSON.stringify({
        type: "typing",
        is_typing: isTyping,
      })
    );
  }

  const getNotificationAudio = useCallback(() => {
    if (notificationAudioRef.current) {
      return notificationAudioRef.current;
    }

    const audio = new Audio("/sounds/notification.mp3");
    audio.preload = "auto";
    audio.volume = 0.7;

    notificationAudioRef.current = audio;

    return audio;
  }, []);

  const playIncomingMessageSound = useCallback(
    (message: Message) => {
      if (!currentUserId || message.sender.id === currentUserId) {
        return;
      }

      const messageConversation = conversationsRef.current.find(
        (conversation) => conversation.id === message.conversation
      );

      if (messageConversation?.is_muted) {
        return;
      }

      if (playedSoundMessageIdsRef.current.has(message.id)) {
        return;
      }

      playedSoundMessageIdsRef.current.add(message.id);

      if (playedSoundMessageIdsRef.current.size > 200) {
        const firstMessageId = playedSoundMessageIdsRef.current
          .values()
          .next().value;

        if (firstMessageId !== undefined) {
          playedSoundMessageIdsRef.current.delete(firstMessageId);
        }
      }

      const audio = getNotificationAudio();

      audio.currentTime = 0;

      void audio.play().catch(() => {
        console.warn("Notification sound was blocked by the browser.");
      });
    },
    [currentUserId, getNotificationAudio]
  );

  useEffect(() => {
    function unlockNotificationAudio() {
      const audio = getNotificationAudio();
      const originalVolume = audio.volume;

      audio.volume = 0;

      void audio
        .play()
        .then(() => {
          audio.pause();
          audio.currentTime = 0;
          audio.volume = originalVolume;
        })
        .catch(() => {
          audio.volume = originalVolume;
        });

      window.removeEventListener("pointerdown", unlockNotificationAudio);
      window.removeEventListener("keydown", unlockNotificationAudio);
    }

    window.addEventListener("pointerdown", unlockNotificationAudio);
    window.addEventListener("keydown", unlockNotificationAudio);

    return () => {
      window.removeEventListener("pointerdown", unlockNotificationAudio);
      window.removeEventListener("keydown", unlockNotificationAudio);
    };
  }, [getNotificationAudio]);
  
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
  }, []);

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

      if (data.type === "sidebar_message") {
        const receivedMessage = data.message as Message | undefined;

        if (receivedMessage) {
          playIncomingMessageSound(receivedMessage);
        }

        await refreshConversations(accessToken);
        return;
      }

      if (data.type === "conversation_deleted") {
        removeConversationFromState(data.conversation_id);
        return;
      }

      if (data.type === "user_profile_updated") {
        const updatedUser = addAvatarCacheBust(data.user as User);

        updateUserInState(updatedUser);

        return;
      }

      if (data.type === "online_users") {
        setOnlineUserIds(data.user_ids);
        return;
      }

      if (data.type === "online_status") {
        const user = data.user as User;

        updateUserLastSeenInState(user);

        setOnlineUserIds((previousOnlineUserIds) => {
          if (data.is_online) {
            if (previousOnlineUserIds.includes(user.id)) {
              return previousOnlineUserIds;
            }

            return [...previousOnlineUserIds, user.id];
          }

          return previousOnlineUserIds.filter((userId) => userId !== user.id);
        });

        return;
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
  }, [accessToken, currentUserId, playIncomingMessageSound]);

  useEffect(() => {
  if (!accessToken || !selectedConversationId) {
    return;
  }

  const websocketUrl = `${WS_BASE_URL}/conversations/${selectedConversationId}/?token=${accessToken}`;
  const socket = new WebSocket(websocketUrl);

  socketRef.current = socket;

  socket.onopen = () => {
    setMessageError("");
    sendReadStatus();
  };

  socket.onmessage = async (event) => {
    const data = JSON.parse(event.data);

    if (data.type === "message") {
      const receivedMessage = data.message as Message;

      playIncomingMessageSound(receivedMessage);
      addMessageToState(receivedMessage);

      if (
        accessToken &&
        selectedConversationId === receivedMessage.conversation &&
        receivedMessage.sender.id !== currentUserId
      ) {
        try {
          await markConversationAsRead(accessToken, selectedConversationId);
          markConversationReadInState(selectedConversationId);
          sendReadStatus();
        } catch {
          console.error("Failed to mark WebSocket message as read.");
        }
      }

      return;
    }

    if (data.type === "message_updated") {
      const updatedMessage = data.message as Message;

      updateMessageInState(updatedMessage);

      return;
    }

    if (data.type === "delivered") {
      const messageIds = data.message_ids as number[];

      markMessagesAsDeliveredInState(messageIds);

      return;
    }

    if (data.type === "read") {
      const reader = data.user as User;

      if (reader.id === currentUserId) {
        return;
      }

      setMessages((previousMessages) =>
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
        setTypingUser(receivedTypingUser);
      } else {
        setTypingUser(null);
      }

      return;
    }

    if (data.type === "error") {
      setMessageError(data.detail);
    }
  };

  socket.onerror = () => {
    if (socketRef.current === socket) {
      setMessageError("WebSocket connection error.");
    }

    console.error("Chat WebSocket connection error.");
  };

  return () => {
    if (typingTimeoutRef.current) {
      window.clearTimeout(typingTimeoutRef.current);
      typingTimeoutRef.current = null;
    }

    setTypingUser(null);

    socket.close();

    if (socketRef.current === socket) {
      socketRef.current = null;
    }
  };
}, [accessToken, selectedConversationId, currentUserId, playIncomingMessageSound]);

  useLayoutEffect(() => {
    const messagesContainer = messagesContainerRef.current;

    if (scrollBehaviorRef.current === "preserve" && messagesContainer) {
      const newScrollHeight = messagesContainer.scrollHeight;
      messagesContainer.scrollTop =
        newScrollHeight -
        previousScrollHeightRef.current +
        previousScrollTopRef.current;
      scrollBehaviorRef.current = "bottom";
      return;
    }

    messagesEndRef.current?.scrollIntoView({
      behavior: "smooth",
    });
  }, [messages, selectedConversation?.id]);

  async function handleLogin(event: FormEvent<HTMLFormElement>) {
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
  }

  async function handleRegister(event: FormEvent<HTMLFormElement>) {
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
  }

  async function handleSelectConversation(conversation: Conversation) {
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
  }

  async function handleSearchUsers(event: FormEvent<HTMLFormElement>) {
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
  }

  async function handleStartConversation(user: User) {
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
  }

  async function handleDeleteConversation(
    conversation: Conversation,
    mode: DeleteConversationMode
  ) {
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
  }

  async function handleMuteConversation(conversation: Conversation) {
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
  }

  async function handleSearchMessages(event: FormEvent<HTMLFormElement>) {
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
    scrollBehaviorRef.current = "bottom";

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
  }

  async function handleClearMessageSearch() {
    if (!accessToken || !selectedConversation) {
      setMessageSearchQuery("");
      setIsMessageSearchActive(false);
      return;
    }

    setMessageSearchQuery("");
    setIsMessageSearchActive(false);
    setIsSearchingMessages(false);

    await loadMessages(accessToken, selectedConversation.id);
  }

  function handleMessagesScroll() {
    const messagesContainer = messagesContainerRef.current;

    if (!messagesContainer) {
      return;
    }

    if (messagesContainer.scrollTop <= 32) {
      loadOlderMessages();
    }
  }

  function handleNewMessageChange(value: string) {
    setNewMessage(value);

    if (!selectedConversation) {
      return;
    }

    sendTypingStatus(true);

    if (typingTimeoutRef.current) {
      window.clearTimeout(typingTimeoutRef.current);
    }

    typingTimeoutRef.current = window.setTimeout(() => {
      sendTypingStatus(false);
    }, 1200);
  }

  function handleAttachmentChange(event: ChangeEvent<HTMLInputElement>) {
    const file = event.target.files?.[0];

    if (!file) {
      return;
    }

    setSelectedAttachment(file);
    setMessageError("");
  }

  function handleRemoveAttachment() {
    setSelectedAttachment(null);
  }

  function handleStartReplyMessage(message: Message) {
    if (message.is_deleted) {
      return;
    }

    setReplyToMessage(message);
    setMessageError("");
  }

  function handleCancelReplyMessage() {
    setReplyToMessage(null);
  }

  function handleStartForwardMessage(message: Message) {
    if (message.is_deleted) {
      return;
    }

    setForwardingMessage(message);
    setMessageError("");
  }

  function handleCancelForwardMessage() {
    setForwardingMessage(null);
  }

  async function handleForwardMessageToConversation(conversationId: number) {
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
  }

  async function handleSendMessage(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();

    if (!accessToken || !selectedConversation) {
      return;
    }

    const text = newMessage.trim();

    if (!text && !selectedAttachment) {
      setMessageError("Message text or attachment is required.");
      return;
    }

    const socket = socketRef.current;
    const replyToMessageId = replyToMessage?.id ?? null;

    if (!selectedAttachment && (!socket || socket.readyState !== WebSocket.OPEN)) {
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

      if (typingTimeoutRef.current) {
        window.clearTimeout(typingTimeoutRef.current);
        typingTimeoutRef.current = null;
      }

      setNewMessage("");
      setSelectedAttachment(null);
      setReplyToMessage(null);
    } catch {
      setMessageError("Failed to send message.");
    } finally {
      setIsSending(false);
    }
  }

  function handleMessageKeyDown(event: KeyboardEvent<HTMLTextAreaElement>) {
    if (event.key === "Enter" && !event.shiftKey) {
      event.preventDefault();

      const form = event.currentTarget.form;

      if (form) {
        form.requestSubmit();
      }
    }
  }

  function handleStartEditMessage(message: Message) {
    setEditingMessageId(message.id);
    setEditingMessageText(message.text);
    setMessageError("");
  }

  function handleCancelEditMessage() {
    setEditingMessageId(null);
    setEditingMessageText("");
  }

  async function handleSaveEditedMessage(messageId: number) {
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
  }

  async function handleDeleteMessage(messageId: number) {
    if (!accessToken) {
      return;
    }

    setIsDeletingMessageId(messageId);
    setMessageError("");

    try {
      const deletedMessage = await deleteMessage(accessToken, messageId);
      updateMessageInState(deletedMessage);

      if (editingMessageId === messageId) {
        setEditingMessageId(null);
        setEditingMessageText("");
      }
    } catch {
      setMessageError("Failed to delete message.");
    } finally {
      setIsDeletingMessageId(null);
    }
  }

  async function handleRemoveMessageAttachment(messageId: number) {
    if (!accessToken) {
      return;
    }

    setIsDeletingMessageId(messageId);
    setMessageError("");

    try {
      const updatedMessage = await removeMessageAttachment(accessToken, messageId);
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
  }

  async function handleToggleMessageReaction(messageId: number, emoji: string) {
    if (!accessToken) {
      return;
    }

    setMessageError("");

    try {
      const response = await toggleMessageReaction(accessToken, messageId, emoji);
      updateMessageInState(response.message);
    } catch {
      setMessageError("Failed to update reaction.");
    }
  }

  if (isAuthChecking) {
    return (
      <div className="auth-page">
        <div className="auth-card">
          <h1>Messenger</h1>
          <p>Loading...</p>
        </div>
      </div>
    );
  }

  if (!currentUser || !accessToken) {
    return (
      <AuthPage
        authMode={authMode}
        setAuthMode={setAuthMode}
        username={username}
        setUsername={setUsername}
        email={email}
        setEmail={setEmail}
        password={password}
        setPassword={setPassword}
        passwordConfirm={passwordConfirm}
        setPasswordConfirm={setPasswordConfirm}
        error={error}
        setError={setError}
        isLoading={isLoading}
        handleLogin={handleLogin}
        handleRegister={handleRegister}
      />
    );
  }

  const selectedConversationName = selectedConversation
    ? getConversationName(selectedConversation, currentUser)
    : "No conversation selected";

  const selectedConversationUser = selectedConversation
    ? getOtherParticipant(selectedConversation, currentUser) ?? null
    : null;

  const selectedConversationUserIsOnline = selectedConversationUser
    ? onlineUserIds.includes(selectedConversationUser.id)
    : false;

  return (
    <div className="app">
      <Sidebar
        currentUser={currentUser}
        onlineUserIds={onlineUserIds}
        conversations={conversations}
        selectedConversation={selectedConversation}
        userSearchQuery={userSearchQuery}
        setUserSearchQuery={setUserSearchQuery}
        searchResults={searchResults}
        isSearchingUsers={isSearchingUsers}
        userSearchError={userSearchError}
        isDeletingConversationId={isDeletingConversationId}
        handleLogout={handleLogout}
        handleOpenProfileSettings={() => setIsProfileSettingsOpen(true)}
        handleSearchUsers={handleSearchUsers}
        handleStartConversation={handleStartConversation}
        handleSelectConversation={handleSelectConversation}
        handleDeleteConversation={handleDeleteConversation}
        handleMuteConversation={handleMuteConversation}
      />

      <ChatWindow
        currentUser={currentUser}
        selectedConversation={selectedConversation}
        selectedConversationName={selectedConversationName}
        selectedConversationUser={selectedConversationUser}
        selectedConversationUserIsOnline={selectedConversationUserIsOnline}
        messages={messages}
        isMessagesLoading={isMessagesLoading}
        isOlderMessagesLoading={isOlderMessagesLoading}
        hasMoreMessages={hasMoreMessages}
        messageSearchQuery={messageSearchQuery}
        setMessageSearchQuery={setMessageSearchQuery}
        isMessageSearchActive={isMessageSearchActive}
        isSearchingMessages={isSearchingMessages}
        typingUser={typingUser}
        messageError={messageError}
        newMessage={newMessage}
        selectedAttachment={selectedAttachment}
        replyToMessage={replyToMessage}
        isSending={isSending}
        editingMessageId={editingMessageId}
        editingMessageText={editingMessageText}
        setEditingMessageText={setEditingMessageText}
        isEditingMessage={isEditingMessage}
        isDeletingMessageId={isDeletingMessageId}
        messagesContainerRef={messagesContainerRef}
        messagesEndRef={messagesEndRef}
        handleMessagesScroll={handleMessagesScroll}
        handleSearchMessages={handleSearchMessages}
        handleClearMessageSearch={handleClearMessageSearch}
        handleNewMessageChange={handleNewMessageChange}
        handleAttachmentChange={handleAttachmentChange}
        handleRemoveAttachment={handleRemoveAttachment}
        handleStartReplyMessage={handleStartReplyMessage}
        handleCancelReplyMessage={handleCancelReplyMessage}
        handleStartForwardMessage={handleStartForwardMessage}
        handleMessageKeyDown={handleMessageKeyDown}
        handleSendMessage={handleSendMessage}
        handleStartEditMessage={handleStartEditMessage}
        handleCancelEditMessage={handleCancelEditMessage}
        handleSaveEditedMessage={handleSaveEditedMessage}
        handleDeleteMessage={handleDeleteMessage}
        handleRemoveMessageAttachment={handleRemoveMessageAttachment}
        handleToggleMessageReaction={handleToggleMessageReaction}
      />
      {isProfileSettingsOpen && (
        <ProfileSettingsModal
          currentUser={currentUser}
          avatarError={avatarError}
          profileError={profileError}
          isAvatarUpdating={isAvatarUpdating}
          isProfileUpdating={isProfileUpdating}
          handleClose={() => {
            setIsProfileSettingsOpen(false);
            setAvatarError("");
            setProfileError("");
          }}
          handleCurrentUserAvatarChange={handleCurrentUserAvatarChange}
          handleDeleteCurrentUserAvatar={handleDeleteCurrentUserAvatar}
          handleUpdateCurrentUserProfile={handleUpdateCurrentUserProfile}
        />
      )}
      {forwardingMessage && (
        <div className="modal-backdrop">
          <div className="forward-modal">
            <h3>Forward message</h3>

            <div className="forward-message-preview">
              {getForwardPreviewText(forwardingMessage)}
            </div>

            <div className="forward-conversation-list">
              {conversations.map((conversation) => {
                const lastMessage = conversation.last_message;
                const lastMessageText =
                  lastMessage?.text.trim() ||
                  lastMessage?.attachment_name ||
                  "No messages yet";

                return (
                  <button
                    key={conversation.id}
                    type="button"
                    onClick={() => handleForwardMessageToConversation(conversation.id)}
                    disabled={isForwardingMessage}
                  >
                    <span className="forward-conversation-name">
                      {getConversationName(conversation, currentUser)}
                    </span>
                    <span className="forward-conversation-preview">
                      {lastMessageText}
                    </span>
                  </button>
                );
              })}
            </div>

            <div className="forward-modal-actions">
              <button
                type="button"
                onClick={handleCancelForwardMessage}
                disabled={isForwardingMessage}
              >
                Cancel
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;