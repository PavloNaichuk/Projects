import type { ReactNode } from "react";

type MessageTextProps = {
  text: string;
  searchQuery: string;
};

function renderHighlightedText(text: string, searchQuery: string) {
  const query = searchQuery.trim();

  if (!query) {
    return text;
  }

  const lowerText = text.toLowerCase();
  const lowerQuery = query.toLowerCase();

  const parts: ReactNode[] = [];
  let currentIndex = 0;

  while (currentIndex < text.length) {
    const matchIndex = lowerText.indexOf(lowerQuery, currentIndex);

    if (matchIndex === -1) {
      parts.push(text.slice(currentIndex));
      break;
    }

    if (matchIndex > currentIndex) {
      parts.push(text.slice(currentIndex, matchIndex));
    }

    const matchedText = text.slice(matchIndex, matchIndex + query.length);

    parts.push(<mark key={`${matchIndex}-${matchedText}`}>{matchedText}</mark>);

    currentIndex = matchIndex + query.length;
  }

  return parts;
}

function MessageText({ text, searchQuery }: MessageTextProps) {
  return <p>{renderHighlightedText(text, searchQuery)}</p>;
}

export default MessageText;
