import { useCallback, useState } from "react";
import "./App.css";
import { resendEmailVerification, type User } from "./api/auth";
import {
  getConversationMessagesPage,
  markConversationAsRead,
  type Conversation,
  type Message,
} from "./api/conversations";
import AuthPage from "./components/AuthPage";
import ChatWindow from "./components/ChatWindow";
import Sidebar from "./components/Sidebar";
import ProfileSettingsModal from "./components/ProfileSettingsModal";
import VerifyEmailPage from "./components/VerifyEmailPage";
import ContactNicknameModal from "./components/ContactNicknameModal";
import ForwardMessageModal from "./components/ForwardMessageModal";
import { useAuthSession } from "./hooks/useAuthSession";
import { useConversationActions } from "./hooks/useConversationActions";
import { useMessageActions } from "./hooks/useMessageActions";
import { useMessageScroll } from "./hooks/useMessageScroll";
import { useMessengerStateUpdates } from "./hooks/useMessengerStateUpdates";
import { useMessengerSockets } from "./hooks/useMessengerSockets";
import { useNotificationSound } from "./hooks/useNotificationSound";
import { useProfileActions } from "./hooks/useProfileActions";
import { useUserSearchActions } from "./hooks/useUserSearchActions";
import { useUserStateUpdates } from "./hooks/useUserStateUpdates";
import {
  getConversationName,
  getOtherParticipant,
} from "./utils/chat";

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
  const [emailVerificationMessage, setEmailVerificationMessage] = useState("");
  const [emailVerificationError, setEmailVerificationError] = useState("");
  const [isResendingEmailVerification, setIsResendingEmailVerification] =
    useState(false);
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
    setEmailVerificationMessage("");
    setEmailVerificationError("");
    setIsResendingEmailVerification(false);
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

  const isEmailVerificationPage = window.location.pathname === "/verify-email";

  const handleEmailVerified = useCallback(
    (verifiedUser: User) => {
      if (currentUser?.id === verifiedUser.id) {
        setCurrentUser(verifiedUser);
      }

      setEmailVerificationMessage("Email verified successfully.");
      setEmailVerificationError("");
    },
    [currentUser?.id, setCurrentUser]
  );

  const handleResendEmailVerification = useCallback(async () => {
    if (!accessToken) {
      return;
    }

    setIsResendingEmailVerification(true);
    setEmailVerificationMessage("");
    setEmailVerificationError("");

    try {
      const response = await resendEmailVerification(accessToken);
      setEmailVerificationMessage(response.detail);
    } catch (error) {
      setEmailVerificationError(
        error instanceof Error
          ? error.message
          : "Failed to resend verification email."
      );
    } finally {
      setIsResendingEmailVerification(false);
    }
  }, [accessToken]);

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
    markConversationReadInState,
    addMessageToState,
    removeConversationFromState,
    updateConversationInState,
    removeMessageFromState,
    updateMessageInState,
    markMessagesAsDeliveredInState,
  } = useMessengerStateUpdates({
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
  });

  const {
    refreshConversations,
    handleSelectConversation,
    handleStartConversation,
    handleDeleteConversation,
    handleMuteConversation,
    handlePinConversation,
    handleMarkConversationAsUnread,
    handleClearConversationHistory,
  } = useConversationActions({
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

  const {
    handleCurrentUserAvatarChange,
    handleDeleteCurrentUserAvatar,
    handleUpdateCurrentUserProfile,
    handleLoadBlockedUsers,
    handleUnblockBlockedUser,
    handleOpenContactNicknameModal,
    handleUpdateContactNickname,
  } = useProfileActions({
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
  });

  const { handleSearchUsers, handleBlockUser, handleUnblockUser } =
    useUserSearchActions({
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
    });

  const {
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
  } = useMessageActions({
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
  });

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

  if (isEmailVerificationPage) {
    return <VerifyEmailPage handleVerified={handleEmailVerified} />;
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

      <div className="main-content">
        {!currentUser.is_email_verified && (
          <div className="email-verification-banner">
            <div>
              <strong>Your email is not verified.</strong>
              <span>
                {" "}
                Check your inbox or resend the verification email.
              </span>

              {emailVerificationMessage && (
                <p className="email-verification-success">
                  {emailVerificationMessage}
                </p>
              )}

              {emailVerificationError && (
                <p className="email-verification-error">
                  {emailVerificationError}
                </p>
              )}
            </div>

            <button
              type="button"
              onClick={handleResendEmailVerification}
              disabled={isResendingEmailVerification}
            >
              {isResendingEmailVerification ? "Sending..." : "Resend email"}
            </button>
          </div>
        )}

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
      </div>

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
