from datetime import datetime, timezone
from types import SimpleNamespace
from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app


def make_fixture() -> SimpleNamespace:
    league = SimpleNamespace(
        api_id=39,
        name="Premier League",
    )
    season = SimpleNamespace(
        year=2024,
        league=league,
    )
    home_team = SimpleNamespace(
        api_id=33,
        name="Manchester United",
        logo_url="https://example.test/manchester-united.png",
    )
    away_team = SimpleNamespace(
        api_id=36,
        name="Fulham",
        logo_url="https://example.test/fulham.png",
    )

    return SimpleNamespace(
        api_id=1208021,
        season=season,
        home_team=home_team,
        away_team=away_team,
        referee="Robert Jones",
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
    )


def test_read_fixture_details_returns_complete_response() -> None:
    fixture = make_fixture()

    with (
        patch(
            "app.api.routes.fixture_details.get_fixture_by_api_id",
            new_callable=AsyncMock,
            return_value=fixture,
        ) as get_fixture,
        patch(
            "app.api.routes.fixture_details.list_fixture_events",
            new_callable=AsyncMock,
            return_value=[],
        ) as list_events,
        patch(
            "app.api.routes.fixture_details.list_fixture_statistics",
            new_callable=AsyncMock,
            return_value=[],
        ) as list_statistics,
        patch(
            "app.api.routes.fixture_details.list_fixture_lineups",
            new_callable=AsyncMock,
            return_value=[],
        ) as list_lineups,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/details",
            )

    assert response.status_code == 200

    response_data = response.json()

    assert response_data["fixture"]["api_id"] == 1208021
    assert response_data["fixture"]["season"] == {
        "year": 2024,
        "league": {
            "api_id": 39,
            "name": "Premier League",
        },
    }
    assert response_data["fixture"]["home_team"]["api_id"] == 33
    assert response_data["fixture"]["away_team"]["api_id"] == 36
    assert response_data["fixture"]["home_goals"] == 1
    assert response_data["fixture"]["away_goals"] == 0
    assert response_data["events"] == []
    assert response_data["statistics"] == []
    assert response_data["lineups"] == []

    get_fixture.assert_awaited_once()
    list_events.assert_awaited_once()
    list_statistics.assert_awaited_once()
    list_lineups.assert_awaited_once()

    assert get_fixture.await_args.args[1] == 1208021
    assert list_events.await_args.args[1] == 1208021
    assert list_statistics.await_args.args[1] == 1208021
    assert list_lineups.await_args.args[1] == 1208021


def test_read_fixture_details_returns_404() -> None:
    with (
        patch(
            "app.api.routes.fixture_details.get_fixture_by_api_id",
            new_callable=AsyncMock,
            return_value=None,
        ),
        patch(
            "app.api.routes.fixture_details.list_fixture_events",
            new_callable=AsyncMock,
        ) as list_events,
        patch(
            "app.api.routes.fixture_details.list_fixture_statistics",
            new_callable=AsyncMock,
        ) as list_statistics,
        patch(
            "app.api.routes.fixture_details.list_fixture_lineups",
            new_callable=AsyncMock,
        ) as list_lineups,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/999999/details",
            )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Fixture not found",
    }
    list_events.assert_not_awaited()
    list_statistics.assert_not_awaited()
    list_lineups.assert_not_awaited()
