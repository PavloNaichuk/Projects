from sqlalchemy import func
from sqlalchemy.dialects.postgresql import insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import Team
from app.schemas.api_football import APIFootballTeam


async def upsert_teams(
    session: AsyncSession,
    teams: list[APIFootballTeam],
) -> int:
    unique_teams = {team.id: team for team in teams}

    if not unique_teams:
        return 0

    team_rows = [
        {
            "api_id": team.id,
            "name": team.name,
            "code": team.code,
            "country_name": team.country,
            "founded": team.founded,
            "is_national": team.national,
            "logo_url": team.logo,
        }
        for team in unique_teams.values()
    ]

    statement = insert(Team).values(team_rows)

    statement = statement.on_conflict_do_update(
        index_elements=[Team.api_id],
        set_={
            "name": statement.excluded.name,
            "code": statement.excluded.code,
            "country_name": statement.excluded.country_name,
            "founded": statement.excluded.founded,
            "is_national": statement.excluded.is_national,
            "logo_url": statement.excluded.logo_url,
            "updated_at": func.now(),
        },
    )

    await session.execute(statement)

    return len(team_rows)
