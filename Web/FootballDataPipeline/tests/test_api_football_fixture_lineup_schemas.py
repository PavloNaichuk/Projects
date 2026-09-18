import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballFixtureLineupEntry,
)


def make_lineup_payload() -> dict[str, object]:
    return {
        "team": {
            "id": 33,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
            "colors": {
                "player": {
                    "primary": "FF0000",
                    "number": "FFFFFF",
                    "border": "000000",
                },
                "goalkeeper": {
                    "primary": "00FF00",
                    "number": "000000",
                    "border": "FFFFFF",
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


def test_fixture_lineup_schema_validates_payload() -> None:
    lineup = APIFootballFixtureLineupEntry.model_validate(
        make_lineup_payload(),
    )

    assert lineup.team.id == 33
    assert lineup.team.name == "Manchester United"
    assert lineup.coach.id == 19
    assert lineup.formation == "4-2-3-1"
    assert len(lineup.starting_xi) == 1
    assert lineup.starting_xi[0].player.id == 2935
    assert lineup.starting_xi[0].player.position == "D"
    assert len(lineup.substitutes) == 1
    assert lineup.substitutes[0].player.id == 284324


def test_fixture_lineup_schema_rejects_empty_team_name() -> None:
    payload = make_lineup_payload()
    payload["team"] = {
        "id": 33,
        "name": "",
        "logo": None,
        "colors": None,
    }

    with pytest.raises(ValidationError):
        APIFootballFixtureLineupEntry.model_validate(payload)
