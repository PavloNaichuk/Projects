from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import APIFootballClient
from app.services.fixture_event_sync import (
    FixtureEventTeamNotFoundError,
    FixtureNotFoundError,
    sync_fixture_events,
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


def make_event_payload(
    *,
    team_id: int = 33,
) -> dict[str, object]:
    return {
        "time": {
            "elapsed": 45,
            "extra": 2,
        },
        "team": {
            "id": team_id,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
        },
        "player": {
            "id": 874,
            "name": "Bruno Fernandes",
        },
        "assist": {
            "id": 1000,
            "name": "Teammate",
        },
        "type": "Goal",
        "detail": "Normal Goal",
        "comments": None,
    }


@pytest.mark.asyncio
async def test_sync_fixture_events_replaces_events() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    raw_event = make_event_payload()
    client.get.return_value = {
        "response": [raw_event],
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
            "app.services.fixture_event_sync.get_fixture_event_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_event_sync.replace_fixture_events",
            replace_mock,
        ),
    ):
        result = await sync_fixture_events(
            session,
            client,
            fixture_api_id=1208021,
        )

    client.get.assert_awaited_once_with(
        "/fixtures/events",
        params={
            "fixture": 1208021,
        },
    )
    context_mock.assert_awaited_once_with(
        session,
        1208021,
    )
    replace_mock.assert_awaited_once()

    assert result.events_synced == 1
    assert replace_mock.await_args.kwargs["fixture_id"] == 501

    event_rows = replace_mock.await_args.kwargs["event_rows"]
    assert len(event_rows) == 1

    event_row = event_rows[0]
    assert event_row["fixture_id"] == 501
    assert event_row["team_id"] == 101
    assert event_row["event_order"] == 0
    assert event_row["elapsed"] == 45
    assert event_row["extra_time"] == 2
    assert event_row["player_api_id"] == 874
    assert event_row["event_type"] == "Goal"
    assert event_row["raw_payload"] == raw_event


@pytest.mark.asyncio
async def test_sync_fixture_events_clears_old_events() -> None:
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
            "app.services.fixture_event_sync.get_fixture_event_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_event_sync.replace_fixture_events",
            replace_mock,
        ),
    ):
        result = await sync_fixture_events(
            session,
            client,
            fixture_api_id=1208021,
        )

    assert result.events_synced == 0
    assert replace_mock.await_args.kwargs["event_rows"] == []


@pytest.mark.asyncio
async def test_sync_fixture_events_rejects_unknown_fixture() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [],
    }

    context_mock = AsyncMock(return_value=None)
    replace_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_event_sync.get_fixture_event_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_event_sync.replace_fixture_events",
            replace_mock,
        ),
        pytest.raises(
            FixtureNotFoundError,
            match="Fixture 1208021 was not found",
        ),
    ):
        await sync_fixture_events(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()


@pytest.mark.asyncio
async def test_sync_fixture_events_rejects_unrelated_team() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [
            make_event_payload(
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
            "app.services.fixture_event_sync.get_fixture_event_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_event_sync.replace_fixture_events",
            replace_mock,
        ),
        pytest.raises(
            FixtureEventTeamNotFoundError,
            match="Team 999 does not belong to fixture 1208021",
        ),
    ):
        await sync_fixture_events(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()
