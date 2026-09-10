from datetime import date, datetime, time, timezone

from sqlalchemy import or_, select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import joinedload

from app.models import Fixture, League, Season, Team


def fixture_load_options() -> tuple[object, ...]:
    return (
        joinedload(Fixture.season).joinedload(Season.league),
        joinedload(Fixture.home_team),
        joinedload(Fixture.away_team),
    )


async def list_fixtures(
    session: AsyncSession,
    *,
    offset: int = 0,
    limit: int = 50,
    league_id: int | None = None,
    season: int | None = None,
    team_id: int | None = None,
    status_short: str | None = None,
    date_from: date | None = None,
    date_to: date | None = None,
) -> list[Fixture]:
    statement = (
        select(Fixture)
        .options(*fixture_load_options())
        .order_by(
            Fixture.kickoff_at,
            Fixture.api_id,
        )
        .offset(offset)
        .limit(limit)
    )

    if league_id is not None:
        statement = statement.where(
            Fixture.season.has(
                Season.league.has(
                    League.api_id == league_id,
                ),
            ),
        )

    if season is not None:
        statement = statement.where(
            Fixture.season.has(
                Season.year == season,
            ),
        )

    if team_id is not None:
        statement = statement.where(
            or_(
                Fixture.home_team.has(
                    Team.api_id == team_id,
                ),
                Fixture.away_team.has(
                    Team.api_id == team_id,
                ),
            ),
        )

    if status_short is not None:
        statement = statement.where(
            Fixture.status_short == status_short.upper(),
        )

    if date_from is not None:
        start_at = datetime.combine(
            date_from,
            time.min,
            tzinfo=timezone.utc,
        )
        statement = statement.where(
            Fixture.kickoff_at >= start_at,
        )

    if date_to is not None:
        end_at = datetime.combine(
            date_to,
            time.max,
            tzinfo=timezone.utc,
        )
        statement = statement.where(
            Fixture.kickoff_at <= end_at,
        )

    result = await session.scalars(statement)

    return list(result.all())


async def get_fixture_by_api_id(
    session: AsyncSession,
    api_id: int,
) -> Fixture | None:
    statement = (
        select(Fixture).options(*fixture_load_options()).where(Fixture.api_id == api_id)
    )

    return await session.scalar(statement)
