import {
  useCallback,
  useState,
  type ChangeEvent,
  type FormEvent,
  type KeyboardEvent,
} from "react";
import "./App.css";
import {
  deleteCurrentUserAvatar,
  updateContactNickname,
  updateCurrentUserAvatar,
  updateCurrentUserProfile,
  type User,
} from "./api/auth";
import {
  clearConversationHistory,
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
  markConversationAsUnread,
  muteConversation,
  pinConversation,
  type Conversation,
  type DeleteConversationMode,
  type DeleteMessageMode,
  type Message,
} from "./api/conversations";
import { blockUser, getBlockedUsers, searchUsers, unblockUser } from "./api/users";
import AuthPage from "./components/AuthPage";
import ChatWindow from "./components/ChatWindow";
import Sidebar from "./components/Sidebar";
import ProfileSettingsModal from "./components/ProfileSettingsModal";
import ContactNicknameModal from "./components/ContactNicknameModal";
import ForwardMessageModal from "./components/ForwardMessageModal";
import { useAuthSession } from "./hooks/useAuthSession";
import { useMessageScroll } from "./hooks/useMessageScroll";
import { useMessengerSockets } from "./hooks/useMessengerSockets";
import { useNotificationSound } from "./hooks/useNotificationSound";
import { useUserStateUpdates } from "./hooks/useUserStateUpdates";
import {
  getConversationName,
  getOtherParticipant,
  sortConversationsByUpdatedAt,
} from "./utils/chat";
import { addAvatarCacheBust } from "./utils/users";

const MESSAGE_PAGE_SIZE = 20;


function App() {
  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [selectedConversation, setSelectedConversation] =
    useState<Conversation | null>(null);
  const [messages, setMessages] = useState<Message[]>([]);

  const [isMessagesLoading, setIsMessagesLoading] = useState(false);
  const [isOlderMessagesLoading, setIsOlderMessagesLoading] = useState(false);
  const [hasMoreMessages, setHasMoreMessages] = useState(false);
  const [newMessage, setNewMessage] = useState("");
  const [selectedAttachment, setSelectedAttachment] = useState<File | null>(
    null
  );
  const [replyToMessage, setReplyToMessage] = useState<Message | null>(null);
  const [forwardingMessage, setForwardingMessage] = useState<Message | null>(
    null
  );
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
  const [blockedUsers, setBlockedUsers] = useState<User[]>([]);
  const [blockedUsersError, setBlockedUsersError] = useState("");
  const [isBlockedUsersLoading, setIsBlockedUsersLoading] = useState(false);
  const [isUnblockingUserId, setIsUnblockingUserId] = useState<number | null>(
    null
  );
  const [contactNicknameUser, setContactNicknameUser] = useState<User | null>(
    null
  );
  const [contactNicknameError, setContactNicknameError] = useState("");
  const [isContactNicknameUpdating, setIsContactNicknameUpdating] =
    useState(false);

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
  const [isBlockingUserId, setIsBlockingUserId] = useState<number | null>(null);

  const resetSessionState = useCallback(() => {
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
    setBlockedUsers([]);
    setBlockedUsersError("");
    setIsBlockedUsersLoading(false);
    setIsUnblockingUserId(null);
    setContactNicknameUser(null);
    setContactNicknameError("");
    setIsContactNicknameUpdating(false);

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
    setIsBlockingUserId(null);
  }, []);

  const {
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
    handleLogin,
    handleRegister,
    handleLogout,
  } = useAuthSession({
    setConversations,
    setSelectedConversation,
    setMessages,
    setHasMoreMessages,
    resetSessionState,
  });

  const currentUserId = currentUser?.id ?? null;
  const selectedConversationId = selectedConversation?.id ?? null;
  const selectedConversationUser =
    selectedConversation && currentUser
      ? getOtherParticipant(selectedConversation, currentUser) ?? null
      : null;
  const selectedConversationUserIsOnline = selectedConversationUser
    ? onlineUserIds.includes(selectedConversationUser.id)
    : false;

  const playIncomingMessageSound = useNotificationSound({
    currentUserId,
    conversations,
  });

  const {
    messagesContainerRef,
    messagesEndRef,
    preserveScrollPosition,
    scrollToBottomOnNextRender,
    handleMessagesScroll,
  } = useMessageScroll({
    messages,
    selectedConversationId,
    onLoadOlderMessages: loadOlderMessages,
  });

  const { updateUserInState, updateUserLastSeenInState } = useUserStateUpdates({
    setCurrentUser,
    setConversations,
    setSelectedConversation,
    setMessages,
    setSearchResults,
  });

  const {
    sendTypingStatus,
    notifyTypingActivity,
    clearTypingTimeout,
    getChatSocket,
  } = useMessengerSockets({
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
  });

  function markConversationReadInState(conversationId: number) {
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
            is_marked_unread: false,
          }
        : previousConversation
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
  }

  async function loadMessages(
    token: string,
    conversationId: number,
    shouldMarkAsRead = true
  ) {
    setIsMessagesLoading(true);
    setIsOlderMessagesLoading(false);
    setHasMoreMessages(false);
    scrollToBottomOnNextRender();

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

    preserveScrollPosition();

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
      scrollToBottomOnNextRender();
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

  function removeMessageFromState(messageId: number) {
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

  async function handlePinConversation(conversation: Conversation) {
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
  }

  async function handleMarkConversationAsUnread(conversation: Conversation) {
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
  }

  async function handleClearConversationHistory(conversation: Conversation) {
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
  }

  function handleOpenContactNicknameModal(conversation: Conversation) {
    if (!currentUser) {
      return;
    }

    const user = getOtherParticipant(conversation, currentUser);

    if (!user) {
      return;
    }

    setContactNicknameUser(user);
    setContactNicknameError("");
  }

  async function handleUpdateContactNickname(nickname: string) {
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
  }

  async function handleBlockUser(user: User) {
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
  }

  async function handleUnblockUser(user: User) {
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
  }

  async function handleLoadBlockedUsers() {
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
  }

  async function handleUnblockBlockedUser(user: User) {
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

  function handleNewMessageChange(value: string) {
    setNewMessage(value);

    if (!selectedConversation) {
      return;
    }

    notifyTypingActivity();
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

      clearTypingTimeout();

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

  async function handleDeleteMessage(
    messageId: number,
    mode: DeleteMessageMode
  ) {
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
        isBlockingUserId={isBlockingUserId}
        handleLogout={handleLogout}
        handleOpenProfileSettings={() => setIsProfileSettingsOpen(true)}
        handleSearchUsers={handleSearchUsers}
        handleStartConversation={handleStartConversation}
        handleSelectConversation={handleSelectConversation}
        handleDeleteConversation={handleDeleteConversation}
        handleMuteConversation={handleMuteConversation}
        handlePinConversation={handlePinConversation}
        handleMarkConversationAsUnread={handleMarkConversationAsUnread}
        handleClearConversationHistory={handleClearConversationHistory}
        handleOpenContactNicknameModal={handleOpenContactNicknameModal}
        handleBlockUser={handleBlockUser}
        handleUnblockUser={handleUnblockUser}
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
        isDeletingConversationId={isDeletingConversationId}
        isBlockingUserId={isBlockingUserId}
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
        handleMuteConversation={handleMuteConversation}
        handlePinConversation={handlePinConversation}
        handleOpenContactNicknameModal={handleOpenContactNicknameModal}
        handleDeleteConversation={handleDeleteConversation}
        handleBlockUser={handleBlockUser}
        handleUnblockUser={handleUnblockUser}
      />

      {isProfileSettingsOpen && (
        <ProfileSettingsModal
          currentUser={currentUser}
          avatarError={avatarError}
          profileError={profileError}
          isAvatarUpdating={isAvatarUpdating}
          isProfileUpdating={isProfileUpdating}
          blockedUsers={blockedUsers}
          blockedUsersError={blockedUsersError}
          isBlockedUsersLoading={isBlockedUsersLoading}
          isUnblockingUserId={isUnblockingUserId}
          handleClose={() => {
            setIsProfileSettingsOpen(false);
            setAvatarError("");
            setProfileError("");
            setBlockedUsersError("");
          }}
          handleCurrentUserAvatarChange={handleCurrentUserAvatarChange}
          handleDeleteCurrentUserAvatar={handleDeleteCurrentUserAvatar}
          handleUpdateCurrentUserProfile={handleUpdateCurrentUserProfile}
          handleLoadBlockedUsers={handleLoadBlockedUsers}
          handleUnblockBlockedUser={handleUnblockBlockedUser}
        />
      )}

      {contactNicknameUser && (
        <ContactNicknameModal
          user={contactNicknameUser}
          isUpdating={isContactNicknameUpdating}
          error={contactNicknameError}
          handleClose={() => {
            setContactNicknameUser(null);
            setContactNicknameError("");
          }}
          handleSave={handleUpdateContactNickname}
        />
      )}

      {forwardingMessage && (
        <ForwardMessageModal
          forwardingMessage={forwardingMessage}
          conversations={conversations}
          currentUser={currentUser}
          isForwardingMessage={isForwardingMessage}
          handleForwardMessageToConversation={handleForwardMessageToConversation}
          handleCancelForwardMessage={handleCancelForwardMessage}
        />
      )}
    </div>
  );
}

export default App;