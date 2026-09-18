from sqlalchemy import delete, insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import FixtureLineup


async def replace_fixture_lineups(
    session: AsyncSession,
    *,
    fixture_id: int,
    lineup_rows: list[dict[str, object]],
) -> int:
    await session.execute(
        delete(FixtureLineup).where(
            FixtureLineup.fixture_id == fixture_id,
        ),
    )

    if not lineup_rows:
        return 0

    deduplicated_rows = {int(row["team_id"]): row for row in lineup_rows}

    statement = insert(FixtureLineup).values(
        list(deduplicated_rows.values()),
    )

    await session.execute(statement)

    return len(deduplicated_rows)
