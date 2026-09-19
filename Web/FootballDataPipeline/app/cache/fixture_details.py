from pydantic import ValidationError
from redis.asyncio import Redis
from redis.exceptions import RedisError

from app.schemas.fixture_details import FixtureDetailsResponse

CACHE_KEY_PREFIX = "fixture-details"


def fixture_details_cache_key(
    fixture_api_id: int,
) -> str:
    return f"{CACHE_KEY_PREFIX}:{fixture_api_id}"


async def get_cached_fixture_details(
    redis_client: Redis,
    fixture_api_id: int,
) -> FixtureDetailsResponse | None:
    cache_key = fixture_details_cache_key(fixture_api_id)

    try:
        cached_value = await redis_client.get(cache_key)
    except RedisError:
        return None

    if cached_value is None:
        return None

    try:
        return FixtureDetailsResponse.model_validate_json(
            cached_value,
        )
    except ValidationError:
        try:
            await redis_client.delete(cache_key)
        except RedisError:
            pass

        return None


async def set_cached_fixture_details(
    redis_client: Redis,
    fixture_api_id: int,
    fixture_details: FixtureDetailsResponse,
    *,
    ttl_seconds: int,
) -> None:
    cache_key = fixture_details_cache_key(fixture_api_id)

    try:
        await redis_client.set(
            cache_key,
            fixture_details.model_dump_json(),
            ex=ttl_seconds,
        )
    except RedisError:
        pass
