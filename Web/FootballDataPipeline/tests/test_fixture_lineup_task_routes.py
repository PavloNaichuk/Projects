from unittest.mock import MagicMock, patch

from fastapi.testclient import TestClient

from app.main import app


def test_submit_fixture_lineup_sync_returns_task() -> None:
    task = MagicMock()
    task.id = "fixture-lineup-task-id"
    task.state = "PENDING"

    with patch(
        "app.api.routes.tasks.sync_fixture_lineups_task.delay",
        return_value=task,
    ) as delay_task:
        with TestClient(app) as client:
            response = client.post(
                "/tasks/fixtures/1208021/lineups/sync",
            )

    assert response.status_code == 202
    assert response.json() == {
        "task_id": "fixture-lineup-task-id",
        "status": "PENDING",
    }
    delay_task.assert_called_once_with(1208021)


def test_submit_fixture_lineup_sync_rejects_invalid_id() -> None:
    with TestClient(app) as client:
        response = client.post(
            "/tasks/fixtures/0/lineups/sync",
        )

    assert response.status_code == 422
