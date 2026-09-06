from datetime import date

import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballLeague,
    APIFootballLeagueEntry,
)


def test_league_entry_is_validated() -> None:
    payload = {
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
                "year": 2025,
                "start": "2025-08-15",
                "end": "2026-05-24",
                "current": True,
                "coverage": {
                    "fixtures": {
                        "events": True,
                        "lineups": True,
                    },
                    "standings": True,
                },
            },
        ],
    }

    entry = APIFootballLeagueEntry.model_validate(payload)

    assert entry.league.id == 39
    assert entry.league.name == "Premier League"
    assert entry.country.code == "GB-ENG"
    assert entry.seasons[0].year == 2025
    assert entry.seasons[0].start == date(2025, 8, 15)
    assert entry.seasons[0].current is True


def test_league_rejects_invalid_api_id() -> None:
    payload = {
        "id": 0,
        "name": "Premier League",
        "type": "League",
        "logo": None,
    }

    with pytest.raises(ValidationError):
        APIFootballLeague.model_validate(payload)
