from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import selectinload

from app.models import Fixture, FixtureLineup


async def list_fixture_lineups(
    session: AsyncSession,
    fixture_api_id: int,
) -> list[FixtureLineup]:
    statement = (
        select(FixtureLineup)
        .join(
            Fixture,
            FixtureLineup.fixture_id == Fixture.id,
        )
        .options(
            selectinload(FixtureLineup.team),
        )
        .where(
            Fixture.api_id == fixture_api_id,
        )
        .order_by(
            FixtureLineup.team_id,
        )
    )

    result = await session.scalars(statement)

    return list(result.all())
