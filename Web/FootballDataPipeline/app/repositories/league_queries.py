from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import selectinload

from app.models import League


async def list_leagues(
    session: AsyncSession,
    *,
    offset: int,
    limit: int,
) -> list[League]:
    statement = select(League).order_by(League.name).offset(offset).limit(limit)

    result = await session.scalars(statement)

    return list(result.all())


async def get_league_by_api_id(
    session: AsyncSession,
    api_id: int,
) -> League | None:
    statement = (
        select(League)
        .options(selectinload(League.seasons))
        .where(League.api_id == api_id)
    )

    return await session.scalar(statement)
