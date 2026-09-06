from unittest.mock import MagicMock, patch

import pytest
from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.schemas.api_football import APIFootballLeagueEntry
from app.services.league_sync import sync_leagues


def make_league_payload() -> dict[str, object]:
    return {
        "league": {
            "id": 39,
            "name": "Premier League",
            "type": "League",
            "logo": "https://example.test/premier-league.png",
        },
        "country": {
            "name": "England",
            "code": "GB-ENG",
            "flag": "https://example.test/england.svg",
        },
        "seasons": [
            {
                "year": 2024,
                "start": "2024-08-16",
                "end": "2025-05-25",
                "current": False,
                "coverage": {
                    "fixtures": {
                        "events": True,
                    },
                    "standings": True,
                },
            },
        ],
    }


@pytest.mark.asyncio
async def test_sync_leagues_saves_valid_entries() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [make_league_payload()],
    }

    with patch(
        "app.services.league_sync.upsert_league_entry",
        autospec=True,
    ) as upsert:
        upsert.return_value = 1

        result = await sync_leagues(session, client)

    client.get.assert_awaited_once_with(
        "/leagues",
        params=None,
    )
    session.begin.assert_called_once_with()
    upsert.assert_awaited_once()

    saved_entry = upsert.await_args.args[1]

    assert isinstance(saved_entry, APIFootballLeagueEntry)
    assert saved_entry.league.id == 39
    assert result.leagues_synced == 1
    assert result.seasons_synced == 1


@pytest.mark.asyncio
async def test_sync_leagues_rejects_invalid_response() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": None,
    }

    with pytest.raises(
        APIFootballResponseError,
        match="does not contain a leagues list",
    ):
        await sync_leagues(session, client)

    session.begin.assert_not_called()


@pytest.mark.asyncio
async def test_sync_leagues_passes_filters() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)
    client.get.return_value = {
        "response": [],
    }

    result = await sync_leagues(
        session,
        client,
        league_id=39,
        current_only=True,
    )

    client.get.assert_awaited_once_with(
        "/leagues",
        params={
            "id": 39,
            "current": "true",
        },
    )
    assert result.leagues_synced == 0
    assert result.seasons_synced == 0


@pytest.mark.asyncio
async def test_sync_leagues_rejects_invalid_league_id() -> None:
    session = MagicMock(spec=AsyncSession)
    client = MagicMock(spec=APIFootballClient)

    with pytest.raises(
        ValueError,
        match="League ID must be greater than zero",
    ):
        await sync_leagues(
            session,
            client,
            league_id=0,
        )

    client.get.assert_not_awaited()
    session.begin.assert_not_called()
