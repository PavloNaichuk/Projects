from unittest.mock import AsyncMock, patch

from app.tasks.fixture_tasks import sync_fixtures_task


def test_sync_fixtures_task_runs_async_service() -> None:
    expected_result = {
        "fixtures_synced": 380,
    }

    with patch(
        "app.tasks.fixture_tasks.run_fixture_sync",
        new_callable=AsyncMock,
    ) as run_sync:
        run_sync.return_value = expected_result

        result = sync_fixtures_task.run(39, 2024)

    run_sync.assert_awaited_once_with(39, 2024)
    assert result == expected_result
    assert sync_fixtures_task.name == "football.sync_fixtures"
