import asyncio
import logging

from app.cache.client import create_redis_client
from app.cache.fixture_details import delete_all_cached_fixture_details
from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.fixture_sync import sync_fixtures
from app.services.league_sync import sync_leagues
from app.services.standing_sync import sync_standings
from app.services.team_sync import sync_teams
from app.tasks.celery_app import celery_app

logger = logging.getLogger("football_data_pipeline.pipeline")


async def run_pipeline_sync(
    league_id: int,
    season: int,
) -> dict[str, int]:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)
    redis_client = create_redis_client()

    logger.info(
        "pipeline_started",
        extra={
            "task_name": "football.sync_pipeline",
            "league_id": league_id,
            "season": season,
        },
    )

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                league_result = await sync_leagues(
                    session,
                    client,
                    league_id=league_id,
                )

            async with session_factory() as session:
                team_result = await sync_teams(
                    session,
                    client,
                    league_id=league_id,
                    season=season,
                )

            async with session_factory() as session:
                fixture_result = await sync_fixtures(
                    session,
                    client,
                    league_id=league_id,
                    season=season,
                )

            async with session_factory() as session:
                standing_result = await sync_standings(
                    session,
                    client,
                    league_id=league_id,
                    season=season,
                )

        await delete_all_cached_fixture_details(redis_client)
    finally:
        await redis_client.aclose()
        await engine.dispose()

    result = {
        "leagues_synced": league_result.leagues_synced,
        "seasons_synced": league_result.seasons_synced,
        "teams_synced": team_result.teams_synced,
        "fixtures_synced": fixture_result.fixtures_synced,
        "standings_synced": standing_result.standings_synced,
    }

    logger.info(
        "pipeline_completed",
        extra={
            "task_name": "football.sync_pipeline",
            "league_id": league_id,
            "season": season,
        },
    )

    return result


@celery_app.task(name="football.sync_pipeline")
def sync_pipeline_task(
    league_id: int,
    season: int,
) -> dict[str, int]:
    return asyncio.run(
        run_pipeline_sync(
            league_id,
            season,
        ),
    )
