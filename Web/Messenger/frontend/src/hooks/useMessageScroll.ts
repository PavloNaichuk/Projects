import { useCallback, useLayoutEffect, useRef } from "react";

import type { Message } from "../api/conversations";

type ScrollBehavior = "bottom" | "preserve";

type UseMessageScrollParams = {
  messages: Message[];
  selectedConversationId: number | null;
  onLoadOlderMessages: () => void;
};

export function useMessageScroll({
  messages,
  selectedConversationId,
  onLoadOlderMessages,
}: UseMessageScrollParams) {
  const messagesContainerRef = useRef<HTMLElement | null>(null);
  const messagesEndRef = useRef<HTMLDivElement | null>(null);
  const scrollBehaviorRef = useRef<ScrollBehavior>("bottom");
  const previousScrollHeightRef = useRef(0);
  const previousScrollTopRef = useRef(0);

  const scrollToBottomOnNextRender = useCallback(() => {
    scrollBehaviorRef.current = "bottom";
  }, []);

  const preserveScrollPosition = useCallback(() => {
    const messagesContainer = messagesContainerRef.current;

    if (!messagesContainer) {
      return;
    }

    previousScrollHeightRef.current = messagesContainer.scrollHeight;
    previousScrollTopRef.current = messagesContainer.scrollTop;
    scrollBehaviorRef.current = "preserve";
  }, []);

  const handleMessagesScroll = useCallback(() => {
    const messagesContainer = messagesContainerRef.current;

    if (!messagesContainer) {
      return;
    }

    if (messagesContainer.scrollTop <= 32) {
      onLoadOlderMessages();
    }
  }, [onLoadOlderMessages]);

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
  }, [messages, selectedConversationId]);

  return {
    messagesContainerRef,
    messagesEndRef,
    preserveScrollPosition,
    scrollToBottomOnNextRender,
    handleMessagesScroll,
  };
}
