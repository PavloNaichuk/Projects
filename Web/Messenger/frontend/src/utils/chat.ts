import type { User } from "../api/auth";
import type { Conversation } from "../api/conversations";

export function getUserDisplayName(user: User) {
  return user.display_name || user.username;
}

export function getConversationName(
  conversation: Conversation,
  currentUser: User
) {
  const otherParticipant = conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  );

  if (!otherParticipant) {
    return "Unknown user";
  }

  return getUserDisplayName(otherParticipant.user);
}

export function getOtherParticipant(
  conversation: Conversation,
  currentUser: User
) {
  return conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  )?.user;
}

export function formatMessageTime(dateString: string) {
  return new Date(dateString).toLocaleTimeString([], {
    hour: "2-digit",
    minute: "2-digit",
  });
}

export function formatShortTime(dateString: string) {
  return new Date(dateString).toLocaleTimeString([], {
    hour: "2-digit",
    minute: "2-digit",
  });
}

export function formatMessageDate(dateString: string) {
  return new Date(dateString).toLocaleDateString([], {
    day: "2-digit",
    month: "long",
    year: "numeric",
  });
}

export function isSameMessageDate(firstDate: string, secondDate: string) {
  return (
    new Date(firstDate).toDateString() === new Date(secondDate).toDateString()
  );
}

function getConversationSortTime(conversation: Conversation) {
  if (conversation.last_message) {
    return new Date(conversation.last_message.created_at).getTime();
  }

  return new Date(conversation.updated_at).getTime();
}

export function sortConversationsByUpdatedAt(conversations: Conversation[]) {
  return [...conversations].sort((firstConversation, secondConversation) => {
    if (firstConversation.is_pinned !== secondConversation.is_pinned) {
      return firstConversation.is_pinned ? -1 : 1;
    }

    return (
      getConversationSortTime(secondConversation) -
      getConversationSortTime(firstConversation)
    );
  });
}