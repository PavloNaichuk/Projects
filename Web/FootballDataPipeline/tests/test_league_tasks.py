from unittest.mock import AsyncMock, patch

from app.tasks.celery_app import celery_app
from app.tasks.league_tasks import sync_leagues_task


def test_celery_configuration() -> None:
    assert celery_app.conf.task_serializer == "json"
    assert celery_app.conf.result_serializer == "json"
    assert celery_app.conf.task_acks_late is True
    assert celery_app.conf.task_reject_on_worker_lost is True
    assert celery_app.conf.worker_prefetch_multiplier == 1
    assert celery_app.conf.broker_url.startswith("amqp://")
    assert celery_app.conf.result_backend.startswith("redis://")
    assert celery_app.conf.worker_enable_remote_control is False
    assert celery_app.conf.worker_cancel_long_running_tasks_on_connection_loss is True


def test_sync_leagues_task_runs_async_service() -> None:
    expected_result = {
        "leagues_synced": 1,
        "seasons_synced": 17,
    }

    with patch(
        "app.tasks.league_tasks.run_league_sync",
        new_callable=AsyncMock,
    ) as run_sync:
        run_sync.return_value = expected_result

        result = sync_leagues_task.run(39)

    run_sync.assert_awaited_once_with(39)
    assert result == expected_result
