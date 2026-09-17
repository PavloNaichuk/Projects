from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import joinedload

from app.models import Fixture, FixtureEvent


async def fixture_exists(
    session: AsyncSession,
    fixture_api_id: int,
) -> bool:
    statement = select(Fixture.id).where(
        Fixture.api_id == fixture_api_id,
    )

    fixture_id = await session.scalar(statement)

    return fixture_id is not None


async def list_fixture_events(
    session: AsyncSession,
    fixture_api_id: int,
) -> list[FixtureEvent]:
    statement = (
        select(FixtureEvent)
        .join(
            Fixture,
            FixtureEvent.fixture_id == Fixture.id,
        )
        .options(
            joinedload(FixtureEvent.team),
        )
        .where(
            Fixture.api_id == fixture_api_id,
        )
        .order_by(
            FixtureEvent.event_order,
        )
    )

    result = await session.scalars(statement)

    return list(result.all())
