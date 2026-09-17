from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import aliased

from app.models import Fixture, Team


async def get_fixture_context(
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
        .where(
            Fixture.api_id == fixture_api_id,
        )
    )

    row = (await session.execute(statement)).one_or_none()

    if row is None:
        return None

    team_ids = {
        row.home_team_api_id: row.home_team_id,
        row.away_team_api_id: row.away_team_id,
    }

    return row.fixture_id, team_ids
