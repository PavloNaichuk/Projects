from unittest.mock import MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.schemas.api_football import APIFootballTeam
from app.services.team_sync import sync_teams


def make_team_payload() -> dict[str, object]:
    return {
        "team": {
            "id": 33,
            "name": "Manchester United",
            "code": "MUN",
            "country": "England",
            "founded": 1878,
            "national": False,
            "logo": "https://example.test/manchester-united.png",
        },
        "venue": {
            "id": 556,
            "name": "Old Trafford",
        },
    }


@pytest.mark.asyncio
async def test_sync_teams_saves_valid_entries() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_team_payload()],
    }

    with patch(
        "app.services.team_sync.upsert_teams",
        autospec=True,
    ) as upsert:
        upsert.return_value = 1

        result = await sync_teams(
            session,
            client,
            league_id=39,
            season=2026,
        )

    client.get.assert_awaited_once_with(
        "/teams",
        params={
            "league": 39,
            "season": 2026,
        },
    )
    session.begin.assert_called_once_with()
    upsert.assert_awaited_once()

    saved_teams = upsert.await_args.args[1]

    assert len(saved_teams) == 1
    assert isinstance(saved_teams[0], APIFootballTeam)
    assert saved_teams[0].id == 33
    assert saved_teams[0].name == "Manchester United"
    assert result.teams_synced == 1


@pytest.mark.asyncio
async def test_sync_teams_rejects_invalid_response() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": None,
    }

    with pytest.raises(
        APIFootballResponseError,
        match="does not contain a teams list",
    ):
        await sync_teams(
            session,
            client,
            league_id=39,
            season=2026,
        )

    session.begin.assert_not_called()


@pytest.mark.parametrize(
    ("league_id", "season", "error_message"),
    [
        (0, 2026, "League ID must be greater than zero"),
        (39, 0, "Season must be greater than zero"),
    ],
)
@pytest.mark.asyncio
async def test_sync_teams_rejects_invalid_parameters(
    league_id: int,
    season: int,
    error_message: str,
) -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)

    with pytest.raises(
        ValueError,
        match=error_message,
    ):
        await sync_teams(
            session,
            client,
            league_id=league_id,
            season=season,
        )

    client.get.assert_not_awaited()
    session.begin.assert_not_called()
