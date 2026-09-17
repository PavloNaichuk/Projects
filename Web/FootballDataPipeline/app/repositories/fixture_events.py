from sqlalchemy import delete, insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import FixtureEvent


async def replace_fixture_events(
    session: AsyncSession,
    *,
    fixture_id: int,
    event_rows: list[dict[str, object]],
) -> int:
    await session.execute(
        delete(FixtureEvent).where(
            FixtureEvent.fixture_id == fixture_id,
        ),
    )

    if not event_rows:
        return 0

    deduplicated_rows = {int(row["event_order"]): row for row in event_rows}

    statement = insert(FixtureEvent).values(
        list(deduplicated_rows.values()),
    )

    await session.execute(statement)

    return len(deduplicated_rows)
