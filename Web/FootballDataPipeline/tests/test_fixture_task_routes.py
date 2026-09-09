from unittest.mock import MagicMock, patch

import pytest
from fastapi.testclient import TestClient

from app.main import app

client = TestClient(app)


def test_start_fixture_sync_task() -> None:
    task_result = MagicMock()
    task_result.id = "fixture-task-id"
    task_result.state = "PENDING"

    with patch(
        "app.api.routes.tasks.sync_fixtures_task.delay",
        return_value=task_result,
    ) as delay_mock:
        response = client.post(
            "/tasks/fixtures/sync",
            json={
                "league_id": 39,
                "season": 2024,
            },
        )

    assert response.status_code == 202
    assert response.json() == {
        "task_id": "fixture-task-id",
        "status": "PENDING",
    }
    delay_mock.assert_called_once_with(39, 2024)


@pytest.mark.parametrize(
    "payload",
    [
        {
            "league_id": 0,
            "season": 2024,
        },
        {
            "league_id": 39,
            "season": 1799,
        },
        {
            "league_id": 39,
            "season": 10000,
        },
    ],
)
def test_start_fixture_sync_rejects_invalid_parameters(
    payload: dict[str, int],
) -> None:
    with patch(
        "app.api.routes.tasks.sync_fixtures_task.delay",
    ) as delay_mock:
        response = client.post(
            "/tasks/fixtures/sync",
            json=payload,
        )

    assert response.status_code == 422
    delay_mock.assert_not_called()
