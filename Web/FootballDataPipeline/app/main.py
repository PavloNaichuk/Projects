from collections.abc import AsyncIterator
from contextlib import asynccontextmanager

from fastapi import FastAPI

from app.api.routes.fixture_details import router as fixture_details_router
from app.api.routes.fixture_events import router as fixture_events_router
from app.api.routes.fixture_lineups import router as fixture_lineups_router
from app.api.routes.fixture_statistics import (
    router as fixture_statistics_router,
)
from app.api.routes.fixtures import router as fixtures_router
from app.api.routes.health import router as health_router
from app.api.routes.leagues import router as leagues_router
from app.api.routes.standings import router as standings_router
from app.api.routes.tasks import router as tasks_router
from app.api.routes.teams import router as teams_router
from app.cache.client import create_redis_client
from app.core.config import get_settings
from app.core.logging import configure_logging
from app.db.session import create_db_engine, create_session_factory
from app.middleware.request_logging import RequestLoggingMiddleware

settings = get_settings()
configure_logging(settings.log_level)


@asynccontextmanager
async def lifespan(app: FastAPI) -> AsyncIterator[None]:
    engine = create_db_engine()
    app.state.session_factory = create_session_factory(engine)

    redis_client = create_redis_client()
    app.state.redis_client = redis_client

    try:
        yield
    finally:
        await redis_client.aclose()
        await engine.dispose()


app = FastAPI(
    title="Football Data Pipeline",
    description=(
        "Asynchronous football data ingestion and read API backed by "
        "PostgreSQL, RabbitMQ, Celery, and Redis."
    ),
    version="1.0.0",
    lifespan=lifespan,
)

app.add_middleware(RequestLoggingMiddleware)

app.include_router(health_router)
app.include_router(leagues_router)
app.include_router(teams_router)
app.include_router(fixtures_router)
app.include_router(fixture_details_router)
app.include_router(fixture_events_router)
app.include_router(fixture_lineups_router)
app.include_router(fixture_statistics_router)
app.include_router(standings_router)
app.include_router(tasks_router)
