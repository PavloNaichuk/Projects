from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import Team


def make_team() -> Team:
    return Team(
        api_id=33,
        name="Manchester United",
        code="MUN",
        country_name="England",
        founded=1878,
        is_national=False,
        logo_url="https://example.test/manchester-united.png",
    )


def test_read_teams_returns_filtered_list() -> None:
    team = make_team()

    with patch(
        "app.api.routes.teams.list_teams",
        new_callable=AsyncMock,
        return_value=[team],
    ) as list_query:
        with TestClient(app) as client:
            response = client.get(
                "/teams",
                params={
                    "offset": 0,
                    "limit": 10,
                    "country": "England",
                    "search": "Manchester",
                },
            )

    assert response.status_code == 200
    assert response.json() == [
        {
            "api_id": 33,
            "name": "Manchester United",
            "code": "MUN",
            "country_name": "England",
            "founded": 1878,
            "is_national": False,
            "logo_url": ("https://example.test/manchester-united.png"),
        },
    ]

    list_query.assert_awaited_once()
    assert list_query.await_args.kwargs == {
        "offset": 0,
        "limit": 10,
        "country": "England",
        "search": "Manchester",
    }


def test_read_team_returns_team() -> None:
    team = make_team()

    with patch(
        "app.api.routes.teams.get_team_by_api_id",
        new_callable=AsyncMock,
        return_value=team,
    ) as get_query:
        with TestClient(app) as client:
            response = client.get("/teams/33")

    assert response.status_code == 200
    assert response.json() == {
        "api_id": 33,
        "name": "Manchester United",
        "code": "MUN",
        "country_name": "England",
        "founded": 1878,
        "is_national": False,
        "logo_url": "https://example.test/manchester-united.png",
    }

    get_query.assert_awaited_once()
    assert get_query.await_args.args[1] == 33


def test_read_team_returns_404() -> None:
    with patch(
        "app.api.routes.teams.get_team_by_api_id",
        new_callable=AsyncMock,
        return_value=None,
    ):
        with TestClient(app) as client:
            response = client.get("/teams/999999")

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Team not found",
    }
