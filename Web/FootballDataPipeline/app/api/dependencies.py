from collections.abc import AsyncIterator
from typing import Annotated

from fastapi import Depends, Request
from redis.asyncio import Redis
from sqlalchemy.ext.asyncio import AsyncSession, async_sessionmaker


async def get_db_session(
    request: Request,
) -> AsyncIterator[AsyncSession]:
    session_factory: async_sessionmaker[AsyncSession] = (
        request.app.state.session_factory
    )

    async with session_factory() as session:
        yield session


def get_redis_client(request: Request) -> Redis:
    return request.app.state.redis_client


DBSession = Annotated[
    AsyncSession,
    Depends(get_db_session),
]
CacheClient = Annotated[
    Redis,
    Depends(get_redis_client),
]
