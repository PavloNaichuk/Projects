import pytest
from pydantic import ValidationError

from app.schemas.api_football import APIFootballFixtureEvent


def make_event_payload() -> dict[str, object]:
    return {
        "time": {
            "elapsed": 45,
            "extra": 2,
        },
        "team": {
            "id": 33,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
        },
        "player": {
            "id": 874,
            "name": "Bruno Fernandes",
        },
        "assist": {
            "id": None,
            "name": None,
        },
        "type": "Goal",
        "detail": "Normal Goal",
        "comments": None,
    }


def test_fixture_event_schema_validates_payload() -> None:
    event = APIFootballFixtureEvent.model_validate(
        make_event_payload(),
    )

    assert event.time.elapsed == 45
    assert event.time.extra == 2
    assert event.team.id == 33
    assert event.player.id == 874
    assert event.player.name == "Bruno Fernandes"
    assert event.assist.id is None
    assert event.type == "Goal"
    assert event.detail == "Normal Goal"


def test_fixture_event_schema_rejects_negative_elapsed_time() -> None:
    payload = make_event_payload()
    payload["time"] = {
        "elapsed": -1,
        "extra": None,
    }

    with pytest.raises(ValidationError):
        APIFootballFixtureEvent.model_validate(payload)
