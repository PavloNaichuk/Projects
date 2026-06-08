import { useCallback, type Dispatch, type SetStateAction } from "react";

import type { User } from "../api/auth";
import type { Conversation, Message } from "../api/conversations";

type UseUserStateUpdatesParams = {
  setCurrentUser: Dispatch<SetStateAction<User | null>>;
  setConversations: Dispatch<SetStateAction<Conversation[]>>;
  setSelectedConversation: Dispatch<SetStateAction<Conversation | null>>;
  setMessages: Dispatch<SetStateAction<Message[]>>;
  setSearchResults: Dispatch<SetStateAction<User[]>>;
};

export function useUserStateUpdates({
  setCurrentUser,
  setConversations,
  setSelectedConversation,
  setMessages,
  setSearchResults,
}: UseUserStateUpdatesParams) {
  const updateUserInMessage = useCallback(
    (message: Message, updatedUser: User): Message => {
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
    },
    []
  );

  const updateUserInState = useCallback(
    (updatedUser: User) => {
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
              participants: previousConversation.participants.map(
                (participant) =>
                  participant.user.id === updatedUser.id
                    ? {
                        ...participant,
                        user: updatedUser,
                      }
                    : participant
              ),
              last_message: previousConversation.last_message
                ? updateUserInMessage(
                    previousConversation.last_message,
                    updatedUser
                  )
                : null,
            }
          : previousConversation
      );

      setMessages((previousMessages) =>
        previousMessages.map((message) =>
          updateUserInMessage(message, updatedUser)
        )
      );

      setSearchResults((previousSearchResults) =>
        previousSearchResults.map((user) =>
          user.id === updatedUser.id ? updatedUser : user
        )
      );
    },
    [
      setCurrentUser,
      setConversations,
      setMessages,
      setSearchResults,
      setSelectedConversation,
      updateUserInMessage,
    ]
  );

  const updateUserLastSeenInMessage = useCallback(
    (message: Message, updatedUser: User): Message => {
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
    },
    []
  );

  const updateUserLastSeenInState = useCallback(
    (updatedUser: User) => {
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
            ? updateUserLastSeenInMessage(
                conversation.last_message,
                updatedUser
              )
            : null,
        }))
      );

      setSelectedConversation((previousConversation) =>
        previousConversation
          ? {
              ...previousConversation,
              participants: previousConversation.participants.map(
                (participant) =>
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
    },
    [
      setCurrentUser,
      setConversations,
      setMessages,
      setSearchResults,
      setSelectedConversation,
      updateUserLastSeenInMessage,
    ]
  );

  return {
    updateUserInState,
    updateUserLastSeenInState,
  };
}
