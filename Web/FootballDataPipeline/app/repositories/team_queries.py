from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import Team


async def list_teams(
    session: AsyncSession,
    *,
    offset: int,
    limit: int,
    country: str | None = None,
    search: str | None = None,
) -> list[Team]:
    statement = select(Team)

    if country is not None:
        statement = statement.where(
            Team.country_name.ilike(country),
        )

    if search is not None:
        statement = statement.where(
            Team.name.ilike(f"%{search}%"),
        )

    statement = statement.order_by(Team.name).offset(offset).limit(limit)

    result = await session.scalars(statement)

    return list(result.all())


async def get_team_by_api_id(
    session: AsyncSession,
    api_id: int,
) -> Team | None:
    statement = select(Team).where(
        Team.api_id == api_id,
    )

    return await session.scalar(statement)
