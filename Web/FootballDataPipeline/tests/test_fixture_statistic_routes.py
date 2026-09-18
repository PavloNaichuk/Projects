from unittest.mock import AsyncMock, patch

from fastapi.testclient import TestClient

from app.main import app
from app.models import FixtureStatistic, Team


def make_fixture_statistic() -> FixtureStatistic:
    team = Team(
        api_id=33,
        name="Manchester United",
        code="MUN",
        country_name="England",
        founded=1878,
        is_national=False,
        logo_url="https://example.test/manchester-united.png",
    )

    return FixtureStatistic(
        fixture_id=501,
        team_id=101,
        team=team,
        statistics={
            "Shots on Goal": 5,
            "Total Shots": 14,
            "Ball Possession": "55%",
            "expected_goals": 2.43,
        },
        raw_payload={},
    )


def test_read_fixture_statistics_returns_team_statistics() -> None:
    statistic = make_fixture_statistic()

    with (
        patch(
            "app.api.routes.fixture_statistics.list_fixture_statistics",
            new_callable=AsyncMock,
            return_value=[statistic],
        ) as list_query,
        patch(
            "app.api.routes.fixture_statistics.fixture_exists",
            new_callable=AsyncMock,
        ) as exists_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/statistics",
            )

    assert response.status_code == 200
    assert response.json() == [
        {
            "team": {
                "api_id": 33,
                "name": "Manchester United",
                "logo_url": ("https://example.test/manchester-united.png"),
            },
            "statistics": {
                "Shots on Goal": 5,
                "Total Shots": 14,
                "Ball Possession": "55%",
                "expected_goals": 2.43,
            },
        },
    ]

    list_query.assert_awaited_once()
    assert list_query.await_args.args[1] == 1208021
    exists_query.assert_not_awaited()


def test_read_fixture_statistics_returns_empty_list() -> None:
    with (
        patch(
            "app.api.routes.fixture_statistics.list_fixture_statistics",
            new_callable=AsyncMock,
            return_value=[],
        ),
        patch(
            "app.api.routes.fixture_statistics.fixture_exists",
            new_callable=AsyncMock,
            return_value=True,
        ) as exists_query,
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/1208021/statistics",
            )

    assert response.status_code == 200
    assert response.json() == []
    exists_query.assert_awaited_once()


def test_read_fixture_statistics_returns_404() -> None:
    with (
        patch(
            "app.api.routes.fixture_statistics.list_fixture_statistics",
            new_callable=AsyncMock,
            return_value=[],
        ),
        patch(
            "app.api.routes.fixture_statistics.fixture_exists",
            new_callable=AsyncMock,
            return_value=False,
        ),
    ):
        with TestClient(app) as client:
            response = client.get(
                "/fixtures/999999/statistics",
            )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Fixture not found",
    }
