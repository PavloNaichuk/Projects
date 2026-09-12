from unittest.mock import AsyncMock, patch

from app.tasks.standing_tasks import sync_standings_task


def test_sync_standings_task_runs_async_service() -> None:
    expected_result = {
        "standings_synced": 20,
    }

    with patch(
        "app.tasks.standing_tasks.run_standing_sync",
        new_callable=AsyncMock,
    ) as run_sync:
        run_sync.return_value = expected_result

        result = sync_standings_task.run(39, 2024)

    run_sync.assert_awaited_once_with(39, 2024)
    assert result == expected_result
    assert sync_standings_task.name == "football.sync_standings"
