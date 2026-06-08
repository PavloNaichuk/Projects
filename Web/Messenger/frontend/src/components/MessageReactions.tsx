import type { Message } from "../api/conversations";

type MessageReactionsProps = {
  message: Message;
  handleToggleMessageReaction: (
    messageId: number,
    emoji: string
  ) => Promise<void>;
};

function MessageReactions({
  message,
  handleToggleMessageReaction,
}: MessageReactionsProps) {
  return (
    <div className="message-reactions">
      {message.reactions.map((reaction) => (
        <button
          key={reaction.emoji}
          type="button"
          className={
            reaction.reacted_by_me
              ? "message-reaction active"
              : "message-reaction"
          }
          onClick={() =>
            handleToggleMessageReaction(message.id, reaction.emoji)
          }
        >
          <span>{reaction.emoji}</span>
          <span>{reaction.count}</span>
        </button>
      ))}
    </div>
  );
}

export default MessageReactions;
