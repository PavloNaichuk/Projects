from types import SimpleNamespace
from unittest.mock import AsyncMock, MagicMock, call, patch

import pytest

from app.tasks.pipeline_tasks import sync_pipeline_task


def async_context(value: object) -> MagicMock:
    context = MagicMock()
    context.__aenter__ = AsyncMock(return_value=value)
    context.__aexit__ = AsyncMock(return_value=False)
    return context


@pytest.mark.asyncio
async def test_run_pipeline_sync_runs_each_stage_and_cleans_up() -> None:
    from app.tasks.pipeline_tasks import run_pipeline_sync

    engine = MagicMock()
    engine.dispose = AsyncMock()
    redis_client = MagicMock()
    redis_client.aclose = AsyncMock()
    client = MagicMock()
    sessions = [MagicMock(name=f"session-{index}") for index in range(4)]
    session_factory = MagicMock(
        side_effect=[async_context(session) for session in sessions],
    )

    with (
        patch(
            "app.tasks.pipeline_tasks.create_db_engine",
            return_value=engine,
        ),
        patch(
            "app.tasks.pipeline_tasks.create_session_factory",
            return_value=session_factory,
        ),
        patch(
            "app.tasks.pipeline_tasks.create_redis_client",
            return_value=redis_client,
        ),
        patch(
            "app.tasks.pipeline_tasks.create_api_football_client",
            return_value=async_context(client),
        ),
        patch(
            "app.tasks.pipeline_tasks.sync_leagues",
            new_callable=AsyncMock,
            return_value=SimpleNamespace(
                leagues_synced=1,
                seasons_synced=17,
            ),
        ) as sync_leagues,
        patch(
            "app.tasks.pipeline_tasks.sync_teams",
            new_callable=AsyncMock,
            return_value=SimpleNamespace(teams_synced=20),
        ) as sync_teams,
        patch(
            "app.tasks.pipeline_tasks.sync_fixtures",
            new_callable=AsyncMock,
            return_value=SimpleNamespace(fixtures_synced=380),
        ) as sync_fixtures,
        patch(
            "app.tasks.pipeline_tasks.sync_standings",
            new_callable=AsyncMock,
            return_value=SimpleNamespace(standings_synced=20),
        ) as sync_standings,
        patch(
            "app.tasks.pipeline_tasks.delete_all_cached_fixture_details",
            new_callable=AsyncMock,
        ) as clear_cache,
    ):
        result = await run_pipeline_sync(39, 2024)

    assert result == {
        "leagues_synced": 1,
        "seasons_synced": 17,
        "teams_synced": 20,
        "fixtures_synced": 380,
        "standings_synced": 20,
    }
    assert session_factory.call_args_list == [call(), call(), call(), call()]
    sync_leagues.assert_awaited_once_with(sessions[0], client, league_id=39)
    sync_teams.assert_awaited_once_with(
        sessions[1],
        client,
        league_id=39,
        season=2024,
    )
    sync_fixtures.assert_awaited_once_with(
        sessions[2],
        client,
        league_id=39,
        season=2024,
    )
    sync_standings.assert_awaited_once_with(
        sessions[3],
        client,
        league_id=39,
        season=2024,
    )
    clear_cache.assert_awaited_once_with(redis_client)
    redis_client.aclose.assert_awaited_once()
    engine.dispose.assert_awaited_once()


def test_sync_pipeline_task_runs_async_pipeline() -> None:
    expected_result = {
        "leagues_synced": 1,
        "seasons_synced": 17,
        "teams_synced": 20,
        "fixtures_synced": 380,
        "standings_synced": 20,
    }

    with patch(
        "app.tasks.pipeline_tasks.run_pipeline_sync",
        new_callable=AsyncMock,
        return_value=expected_result,
    ) as run_sync:
        result = sync_pipeline_task.run(39, 2024)

    run_sync.assert_awaited_once_with(39, 2024)
    assert result == expected_result
    assert sync_pipeline_task.name == "football.sync_pipeline"
