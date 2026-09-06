import asyncio

from app.clients.api_football import create_api_football_client
from app.db.session import create_db_engine, create_session_factory
from app.services.league_sync import sync_leagues


async def main() -> None:
    engine = create_db_engine()
    session_factory = create_session_factory(engine)

    try:
        async with create_api_football_client() as client:
            async with session_factory() as session:
                result = await sync_leagues(
                    session,
                    client,
                    league_id=39,
                )
    finally:
        await engine.dispose()

    print("League synchronization completed")
    print(f"Leagues synchronized: {result.leagues_synced}")
    print(f"Seasons synchronized: {result.seasons_synced}")


if __name__ == "__main__":
    asyncio.run(main())
