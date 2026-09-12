from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import APIFootballClient
from app.services.standing_sync import (
    StandingDependencyError,
    sync_standings,
)
from tests.test_api_football_standing_schemas import (
    make_standing_payload,
)


def make_session() -> MagicMock:
    session = MagicMock(spec=AsyncSession)
    session.begin.return_value.__aenter__ = AsyncMock()
    session.begin.return_value.__aexit__ = AsyncMock(
        return_value=False,
    )
    return session


@pytest.mark.asyncio
async def test_sync_standings() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_standing_payload()],
    }

    get_season_id_mock = AsyncMock(return_value=17)
    get_team_ids_mock = AsyncMock(
        return_value={
            42: 101,
        },
    )
    upsert_standings_mock = AsyncMock(return_value=1)

    with (
        patch(
            "app.services.standing_sync.get_standing_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.standing_sync.get_standing_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.standing_sync.upsert_standings",
            upsert_standings_mock,
        ),
    ):
        result = await sync_standings(
            session,
            client,
            league_id=39,
            season=2024,
        )

    assert result.standings_synced == 1

    client.get.assert_awaited_once_with(
        "/standings",
        params={
            "league": 39,
            "season": 2024,
        },
    )
    get_season_id_mock.assert_awaited_once_with(
        session,
        league_api_id=39,
        season_year=2024,
    )
    get_team_ids_mock.assert_awaited_once_with(
        session,
        {42},
    )

    standing_rows = upsert_standings_mock.await_args.args[1]
    standing_row = standing_rows[0]

    assert standing_row["season_id"] == 17
    assert standing_row["team_id"] == 101
    assert standing_row["position"] == 1
    assert standing_row["points"] == 89
    assert standing_row["goals_diff"] == 62
    assert standing_row["played"] == 38
    assert standing_row["wins"] == 28
    assert standing_row["draws"] == 5
    assert standing_row["losses"] == 5
    assert standing_row["goals_for"] == 91
    assert standing_row["goals_against"] == 29

    raw_payload = standing_row["raw_payload"]
    assert isinstance(raw_payload, dict)
    assert raw_payload["goalsDiff"] == 62

    upsert_standings_mock.assert_awaited_once()


@pytest.mark.asyncio
async def test_sync_standings_requires_existing_season() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_standing_payload()],
    }

    get_season_id_mock = AsyncMock(return_value=None)
    get_team_ids_mock = AsyncMock()
    upsert_standings_mock = AsyncMock()

    with (
        patch(
            "app.services.standing_sync.get_standing_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.standing_sync.get_standing_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.standing_sync.upsert_standings",
            upsert_standings_mock,
        ),
        pytest.raises(
            StandingDependencyError,
            match="Synchronize leagues first",
        ),
    ):
        await sync_standings(
            session,
            client,
            league_id=39,
            season=2024,
        )

    get_team_ids_mock.assert_not_awaited()
    upsert_standings_mock.assert_not_awaited()


@pytest.mark.asyncio
async def test_sync_standings_requires_existing_teams() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_standing_payload()],
    }

    get_season_id_mock = AsyncMock(return_value=17)
    get_team_ids_mock = AsyncMock(return_value={})
    upsert_standings_mock = AsyncMock()

    with (
        patch(
            "app.services.standing_sync.get_standing_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.standing_sync.get_standing_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.standing_sync.upsert_standings",
            upsert_standings_mock,
        ),
        pytest.raises(
            StandingDependencyError,
            match="42",
        ),
    ):
        await sync_standings(
            session,
            client,
            league_id=39,
            season=2024,
        )

    upsert_standings_mock.assert_not_awaited()
