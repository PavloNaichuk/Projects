from sqlalchemy import func
from sqlalchemy.dialects.postgresql import insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.models import League, Season
from app.schemas.api_football import APIFootballLeagueEntry


async def upsert_league_entry(
    session: AsyncSession,
    entry: APIFootballLeagueEntry,
) -> int:
    league_statement = insert(League).values(
        api_id=entry.league.id,
        name=entry.league.name,
        league_type=entry.league.type,
        country_name=entry.country.name,
        country_code=entry.country.code,
        logo_url=entry.league.logo,
        country_flag_url=entry.country.flag,
    )

    league_statement = league_statement.on_conflict_do_update(
        index_elements=[League.api_id],
        set_={
            "name": league_statement.excluded.name,
            "league_type": league_statement.excluded.league_type,
            "country_name": league_statement.excluded.country_name,
            "country_code": league_statement.excluded.country_code,
            "logo_url": league_statement.excluded.logo_url,
            "country_flag_url": (league_statement.excluded.country_flag_url),
            "updated_at": func.now(),
        },
    ).returning(League.id)

    result = await session.execute(league_statement)
    league_id = result.scalar_one()

    season_rows = [
        {
            "league_id": league_id,
            "year": season.year,
            "start_date": season.start,
            "end_date": season.end,
            "is_current": season.current,
            "coverage": season.coverage,
        }
        for season in entry.seasons
    ]

    if not season_rows:
        return 0

    season_statement = insert(Season).values(season_rows)

    season_statement = season_statement.on_conflict_do_update(
        index_elements=[Season.league_id, Season.year],
        set_={
            "start_date": season_statement.excluded.start_date,
            "end_date": season_statement.excluded.end_date,
            "is_current": season_statement.excluded.is_current,
            "coverage": season_statement.excluded.coverage,
            "updated_at": func.now(),
        },
    )

    await session.execute(season_statement)

    return len(season_rows)
