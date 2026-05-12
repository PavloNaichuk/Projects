import type { User } from "../api/auth";
import type { Conversation } from "../api/conversations";

export function getConversationName(
  conversation: Conversation,
  currentUser: User
) {
  const otherParticipant = conversation.participants.find(
    (participant) => participant.user.id !== currentUser.id
  );

  return otherParticipant?.user.username ?? "Unknown user";
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

  return 0;
}

export function sortConversationsByUpdatedAt(conversations: Conversation[]) {
  return [...conversations].sort(
    (firstConversation, secondConversation) =>
      getConversationSortTime(secondConversation) -
      getConversationSortTime(firstConversation)
  );
}