import asyncio

from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.standing_sync import sync_standings
from app.tasks.celery_app import celery_app


async def run_standing_sync(
    league_id: int,
    season: int,
) -> dict[str, int]:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                result = await sync_standings(
                    session,
                    client,
                    league_id=league_id,
                    season=season,
                )
    finally:
        await engine.dispose()

    return {
        "standings_synced": result.standings_synced,
    }


@celery_app.task(name="football.sync_standings")
def sync_standings_task(
    league_id: int,
    season: int,
) -> dict[str, int]:
    return asyncio.run(
        run_standing_sync(
            league_id,
            season,
        ),
    )
