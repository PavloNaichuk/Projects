from datetime import date
from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import League, Season


def make_league() -> League:
    return League(
        api_id=39,
        name="Premier League",
        league_type="League",
        country_name="England",
        country_code="GB-ENG",
        logo_url="https://example.test/premier-league.png",
        country_flag_url="https://example.test/england.svg",
    )


def test_read_leagues_returns_paginated_list() -> None:
    league = make_league()

    with patch(
        "app.api.routes.leagues.list_leagues",
        new_callable=AsyncMock,
        return_value=[league],
    ) as list_query:
        with TestClient(app) as client:
            response = client.get(
                "/leagues",
                params={
                    "offset": 0,
                    "limit": 10,
                },
            )

    assert response.status_code == 200
    assert response.json() == [
        {
            "api_id": 39,
            "name": "Premier League",
            "league_type": "League",
            "country_name": "England",
            "country_code": "GB-ENG",
            "logo_url": ("https://example.test/premier-league.png"),
            "country_flag_url": ("https://example.test/england.svg"),
        },
    ]

    list_query.assert_awaited_once()
    assert list_query.await_args.kwargs == {
        "offset": 0,
        "limit": 10,
    }


def test_read_league_returns_seasons() -> None:
    league = make_league()
    league.seasons = [
        Season(
            year=2024,
            start_date=date(2024, 8, 16),
            end_date=date(2025, 5, 25),
            is_current=False,
            coverage={
                "standings": True,
            },
        ),
    ]

    with patch(
        "app.api.routes.leagues.get_league_by_api_id",
        new_callable=AsyncMock,
        return_value=league,
    ) as get_query:
        with TestClient(app) as client:
            response = client.get("/leagues/39")

    assert response.status_code == 200
    assert response.json() == {
        "api_id": 39,
        "name": "Premier League",
        "league_type": "League",
        "country_name": "England",
        "country_code": "GB-ENG",
        "logo_url": "https://example.test/premier-league.png",
        "country_flag_url": "https://example.test/england.svg",
        "seasons": [
            {
                "year": 2024,
                "start_date": "2024-08-16",
                "end_date": "2025-05-25",
                "is_current": False,
                "coverage": {
                    "standings": True,
                },
            },
        ],
    }

    get_query.assert_awaited_once()
    assert get_query.await_args.args[1] == 39


def test_read_league_returns_404() -> None:
    with patch(
        "app.api.routes.leagues.get_league_by_api_id",
        new_callable=AsyncMock,
        return_value=None,
    ):
        with TestClient(app) as client:
            response = client.get("/leagues/999999")

    assert response.status_code == 404
    assert response.json() == {
        "detail": "League not found",
    }
