from sqlalchemy import delete, insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import FixtureStatistic


async def replace_fixture_statistics(
    session: AsyncSession,
    *,
    fixture_id: int,
    statistic_rows: list[dict[str, object]],
) -> int:
    await session.execute(
        delete(FixtureStatistic).where(
            FixtureStatistic.fixture_id == fixture_id,
        ),
    )

    if not statistic_rows:
        return 0

    deduplicated_rows = {int(row["team_id"]): row for row in statistic_rows}

    statement = insert(FixtureStatistic).values(
        list(deduplicated_rows.values()),
    )

    await session.execute(statement)

    return len(deduplicated_rows)
