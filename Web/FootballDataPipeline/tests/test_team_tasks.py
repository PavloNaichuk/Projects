from unittest.mock import AsyncMock, patch

from app.tasks.team_tasks import sync_teams_task


def test_sync_teams_task_runs_async_service() -> None:
    expected_result = {
        "teams_synced": 20,
    }

    with patch(
        "app.tasks.team_tasks.run_team_sync",
        new_callable=AsyncMock,
    ) as run_sync:
        run_sync.return_value = expected_result

        result = sync_teams_task.run(39, 2026)

    run_sync.assert_awaited_once_with(39, 2026)
    assert result == expected_result
    assert sync_teams_task.name == "football.sync_teams"
