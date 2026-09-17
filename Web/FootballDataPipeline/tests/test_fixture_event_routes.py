from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import FixtureEvent, Team


def make_fixture_event() -> FixtureEvent:
    team = Team(
        api_id=33,
        name="Manchester United",
        code="MUN",
        country_name="England",
        founded=1878,
        is_national=False,
        logo_url="https://example.test/manchester-united.png",
    )

    return FixtureEvent(
        fixture_id=501,
        team_id=101,
        team=team,
        event_order=0,
        elapsed=18,
        extra_time=None,
        player_api_id=2935,
        player_name="Mason Mount",
        assist_api_id=None,
        assist_name=None,
        event_type="Card",
        detail="Yellow Card",
        comments=None,
        raw_payload={},
    )


def test_read_fixture_events_returns_timeline() -> None:
    event = make_fixture_event()

    with (
        patch(
            "app.api.routes.fixture_events.list_fixture_events",
            new_callable=AsyncMock,
            return_value=[event],
        ) as list_query,
        patch(
            "app.api.routes.fixture_events.fixture_exists",
            new_callable=AsyncMock,
        ) as exists_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/events",
            )

    assert response.status_code == 200
    assert response.json() == [
        {
            "event_order": 0,
            "elapsed": 18,
            "extra_time": None,
            "team": {
                "api_id": 33,
                "name": "Manchester United",
                "logo_url": ("https://example.test/manchester-united.png"),
            },
            "player_api_id": 2935,
            "player_name": "Mason Mount",
            "assist_api_id": None,
            "assist_name": None,
            "event_type": "Card",
            "detail": "Yellow Card",
            "comments": None,
        },
    ]

    list_query.assert_awaited_once()
    assert list_query.await_args.args[1] == 1208021
    exists_query.assert_not_awaited()


def test_read_fixture_events_returns_empty_list() -> None:
    with (
        patch(
            "app.api.routes.fixture_events.list_fixture_events",
            new_callable=AsyncMock,
            return_value=[],
        ),
        patch(
            "app.api.routes.fixture_events.fixture_exists",
            new_callable=AsyncMock,
            return_value=True,
        ) as exists_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/events",
            )

    assert response.status_code == 200
    assert response.json() == []
    exists_query.assert_awaited_once()


def test_read_fixture_events_returns_404() -> None:
    with (
        patch(
            "app.api.routes.fixture_events.list_fixture_events",
            new_callable=AsyncMock,
            return_value=[],
        ),
        patch(
            "app.api.routes.fixture_events.fixture_exists",
            new_callable=AsyncMock,
            return_value=False,
        ),
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/999999/events",
            )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Fixture not found",
    }
