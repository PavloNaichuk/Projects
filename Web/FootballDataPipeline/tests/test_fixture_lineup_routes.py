from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import FixtureLineup, Team


def make_lineup() -> FixtureLineup:
    team = Team(
        api_id=33,
        name="Manchester United",
        code="MUN",
        country_name="England",
        founded=1878,
        is_national=False,
        logo_url="https://example.test/manchester-united.png",
    )

    return FixtureLineup(
        fixture_id=10,
        team_id=20,
        team=team,
        formation="4-2-3-1",
        coach_api_id=19,
        coach_name="E. ten Hag",
        coach_photo_url="https://example.test/coach.png",
        colors={
            "player": {
                "primary": "DA291C",
            },
        },
        starting_xi=[
            {
                "id": 2931,
                "name": "A. Onana",
                "number": 24,
                "pos": "G",
                "grid": "1:1",
            },
        ],
        substitutes=[],
        raw_payload={},
    )


def test_read_fixture_lineups_returns_lineups() -> None:
    lineup = make_lineup()

    with (
        patch(
            "app.api.routes.fixture_lineups.fixture_exists",
            new_callable=AsyncMock,
            return_value=True,
        ),
        patch(
            "app.api.routes.fixture_lineups.list_fixture_lineups",
            new_callable=AsyncMock,
            return_value=[lineup],
        ) as list_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/lineups",
            )

    assert response.status_code == 200
    assert response.json() == [
        {
            "team": {
                "api_id": 33,
                "name": "Manchester United",
                "logo_url": ("https://example.test/manchester-united.png"),
            },
            "formation": "4-2-3-1",
            "coach_api_id": 19,
            "coach_name": "E. ten Hag",
            "coach_photo_url": "https://example.test/coach.png",
            "colors": {
                "player": {
                    "primary": "DA291C",
                },
            },
            "starting_xi": [
                {
                    "id": 2931,
                    "name": "A. Onana",
                    "number": 24,
                    "position": "G",
                    "grid": "1:1",
                },
            ],
            "substitutes": [],
        },
    ]

    list_query.assert_awaited_once()
    assert list_query.await_args.args[1] == 1208021


def test_read_fixture_lineups_returns_empty_list() -> None:
    with (
        patch(
            "app.api.routes.fixture_lineups.fixture_exists",
            new_callable=AsyncMock,
            return_value=True,
        ),
        patch(
            "app.api.routes.fixture_lineups.list_fixture_lineups",
            new_callable=AsyncMock,
            return_value=[],
        ),
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/lineups",
            )

    assert response.status_code == 200
    assert response.json() == []


def test_read_fixture_lineups_returns_404() -> None:
    with (
        patch(
            "app.api.routes.fixture_lineups.fixture_exists",
            new_callable=AsyncMock,
            return_value=False,
        ),
        patch(
            "app.api.routes.fixture_lineups.list_fixture_lineups",
            new_callable=AsyncMock,
        ) as list_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/999999/lineups",
            )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Fixture not found",
    }
    list_query.assert_not_awaited()
