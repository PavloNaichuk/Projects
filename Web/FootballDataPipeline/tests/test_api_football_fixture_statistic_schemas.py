import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballFixtureStatisticEntry,
)


def make_statistic_payload() -> dict[str, object]:
    return {
        "team": {
            "id": 33,
            "name": "Manchester United",
            "logo": "https://example.test/manchester-united.png",
        },
        "statistics": [
            {
                "type": "Shots on Goal",
                "value": 5,
            },
            {
                "type": "Ball Possession",
                "value": "56%",
            },
            {
                "type": "expected_goals",
                "value": 1.42,
            },
            {
                "type": "Blocked Shots",
                "value": None,
            },
        ],
    }


def test_fixture_statistic_schema_validates_payload() -> None:
    entry = APIFootballFixtureStatisticEntry.model_validate(
        make_statistic_payload(),
    )

    assert entry.team.id == 33
    assert entry.team.name == "Manchester United"
    assert len(entry.statistics) == 4
    assert entry.statistics[0].type == "Shots on Goal"
    assert entry.statistics[0].value == 5
    assert entry.statistics[1].value == "56%"
    assert entry.statistics[2].value == 1.42
    assert entry.statistics[3].value is None


def test_fixture_statistic_schema_rejects_empty_type() -> None:
    payload = make_statistic_payload()
    payload["statistics"] = [
        {
            "type": "",
            "value": 5,
        },
    ]

    with pytest.raises(ValidationError):
        APIFootballFixtureStatisticEntry.model_validate(
            payload,
        )
