import { useCallback, useEffect, useRef } from "react";

import type { Conversation, Message } from "../api/conversations";

type UseNotificationSoundParams = {
  currentUserId: number | null;
  conversations: Conversation[];
};

export function useNotificationSound({
  currentUserId,
  conversations,
}: UseNotificationSoundParams) {
  const conversationsRef = useRef<Conversation[]>([]);
  const playedSoundMessageIdsRef = useRef<Set<number>>(new Set());
  const notificationAudioRef = useRef<HTMLAudioElement | null>(null);

  useEffect(() => {
    conversationsRef.current = conversations;
  }, [conversations]);

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

  return playIncomingMessageSound;
}
