from unittest.mock import AsyncMock, patch

from app.tasks.fixture_lineup_tasks import (
    sync_fixture_lineups_task,
)


def test_sync_fixture_lineups_task_runs_async_service() -> None:
    expected_result = {
        "lineups_synced": 2,
    }

    with patch(
        "app.tasks.fixture_lineup_tasks.run_fixture_lineup_sync",
        new_callable=AsyncMock,
    ) as run_sync:
        run_sync.return_value = expected_result

        result = sync_fixture_lineups_task.run(1208021)

    run_sync.assert_awaited_once_with(1208021)
    assert result == expected_result
    assert sync_fixture_lineups_task.name == "football.sync_fixture_lineups"
