from sqlalchemy.ext.asyncio import (
    AsyncEngine,
    AsyncSession,
    async_sessionmaker,
    create_async_engine,
)

from app.core.config import get_settings


def create_db_engine() -> AsyncEngine:
    return create_async_engine(
        get_settings().database_url,
        pool_pre_ping=True,
        hide_parameters=True,
        connect_args={"timeout": 5},
    )


def create_session_factory(
    engine: AsyncEngine,
) -> async_sessionmaker[AsyncSession]:
    return async_sessionmaker(
        bind=engine,
        expire_on_commit=False,
    )
