from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import joinedload

from app.models import League, Season, Standing, Team


def standing_load_options() -> tuple[object, ...]:
    return (
        joinedload(Standing.season).joinedload(
            Season.league,
        ),
        joinedload(Standing.team),
    )


async def list_standings(
    session: AsyncSession,
    *,
    offset: int = 0,
    limit: int = 100,
    league_id: int | None = None,
    season: int | None = None,
    team_id: int | None = None,
) -> list[Standing]:
    statement = (
        select(Standing)
        .join(
            Season,
            Standing.season_id == Season.id,
        )
        .join(
            League,
            Season.league_id == League.id,
        )
        .options(*standing_load_options())
        .order_by(
            Season.year.desc(),
            Standing.position,
            Standing.team_id,
        )
        .offset(offset)
        .limit(limit)
    )

    if league_id is not None:
        statement = statement.where(
            League.api_id == league_id,
        )

    if season is not None:
        statement = statement.where(
            Season.year == season,
        )

    if team_id is not None:
        statement = statement.where(
            Standing.team.has(
                Team.api_id == team_id,
            ),
        )

    result = await session.scalars(statement)

    return list(result.all())


async def get_league_standings(
    session: AsyncSession,
    *,
    league_id: int,
    season: int,
) -> list[Standing]:
    statement = (
        select(Standing)
        .join(
            Season,
            Standing.season_id == Season.id,
        )
        .join(
            League,
            Season.league_id == League.id,
        )
        .options(*standing_load_options())
        .where(
            League.api_id == league_id,
            Season.year == season,
        )
        .order_by(
            Standing.position,
            Standing.team_id,
        )
    )

    result = await session.scalars(statement)

    return list(result.all())
