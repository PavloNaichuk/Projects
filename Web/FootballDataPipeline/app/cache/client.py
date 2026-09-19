from redis.asyncio import Redis

from app.core.config import get_settings


def create_redis_client() -> Redis:
    settings = get_settings()

    return Redis.from_url(
        settings.redis_cache_url,
        encoding="utf-8",
        decode_responses=True,
    )
