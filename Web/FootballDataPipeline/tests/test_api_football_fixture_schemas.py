from datetime import datetime, timezone

import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballFixtureEntry,
    APIFootballFixtureInfo,
)


def make_fixture_payload() -> dict[str, object]:
    return {
        "fixture": {
            "id": 1208028,
            "referee": "Robert Jones, England",
            "timezone": "UTC",
            "date": "2024-08-16T19:00:00+00:00",
            "timestamp": 1723834800,
            "periods": {
                "first": 1723834800,
                "second": 1723838400,
            },
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
            "name": "Premier League",
            "season": 2024,
            "round": "Regular Season - 1",
        },
        "teams": {
            "home": {
                "id": 33,
                "name": "Manchester United",
                "logo": "https://example.test/manchester-united.png",
                "winner": True,
            },
            "away": {
                "id": 36,
                "name": "Fulham",
                "logo": "https://example.test/fulham.png",
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


def test_fixture_entry_is_validated() -> None:
    entry = APIFootballFixtureEntry.model_validate(
        make_fixture_payload(),
    )

    assert entry.fixture.id == 1208028
    assert entry.fixture.date == datetime(
        2024,
        8,
        16,
        19,
        0,
        tzinfo=timezone.utc,
    )
    assert entry.league.id == 39
    assert entry.league.season == 2024
    assert entry.teams.home.id == 33
    assert entry.teams.away.id == 36
    assert entry.goals.home == 1
    assert entry.goals.away == 0
    assert entry.fixture.status.short == "FT"


def test_fixture_rejects_invalid_api_id() -> None:
    payload = make_fixture_payload()["fixture"]
    assert isinstance(payload, dict)
    payload["id"] = 0

    with pytest.raises(ValidationError):
        APIFootballFixtureInfo.model_validate(payload)
