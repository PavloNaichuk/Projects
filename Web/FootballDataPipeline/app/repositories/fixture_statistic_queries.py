from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import joinedload

from app.models import Fixture, FixtureStatistic, Team


async def list_fixture_statistics(
    session: AsyncSession,
    fixture_api_id: int,
) -> list[FixtureStatistic]:
    statement = (
        select(FixtureStatistic)
        .join(
            Fixture,
            FixtureStatistic.fixture_id == Fixture.id,
        )
        .join(
            Team,
            FixtureStatistic.team_id == Team.id,
        )
        .options(
            joinedload(FixtureStatistic.team),
        )
        .where(
            Fixture.api_id == fixture_api_id,
        )
        .order_by(
            Team.name,
        )
    )

    result = await session.scalars(statement)

    return list(result.all())
