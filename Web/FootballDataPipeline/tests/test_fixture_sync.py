from unittest.mock import AsyncMock, MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import APIFootballClient
from app.services.fixture_sync import (
    FixtureDependencyError,
    sync_fixtures,
)


def make_fixture_payload() -> dict[str, object]:
    return {
        "fixture": {
            "id": 1208028,
            "referee": "Robert Jones, England",
            "timezone": "UTC",
            "date": "2024-08-16T19:00:00+00:00",
            "venue": {
                "id": 556,
                "name": "Old Trafford",
                "city": "Manchester",
            },
            "status": {
                "long": "Match Finished",
                "short": "FT",
                "elapsed": 90,
                "extra": None,
            },
        },
        "league": {
            "id": 39,
            "season": 2024,
            "round": "Regular Season - 1",
        },
        "teams": {
            "home": {
                "id": 33,
                "name": "Manchester United",
                "logo": None,
                "winner": True,
            },
            "away": {
                "id": 36,
                "name": "Fulham",
                "logo": None,
                "winner": False,
            },
        },
        "goals": {
            "home": 1,
            "away": 0,
        },
        "score": {
            "halftime": {
                "home": 0,
                "away": 0,
            },
            "fulltime": {
                "home": 1,
                "away": 0,
            },
            "extratime": {
                "home": None,
                "away": None,
            },
            "penalty": {
                "home": None,
                "away": None,
            },
        },
    }


def make_session() -> MagicMock:
    session = MagicMock(spec=AsyncSession)
    session.begin.return_value.__aenter__ = AsyncMock()
    session.begin.return_value.__aexit__ = AsyncMock(
        return_value=False,
    )
    return session


@pytest.mark.asyncio
async def test_sync_fixtures() -> None:
    raw_fixture = make_fixture_payload()
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [raw_fixture],
    }

    get_season_id_mock = AsyncMock(return_value=17)
    get_team_ids_mock = AsyncMock(
        return_value={
            33: 101,
            36: 102,
        },
    )
    upsert_fixtures_mock = AsyncMock(return_value=1)

    with (
        patch(
            "app.services.fixture_sync.get_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.fixture_sync.get_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.fixture_sync.upsert_fixtures",
            upsert_fixtures_mock,
        ),
    ):
        result = await sync_fixtures(
            session,
            client,
            league_id=39,
            season=2024,
        )

    assert result.fixtures_synced == 1

    client.get.assert_awaited_once_with(
        "/fixtures",
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
        {33, 36},
    )

    fixture_rows = upsert_fixtures_mock.await_args.args[1]
    fixture_row = fixture_rows[0]

    assert fixture_row["api_id"] == 1208028
    assert fixture_row["season_id"] == 17
    assert fixture_row["home_team_id"] == 101
    assert fixture_row["away_team_id"] == 102
    assert fixture_row["status_short"] == "FT"
    assert fixture_row["home_goals"] == 1
    assert fixture_row["away_goals"] == 0
    assert fixture_row["raw_payload"] is raw_fixture


@pytest.mark.asyncio
async def test_sync_fixtures_requires_existing_season() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_fixture_payload()],
    }

    get_season_id_mock = AsyncMock(return_value=None)
    get_team_ids_mock = AsyncMock()
    upsert_fixtures_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_sync.get_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.fixture_sync.get_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.fixture_sync.upsert_fixtures",
            upsert_fixtures_mock,
        ),
        pytest.raises(
            FixtureDependencyError,
            match="Synchronize leagues first",
        ),
    ):
        await sync_fixtures(
            session,
            client,
            league_id=39,
            season=2024,
        )

    get_team_ids_mock.assert_not_awaited()
    upsert_fixtures_mock.assert_not_awaited()


@pytest.mark.asyncio
async def test_sync_fixtures_requires_existing_teams() -> None:
    session = make_session()
    client = AsyncMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_fixture_payload()],
    }

    get_season_id_mock = AsyncMock(return_value=17)
    get_team_ids_mock = AsyncMock(
        return_value={
            33: 101,
        },
    )
    upsert_fixtures_mock = AsyncMock()

    with (
        patch(
            "app.services.fixture_sync.get_season_id",
            get_season_id_mock,
        ),
        patch(
            "app.services.fixture_sync.get_team_ids",
            get_team_ids_mock,
        ),
        patch(
            "app.services.fixture_sync.upsert_fixtures",
            upsert_fixtures_mock,
        ),
        pytest.raises(
            FixtureDependencyError,
            match="36",
        ),
    ):
        await sync_fixtures(
            session,
            client,
            league_id=39,
            season=2024,
        )

    upsert_fixtures_mock.assert_not_awaited()
