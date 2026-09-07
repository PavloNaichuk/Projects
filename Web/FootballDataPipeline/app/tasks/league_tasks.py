import asyncio

from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.league_sync import sync_leagues
from app.tasks.celery_app import celery_app


async def run_league_sync(
    league_id: int | None,
) -> dict[str, int]:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                result = await sync_leagues(
                    session,
                    client,
                    league_id=league_id,
                )
    finally:
        await engine.dispose()

    return {
        "leagues_synced": result.leagues_synced,
        "seasons_synced": result.seasons_synced,
    }


@celery_app.task(name="football.sync_leagues")
def sync_leagues_task(
    league_id: int | None = None,
) -> dict[str, int]:
    return asyncio.run(run_league_sync(league_id))
