from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from redis.asyncio import Redis
from redis.exceptions import RedisError

from app.cache.fixture_details import (
    delete_all_cached_fixture_details,
    delete_cached_fixture_details,
    fixture_details_cache_key,
    get_cached_fixture_details,
    set_cached_fixture_details,
)
from app.schemas.fixture_details import FixtureDetailsResponse


def test_fixture_details_cache_key() -> None:
    assert fixture_details_cache_key(1208021) == "fixture-details:1208021"


@pytest.mark.asyncio
async def test_get_cached_fixture_details_returns_value() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.get = AsyncMock(
        return_value='{"fixture": {}}',
    )

    expected_details = MagicMock(
        spec=FixtureDetailsResponse,
    )

    with patch(
        ("app.cache.fixture_details.FixtureDetailsResponse.model_validate_json"),
        return_value=expected_details,
    ) as validate_json:
        result = await get_cached_fixture_details(
            redis_client,
            1208021,
        )

    assert result is expected_details
    redis_client.get.assert_awaited_once_with(
        "fixture-details:1208021",
    )
    validate_json.assert_called_once_with(
        '{"fixture": {}}',
    )


@pytest.mark.asyncio
async def test_get_cached_fixture_details_removes_invalid_value() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.get = AsyncMock(
        return_value="invalid-json",
    )
    redis_client.delete = AsyncMock()

    result = await get_cached_fixture_details(
        redis_client,
        1208021,
    )

    assert result is None
    redis_client.delete.assert_awaited_once_with(
        "fixture-details:1208021",
    )


@pytest.mark.asyncio
async def test_get_cached_fixture_details_ignores_redis_error() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.get = AsyncMock(
        side_effect=RedisError("Redis unavailable"),
    )

    result = await get_cached_fixture_details(
        redis_client,
        1208021,
    )

    assert result is None


@pytest.mark.asyncio
async def test_set_cached_fixture_details() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.set = AsyncMock()

    fixture_details = MagicMock(
        spec=FixtureDetailsResponse,
    )
    fixture_details.model_dump_json.return_value = '{"fixture": {}}'

    await set_cached_fixture_details(
        redis_client,
        1208021,
        fixture_details,
        ttl_seconds=60,
    )

    redis_client.set.assert_awaited_once_with(
        "fixture-details:1208021",
        '{"fixture": {}}',
        ex=60,
    )


@pytest.mark.asyncio
async def test_set_cached_fixture_details_ignores_redis_error() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.set = AsyncMock(
        side_effect=RedisError("Redis unavailable"),
    )

    fixture_details = MagicMock(
        spec=FixtureDetailsResponse,
    )
    fixture_details.model_dump_json.return_value = '{"fixture": {}}'

    await set_cached_fixture_details(
        redis_client,
        1208021,
        fixture_details,
        ttl_seconds=60,
    )


@pytest.mark.asyncio
async def test_delete_cached_fixture_details() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.delete = AsyncMock()

    await delete_cached_fixture_details(
        redis_client,
        1208021,
    )

    redis_client.delete.assert_awaited_once_with(
        "fixture-details:1208021",
    )


@pytest.mark.asyncio
async def test_delete_cached_fixture_details_ignores_error() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.delete = AsyncMock(
        side_effect=RedisError("Redis unavailable"),
    )

    await delete_cached_fixture_details(
        redis_client,
        1208021,
    )


@pytest.mark.asyncio
async def test_delete_all_cached_fixture_details() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.scan = AsyncMock(
        return_value=(
            0,
            [
                "fixture-details:1208021",
                "fixture-details:1208022",
            ],
        ),
    )
    redis_client.delete = AsyncMock()

    await delete_all_cached_fixture_details(
        redis_client,
    )

    redis_client.scan.assert_awaited_once_with(
        cursor=0,
        match="fixture-details:*",
        count=100,
    )
    redis_client.delete.assert_awaited_once_with(
        "fixture-details:1208021",
        "fixture-details:1208022",
    )


@pytest.mark.asyncio
async def test_delete_all_cached_fixture_details_ignores_error() -> None:
    redis_client = MagicMock(spec=Redis)
    redis_client.scan = AsyncMock(
        side_effect=RedisError("Redis unavailable"),
    )

    await delete_all_cached_fixture_details(
        redis_client,
    )
