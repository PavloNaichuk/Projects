from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import League, Season, Standing, Team


def make_standing() -> Standing:
    league = League(
        api_id=39,
        name="Premier League",
        league_type="League",
        country_name="England",
    )
    season = Season(
        year=2024,
        is_current=False,
        league=league,
    )
    team = Team(
        api_id=40,
        name="Liverpool",
        code="LIV",
        country_name="England",
        founded=1892,
        is_national=False,
        logo_url="https://example.test/liverpool.png",
    )

    return Standing(
        season=season,
        team=team,
        position=1,
        points=84,
        goals_diff=45,
        group_name="Premier League",
        form="WWDWW",
        status="same",
        description="Champions League",
        played=38,
        wins=25,
        draws=9,
        losses=4,
        goals_for=86,
        goals_against=41,
        home_stats={},
        away_stats={},
        raw_payload={},
    )


def test_read_standings_returns_filtered_list() -> None:
    standing = make_standing()

    with patch(
        "app.api.routes.standings.list_standings",
        new_callable=AsyncMock,
        return_value=[standing],
    ) as list_query:
        with TestClient(app) as client:
            response = client.get(
                "/standings",
                params={
                    "offset": 0,
                    "limit": 20,
                    "league_id": 39,
                    "season": 2024,
                    "team_id": 40,
                },
            )

    assert response.status_code == 200

    data = response.json()

    assert len(data) == 1
    assert data[0]["position"] == 1
    assert data[0]["points"] == 84
    assert data[0]["season"]["year"] == 2024
    assert data[0]["season"]["league"]["api_id"] == 39
    assert data[0]["team"]["api_id"] == 40
    assert data[0]["team"]["name"] == "Liverpool"

    list_query.assert_awaited_once()
    assert list_query.await_args.kwargs == {
        "offset": 0,
        "limit": 20,
        "league_id": 39,
        "season": 2024,
        "team_id": 40,
    }


def test_read_league_standings_returns_table() -> None:
    standing = make_standing()

    with patch(
        "app.api.routes.standings.get_league_standings",
        new_callable=AsyncMock,
        return_value=[standing],
    ) as get_query:
        with TestClient(app) as client:
            response = client.get("/standings/39/2024")

    assert response.status_code == 200
    assert response.json()[0]["team"]["name"] == "Liverpool"
    assert response.json()[0]["position"] == 1

    get_query.assert_awaited_once()
    assert get_query.await_args.kwargs == {
        "league_id": 39,
        "season": 2024,
    }


def test_read_league_standings_returns_404() -> None:
    with patch(
        "app.api.routes.standings.get_league_standings",
        new_callable=AsyncMock,
        return_value=[],
    ):
        with TestClient(app) as client:
            response = client.get("/standings/39/2024")

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Standings not found",
    }
