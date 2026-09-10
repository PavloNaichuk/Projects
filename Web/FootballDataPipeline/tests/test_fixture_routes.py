from datetime import date, datetime, timezone
from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import Fixture, League, Season, Team


def make_fixture() -> Fixture:
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
    home_team = Team(
        api_id=33,
        name="Manchester United",
        code="MUN",
        country_name="England",
        founded=1878,
        is_national=False,
        logo_url="https://example.test/manchester-united.png",
    )
    away_team = Team(
        api_id=36,
        name="Fulham",
        code="FUL",
        country_name="England",
        founded=1879,
        is_national=False,
        logo_url="https://example.test/fulham.png",
    )

    return Fixture(
        api_id=1208021,
        season=season,
        home_team=home_team,
        away_team=away_team,
        referee="Robert Jones, England",
        kickoff_at=datetime(
            2024,
            8,
            16,
            19,
            0,
            tzinfo=timezone.utc,
        ),
        timezone="UTC",
        round_name="Regular Season - 1",
        venue_api_id=556,
        venue_name="Old Trafford",
        venue_city="Manchester",
        status_long="Match Finished",
        status_short="FT",
        elapsed=90,
        extra_time=None,
        home_goals=1,
        away_goals=0,
        halftime_home=0,
        halftime_away=0,
        fulltime_home=1,
        fulltime_away=0,
        extra_home=None,
        extra_away=None,
        penalty_home=None,
        penalty_away=None,
        raw_payload={},
    )


def test_read_fixtures_returns_filtered_list() -> None:
    fixture = make_fixture()

    with patch(
        "app.api.routes.fixtures.list_fixtures",
        new_callable=AsyncMock,
        return_value=[fixture],
    ) as list_query:
        with TestClient(app) as client:
            response = client.get(
                "/fixtures",
                params={
                    "offset": 0,
                    "limit": 10,
                    "league_id": 39,
                    "season": 2024,
                    "team_id": 33,
                    "status": "ft",
                    "date_from": "2024-08-16",
                    "date_to": "2024-08-16",
                },
            )

    assert response.status_code == 200

    fixture_data = response.json()[0]

    assert fixture_data["api_id"] == 1208021
    assert fixture_data["season"] == {
        "year": 2024,
        "league": {
            "api_id": 39,
            "name": "Premier League",
        },
    }
    assert fixture_data["home_team"]["api_id"] == 33
    assert fixture_data["away_team"]["api_id"] == 36
    assert fixture_data["status_short"] == "FT"
    assert fixture_data["home_goals"] == 1
    assert fixture_data["away_goals"] == 0

    list_query.assert_awaited_once()
    assert list_query.await_args.kwargs == {
        "offset": 0,
        "limit": 10,
        "league_id": 39,
        "season": 2024,
        "team_id": 33,
        "status_short": "ft",
        "date_from": date(2024, 8, 16),
        "date_to": date(2024, 8, 16),
    }


def test_read_fixture_returns_fixture() -> None:
    fixture = make_fixture()

    with patch(
        "app.api.routes.fixtures.get_fixture_by_api_id",
        new_callable=AsyncMock,
        return_value=fixture,
    ) as get_query:
        with TestClient(app) as client:
            response = client.get("/fixtures/1208021")

    assert response.status_code == 200

    fixture_data = response.json()

    assert fixture_data["api_id"] == 1208021
    assert fixture_data["home_team"]["name"] == "Manchester United"
    assert fixture_data["away_team"]["name"] == "Fulham"
    assert fixture_data["venue_name"] == "Old Trafford"
    assert fixture_data["status_short"] == "FT"

    get_query.assert_awaited_once()
    assert get_query.await_args.args[1] == 1208021


def test_read_fixture_returns_404() -> None:
    with patch(
        "app.api.routes.fixtures.get_fixture_by_api_id",
        new_callable=AsyncMock,
        return_value=None,
    ):
        with TestClient(app) as client:
            response = client.get("/fixtures/999999")

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Fixture not found",
    }


def test_read_fixtures_rejects_invalid_date_range() -> None:
    with patch(
        "app.api.routes.fixtures.list_fixtures",
        new_callable=AsyncMock,
    ) as list_query:
        with TestClient(app) as client:
            response = client.get(
                "/fixtures",
                params={
                    "date_from": "2024-08-18",
                    "date_to": "2024-08-16",
                },
            )

    assert response.status_code == 422
    assert response.json() == {
        "detail": "date_from must not be later than date_to",
    }
    list_query.assert_not_awaited()
