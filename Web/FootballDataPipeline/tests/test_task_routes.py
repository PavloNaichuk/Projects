from unittest.mock import MagicMock, patch

from fastapi.testclient import TestClient

from app.main import app


def test_submit_league_sync_returns_task_id() -> None:
    task = MagicMock()
    task.id = "task-123"
    task.state = "PENDING"

    with patch(
        "app.api.routes.tasks.sync_leagues_task.delay",
        return_value=task,
    ) as delay:
        with TestClient(app) as client:
            response = client.post(
                "/tasks/leagues/sync",
                json={"league_id": 39},
            )

    assert response.status_code == 202
    assert response.json() == {
        "task_id": "task-123",
        "status": "PENDING",
    }
    delay.assert_called_once_with(39)


def test_submit_league_sync_rejects_invalid_id() -> None:
    with TestClient(app) as client:
        response = client.post(
            "/tasks/leagues/sync",
            json={"league_id": 0},
        )

    assert response.status_code == 422


def test_get_successful_task_status() -> None:
    task = MagicMock()
    task.state = "SUCCESS"
    task.result = {
        "leagues_synced": 1,
        "seasons_synced": 17,
    }
    task.successful.return_value = True
    task.failed.return_value = False

    with patch(
        "app.api.routes.tasks.celery_app.AsyncResult",
        return_value=task,
    ):
        with TestClient(app) as client:
            response = client.get("/tasks/task-123")

    assert response.status_code == 200
    assert response.json() == {
        "task_id": "task-123",
        "status": "SUCCESS",
        "result": {
            "leagues_synced": 1,
            "seasons_synced": 17,
        },
        "error": None,
    }


def test_get_failed_task_status() -> None:
    task = MagicMock()
    task.state = "FAILURE"
    task.result = RuntimeError("Synchronization failed")
    task.successful.return_value = False
    task.failed.return_value = True

    with patch(
        "app.api.routes.tasks.celery_app.AsyncResult",
        return_value=task,
    ):
        with TestClient(app) as client:
            response = client.get("/tasks/task-456")

    assert response.status_code == 200
    assert response.json() == {
        "task_id": "task-456",
        "status": "FAILURE",
        "result": None,
        "error": "Synchronization failed",
    }


def test_submit_team_sync_returns_task_id() -> None:
    task = MagicMock()
    task.id = "team-task-123"
    task.state = "PENDING"

    with patch(
        "app.api.routes.tasks.sync_teams_task.delay",
        return_value=task,
    ) as delay:
        with TestClient(app) as client:
            response = client.post(
                "/tasks/teams/sync",
                json={
                    "league_id": 39,
                    "season": 2024,
                },
            )

    assert response.status_code == 202
    assert response.json() == {
        "task_id": "team-task-123",
        "status": "PENDING",
    }
    delay.assert_called_once_with(39, 2024)


def test_submit_team_sync_rejects_invalid_season() -> None:
    with TestClient(app) as client:
        response = client.post(
            "/tasks/teams/sync",
            json={
                "league_id": 39,
                "season": 0,
            },
        )

    assert response.status_code == 422
