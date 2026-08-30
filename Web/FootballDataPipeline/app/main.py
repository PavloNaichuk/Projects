from collections.abc import AsyncIterator
from contextlib import asynccontextmanager

from fastapi import FastAPI

from app.api.routes.health import router as health_router
from app.db.session import create_db_engine, create_session_factory


@asynccontextmanager
async def lifespan(app: FastAPI) -> AsyncIterator[None]:
    engine = create_db_engine()
    app.state.session_factory = create_session_factory(engine)

    try:
        yield
    finally:
        await engine.dispose()


app = FastAPI(
    title="Football Data Pipeline",
    version="0.1.0",
    lifespan=lifespan,
)

app.include_router(health_router)
