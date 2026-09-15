import asyncio

from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.fixture_event_sync import sync_fixture_events
from app.tasks.celery_app import celery_app


async def run_fixture_event_sync(
    fixture_api_id: int,
) -> dict[str, int]:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                result = await sync_fixture_events(
                    session,
                    client,
                    fixture_api_id=fixture_api_id,
                )
    finally:
        await engine.dispose()

    return {
        "events_synced": result.events_synced,
    }


@celery_app.task(name="football.sync_fixture_events")
def sync_fixture_events_task(
    fixture_api_id: int,
) -> dict[str, int]:
    return asyncio.run(
        run_fixture_event_sync(
            fixture_api_id,
        ),
    )
