import asyncio

from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.fixture_sync import sync_fixtures
from app.tasks.celery_app import celery_app


async def run_fixture_sync(
    league_id: int,
    season: int,
    *,
    live: bool = False,
) -> dict[str, int]:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                result = await sync_fixtures(
                    session,
                    client,
                    league_id=league_id,
                    season=season,
                    live=live,
                )
    finally:
        await engine.dispose()

    return {
        "fixtures_synced": result.fixtures_synced,
    }


@celery_app.task(name="football.sync_fixtures")
def sync_fixtures_task(
    league_id: int,
    season: int,
) -> dict[str, int]:
    return asyncio.run(
        run_fixture_sync(
            league_id,
            season,
        ),
    )


@celery_app.task(name="football.sync_live_fixtures")
def sync_live_fixtures_task(
    league_id: int,
    season: int,
) -> dict[str, int]:
    return asyncio.run(
        run_fixture_sync(
            league_id,
            season,
            live=True,
        ),
    )
