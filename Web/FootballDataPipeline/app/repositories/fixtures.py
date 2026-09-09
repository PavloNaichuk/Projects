from sqlalchemy import func, select
from sqlalchemy.dialects.postgresql import insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import Fixture, League, Season, Team


async def get_season_id(
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


async def get_team_ids(
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


async def upsert_fixtures(
    session: AsyncSession,
    fixture_rows: list[dict[str, object]],
) -> int:
    if not fixture_rows:
        return 0

    deduplicated_rows = {int(row["api_id"]): row for row in fixture_rows}

    statement = insert(Fixture).values(
        list(deduplicated_rows.values()),
    )
    excluded = statement.excluded

    statement = statement.on_conflict_do_update(
        index_elements=[Fixture.api_id],
        set_={
            "season_id": excluded.season_id,
            "home_team_id": excluded.home_team_id,
            "away_team_id": excluded.away_team_id,
            "referee": excluded.referee,
            "kickoff_at": excluded.kickoff_at,
            "timezone": excluded.timezone,
            "round_name": excluded.round_name,
            "venue_api_id": excluded.venue_api_id,
            "venue_name": excluded.venue_name,
            "venue_city": excluded.venue_city,
            "status_long": excluded.status_long,
            "status_short": excluded.status_short,
            "elapsed": excluded.elapsed,
            "extra_time": excluded.extra_time,
            "home_goals": excluded.home_goals,
            "away_goals": excluded.away_goals,
            "halftime_home": excluded.halftime_home,
            "halftime_away": excluded.halftime_away,
            "fulltime_home": excluded.fulltime_home,
            "fulltime_away": excluded.fulltime_away,
            "extra_home": excluded.extra_home,
            "extra_away": excluded.extra_away,
            "penalty_home": excluded.penalty_home,
            "penalty_away": excluded.penalty_away,
            "raw_payload": excluded.raw_payload,
            "updated_at": func.now(),
        },
    )

    await session.execute(statement)

    return len(deduplicated_rows)
