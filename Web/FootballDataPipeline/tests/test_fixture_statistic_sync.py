from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import APIFootballClient
from app.services.fixture_statistic_sync import (
    FixtureNotFoundError,
    FixtureStatisticTeamNotFoundError,
    sync_fixture_statistics,
)


def make_session() -> MagicMock:
    session = MagicMock(spec=AsyncSession)
    session.begin.return_value.__aenter__ = AsyncMock(
        return_value=None,
    )
    session.begin.return_value.__aexit__ = AsyncMock(
        return_value=None,
    )
    return session


def make_statistic_payload(
    *,
    team_id: int = 33,
) -> dict[str, object]:
    return {
        "team": {
            "id": team_id,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
        },
        "statistics": [
            {
                "type": "Shots on Goal",
                "value": 5,
            },
            {
                "type": "Ball Possession",
                "value": "56%",
            },
            {
                "type": "expected_goals",
                "value": 1.42,
            },
        ],
    }


@pytest.mark.asyncio
async def test_sync_fixture_statistics_replaces_statistics() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    raw_entry = make_statistic_payload()
    client.get.return_value = {
        "response": [raw_entry],
    }

    context_mock = AsyncMock(
        return_value=(
            501,
            {
                33: 101,
                36: 102,
            },
        ),
    )
    replace_mock = AsyncMock(return_value=1)

    with (
        patch(
            "app.services.fixture_statistic_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_statistic_sync.replace_fixture_statistics",
            replace_mock,
        ),
    ):
        result = await sync_fixture_statistics(
            session,
            client,
            fixture_api_id=1208021,
        )

    client.get.assert_awaited_once_with(
        "/fixtures/statistics",
        params={
            "fixture": 1208021,
        },
    )
    context_mock.assert_awaited_once_with(
        session,
        1208021,
    )
    replace_mock.assert_awaited_once()

    assert result.statistics_synced == 1
    assert replace_mock.await_args.kwargs["fixture_id"] == 501

    rows = replace_mock.await_args.kwargs["statistic_rows"]
    assert len(rows) == 1

    row = rows[0]
    assert row["fixture_id"] == 501
    assert row["team_id"] == 101
    assert row["statistics"] == {
        "Shots on Goal": 5,
        "Ball Possession": "56%",
        "expected_goals": 1.42,
    }
    assert row["raw_payload"] == raw_entry


@pytest.mark.asyncio
async def test_sync_fixture_statistics_clears_old_statistics() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [],
    }

    context_mock = AsyncMock(
        return_value=(
            501,
            {
                33: 101,
                36: 102,
            },
        ),
    )
    replace_mock = AsyncMock(return_value=0)

    with (
        patch(
            "app.services.fixture_statistic_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_statistic_sync.replace_fixture_statistics",
            replace_mock,
        ),
    ):
        result = await sync_fixture_statistics(
            session,
            client,
            fixture_api_id=1208021,
        )

    assert result.statistics_synced == 0
    assert replace_mock.await_args.kwargs["statistic_rows"] == []


@pytest.mark.asyncio
async def test_sync_fixture_statistics_rejects_unknown_fixture() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [],
    }

    context_mock = AsyncMock(return_value=None)
    replace_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_statistic_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_statistic_sync.replace_fixture_statistics",
            replace_mock,
        ),
        pytest.raises(
            FixtureNotFoundError,
            match="Fixture 1208021 was not found",
        ),
    ):
        await sync_fixture_statistics(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()


@pytest.mark.asyncio
async def test_sync_fixture_statistics_rejects_unrelated_team() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [
            make_statistic_payload(
                team_id=999,
            ),
        ],
    }

    context_mock = AsyncMock(
        return_value=(
            501,
            {
                33: 101,
                36: 102,
            },
        ),
    )
    replace_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_statistic_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_statistic_sync.replace_fixture_statistics",
            replace_mock,
        ),
        pytest.raises(
            FixtureStatisticTeamNotFoundError,
            match="Team 999 does not belong to fixture 1208021",
        ),
    ):
        await sync_fixture_statistics(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()
