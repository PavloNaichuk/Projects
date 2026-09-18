from unittest.mock import MagicMock, patch

from fastapi.testclient import TestClient

from app.main import app


def test_submit_fixture_statistic_sync_returns_task_id() -> None:
    task = MagicMock()
    task.id = "fixture-statistic-task-id"
    task.state = "PENDING"

    with patch(
        "app.api.routes.tasks.sync_fixture_statistics_task.delay",
        return_value=task,
    ) as delay:
        with TestClient(app) as client:
            response = client.post(
                "/tasks/fixtures/1208021/statistics/sync",
            )

    assert response.status_code == 202
    assert response.json() == {
        "task_id": "fixture-statistic-task-id",
        "status": "PENDING",
    }
    delay.assert_called_once_with(1208021)


def test_submit_fixture_statistic_sync_rejects_invalid_id() -> None:
    with patch(
        "app.api.routes.tasks.sync_fixture_statistics_task.delay",
    ) as delay:
        with TestClient(app) as client:
            response = client.post(
                "/tasks/fixtures/0/statistics/sync",
            )

    assert response.status_code == 422
    delay.assert_not_called()
