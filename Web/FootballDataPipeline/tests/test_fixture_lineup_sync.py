from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import APIFootballClient
from app.services.fixture_lineup_sync import (
    FixtureLineupTeamNotFoundError,
    FixtureNotFoundError,
    sync_fixture_lineups,
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


def make_lineup_payload(
    *,
    team_id: int = 33,
) -> dict[str, object]:
    return {
        "team": {
            "id": team_id,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
            "colors": {
                "player": {
                    "primary": "FF0000",
                    "number": "FFFFFF",
                    "border": "000000",
                },
            },
        },
        "coach": {
            "id": 19,
            "name": "Erik ten Hag",
            "photo": "https://example.test/coach.png",
        },
        "formation": "4-2-3-1",
        "startXI": [
            {
                "player": {
                    "id": 2935,
                    "name": "Harry Maguire",
                    "number": 5,
                    "pos": "D",
                    "grid": "2:2",
                },
            },
        ],
        "substitutes": [
            {
                "player": {
                    "id": 284324,
                    "name": "A. Garnacho",
                    "number": 17,
                    "pos": "F",
                    "grid": None,
                },
            },
        ],
    }


@pytest.mark.asyncio
async def test_sync_fixture_lineups_replaces_lineups() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    raw_entry = make_lineup_payload()
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
            "app.services.fixture_lineup_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_lineup_sync.replace_fixture_lineups",
            replace_mock,
        ),
    ):
        result = await sync_fixture_lineups(
            session,
            client,
            fixture_api_id=1208021,
        )

    client.get.assert_awaited_once_with(
        "/fixtures/lineups",
        params={
            "fixture": 1208021,
        },
    )
    context_mock.assert_awaited_once_with(
        session,
        1208021,
    )
    replace_mock.assert_awaited_once()

    assert result.lineups_synced == 1
    assert replace_mock.await_args.kwargs["fixture_id"] == 501

    rows = replace_mock.await_args.kwargs["lineup_rows"]
    assert len(rows) == 1

    row = rows[0]
    assert row["fixture_id"] == 501
    assert row["team_id"] == 101
    assert row["formation"] == "4-2-3-1"
    assert row["coach_api_id"] == 19
    assert row["coach_name"] == "Erik ten Hag"
    assert row["starting_xi"] == [
        {
            "id": 2935,
            "name": "Harry Maguire",
            "number": 5,
            "pos": "D",
            "grid": "2:2",
        },
    ]
    assert row["substitutes"][0]["id"] == 284324
    assert row["raw_payload"] == raw_entry


@pytest.mark.asyncio
async def test_sync_fixture_lineups_clears_old_lineups() -> None:
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
            "app.services.fixture_lineup_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_lineup_sync.replace_fixture_lineups",
            replace_mock,
        ),
    ):
        result = await sync_fixture_lineups(
            session,
            client,
            fixture_api_id=1208021,
        )

    assert result.lineups_synced == 0
    assert replace_mock.await_args.kwargs["lineup_rows"] == []


@pytest.mark.asyncio
async def test_sync_fixture_lineups_rejects_unknown_fixture() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [],
    }

    context_mock = AsyncMock(return_value=None)
    replace_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_lineup_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_lineup_sync.replace_fixture_lineups",
            replace_mock,
        ),
        pytest.raises(
            FixtureNotFoundError,
            match="Fixture 1208021 was not found",
        ),
    ):
        await sync_fixture_lineups(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()


@pytest.mark.asyncio
async def test_sync_fixture_lineups_rejects_unrelated_team() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [
            make_lineup_payload(
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
            "app.services.fixture_lineup_sync.get_fixture_context",
            context_mock,
        ),
        patch(
            "app.services.fixture_lineup_sync.replace_fixture_lineups",
            replace_mock,
        ),
        pytest.raises(
            FixtureLineupTeamNotFoundError,
            match="Team 999 does not belong to fixture 1208021",
        ),
    ):
        await sync_fixture_lineups(
            session,
            client,
            fixture_api_id=1208021,
        )

    replace_mock.assert_not_awaited()
