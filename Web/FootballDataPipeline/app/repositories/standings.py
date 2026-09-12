from sqlalchemy import func, select
from sqlalchemy.dialects.postgresql import insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import League, Season, Standing, Team


async def get_standing_season_id(
    session: AsyncSession,
    *,
    league_api_id: int,
    season_year: int,
) -> int | None:
    statement = (
        select(Season.id)
        .join(
            League,
            Season.league_id == League.id,
        )
        .where(
            League.api_id == league_api_id,
            Season.year == season_year,
        )
    )

    return await session.scalar(statement)


async def get_standing_team_ids(
    session: AsyncSession,
    api_ids: set[int],
) -> dict[int, int]:
    if not api_ids:
        return {}

    statement = select(
        Team.api_id,
        Team.id,
    ).where(Team.api_id.in_(api_ids))

    result = await session.execute(statement)

    return {api_id: team_id for api_id, team_id in result.all()}


async def upsert_standings(
    session: AsyncSession,
    standing_rows: list[dict[str, object]],
) -> int:
    if not standing_rows:
        return 0

    deduplicated_rows = {int(row["team_id"]): row for row in standing_rows}

    statement = insert(Standing).values(
        list(deduplicated_rows.values()),
    )
    excluded = statement.excluded

    statement = statement.on_conflict_do_update(
        index_elements=[
            Standing.season_id,
            Standing.team_id,
        ],
        set_={
            "position": excluded.position,
            "points": excluded.points,
            "goals_diff": excluded.goals_diff,
            "group_name": excluded.group_name,
            "form": excluded.form,
            "status": excluded.status,
            "description": excluded.description,
            "played": excluded.played,
            "wins": excluded.wins,
            "draws": excluded.draws,
            "losses": excluded.losses,
            "goals_for": excluded.goals_for,
            "goals_against": excluded.goals_against,
            "home_stats": excluded.home_stats,
            "away_stats": excluded.away_stats,
            "raw_payload": excluded.raw_payload,
            "source_updated_at": excluded.source_updated_at,
            "updated_at": func.now(),
        },
    )

    await session.execute(statement)

    return len(deduplicated_rows)
