from datetime import datetime, timezone

import pytest
from pydantic import ValidationError

from app.schemas.api_football import (
    APIFootballStandingEntry,
    APIFootballStandingsResponseEntry,
)


def make_stats(
    *,
    played: int,
    win: int,
    draw: int,
    lose: int,
    goals_for: int,
    goals_against: int,
) -> dict[str, object]:
    return {
        "played": played,
        "win": win,
        "draw": draw,
        "lose": lose,
        "goals": {
            "for": goals_for,
            "against": goals_against,
        },
    }


def make_standing_payload() -> dict[str, object]:
    standing = {
        "rank": 1,
        "team": {
            "id": 42,
            "name": "Arsenal",
            "logo": "https://example.test/arsenal.png",
        },
        "points": 89,
        "goalsDiff": 62,
        "group": "Premier League",
        "form": "WWDWW",
        "status": "same",
        "description": "Promotion - Champions League",
        "all": make_stats(
            played=38,
            win=28,
            draw=5,
            lose=5,
            goals_for=91,
            goals_against=29,
        ),
        "home": make_stats(
            played=19,
            win=15,
            draw=2,
            lose=2,
            goals_for=48,
            goals_against=16,
        ),
        "away": make_stats(
            played=19,
            win=13,
            draw=3,
            lose=3,
            goals_for=43,
            goals_against=13,
        ),
        "update": "2025-05-25T18:00:00+00:00",
    }

    return {
        "league": {
            "id": 39,
            "name": "Premier League",
            "country": "England",
            "logo": "https://example.test/premier-league.png",
            "flag": "https://example.test/england.svg",
            "season": 2024,
            "standings": [
                [standing],
            ],
        },
    }


def test_standings_response_is_validated() -> None:
    response_entry = APIFootballStandingsResponseEntry.model_validate(
        make_standing_payload(),
    )

    league = response_entry.league
    standing = league.standings[0][0]

    assert league.id == 39
    assert league.season == 2024
    assert standing.rank == 1
    assert standing.team.id == 42
    assert standing.points == 89
    assert standing.goals_diff == 62
    assert standing.all.played == 38
    assert standing.all.goals.goals_for == 91
    assert standing.all.goals.against == 29
    assert standing.update == datetime(
        2025,
        5,
        25,
        18,
        0,
        tzinfo=timezone.utc,
    )


def test_standing_rejects_invalid_rank() -> None:
    response_payload = make_standing_payload()
    league = response_payload["league"]
    assert isinstance(league, dict)

    groups = league["standings"]
    assert isinstance(groups, list)

    group = groups[0]
    assert isinstance(group, list)

    standing = group[0]
    assert isinstance(standing, dict)
    standing["rank"] = 0

    with pytest.raises(ValidationError):
        APIFootballStandingEntry.model_validate(standing)
