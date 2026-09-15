from sqlalchemy import delete, insert, select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import aliased

from app.models import Fixture, FixtureEvent, Team


async def get_fixture_event_context(
    session: AsyncSession,
    fixture_api_id: int,
) -> tuple[int, dict[int, int]] | None:
    home_team = aliased(Team)
    away_team = aliased(Team)

    statement = (
        select(
            Fixture.id.label("fixture_id"),
            home_team.api_id.label("home_team_api_id"),
            Fixture.home_team_id,
            away_team.api_id.label("away_team_api_id"),
            Fixture.away_team_id,
        )
        .join(
            home_team,
            Fixture.home_team_id == home_team.id,
        )
        .join(
            away_team,
            Fixture.away_team_id == away_team.id,
        )
        .where(Fixture.api_id == fixture_api_id)
    )

    row = (await session.execute(statement)).one_or_none()

    if row is None:
        return None

    team_ids = {
        row.home_team_api_id: row.home_team_id,
        row.away_team_api_id: row.away_team_id,
    }

    return row.fixture_id, team_ids


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
