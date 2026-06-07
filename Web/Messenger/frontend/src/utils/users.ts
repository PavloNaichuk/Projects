import type { User } from "../api/auth";

export function addAvatarCacheBust(user: User): User {
  if (!user.avatar_url) {
    return user;
  }

  const separator = user.avatar_url.includes("?") ? "&" : "?";

  return {
    ...user,
    avatar_url: `${user.avatar_url}${separator}v=${Date.now()}`,
  };
}