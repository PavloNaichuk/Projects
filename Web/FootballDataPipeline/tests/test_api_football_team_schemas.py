import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballTeam,
    APIFootballTeamEntry,
)


def test_team_entry_is_validated() -> None:
    payload = {
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
            "city": "Manchester",
        },
    }

    entry = APIFootballTeamEntry.model_validate(payload)

    assert entry.team.id == 33
    assert entry.team.name == "Manchester United"
    assert entry.team.code == "MUN"
    assert entry.team.country == "England"
    assert entry.team.founded == 1878
    assert entry.team.national is False


def test_team_rejects_invalid_api_id() -> None:
    payload = {
        "id": 0,
        "name": "Manchester United",
        "code": "MUN",
        "country": "England",
        "founded": 1878,
        "national": False,
        "logo": None,
    }

    with pytest.raises(ValidationError):
        APIFootballTeam.model_validate(payload)
