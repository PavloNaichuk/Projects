from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.fixtures import (
    get_season_id,
    get_team_ids,
    upsert_fixtures,
)
from app.schemas.api_football import APIFootballFixtureEntry


class FixtureDependencyError(RuntimeError):
    """Raised when required season or teams are missing."""


@dataclass(frozen=True, slots=True)
class FixtureSyncResult:
    fixtures_synced: int


def build_fixture_row(
    entry: APIFootballFixtureEntry,
    *,
    raw_payload: dict[str, object],
    season_id: int,
    team_ids: dict[int, int],
) -> dict[str, object]:
    fixture = entry.fixture
    home_team = entry.teams.home
    away_team = entry.teams.away

    return {
        "api_id": fixture.id,
        "season_id": season_id,
        "home_team_id": team_ids[home_team.id],
        "away_team_id": team_ids[away_team.id],
        "referee": fixture.referee,
        "kickoff_at": fixture.date,
        "timezone": fixture.timezone,
        "round_name": entry.league.round,
        "venue_api_id": fixture.venue.id,
        "venue_name": fixture.venue.name,
        "venue_city": fixture.venue.city,
        "status_long": fixture.status.long,
        "status_short": fixture.status.short,
        "elapsed": fixture.status.elapsed,
        "extra_time": fixture.status.extra,
        "home_goals": entry.goals.home,
        "away_goals": entry.goals.away,
        "halftime_home": entry.score.halftime.home,
        "halftime_away": entry.score.halftime.away,
        "fulltime_home": entry.score.fulltime.home,
        "fulltime_away": entry.score.fulltime.away,
        "extra_home": entry.score.extratime.home,
        "extra_away": entry.score.extratime.away,
        "penalty_home": entry.score.penalty.home,
        "penalty_away": entry.score.penalty.away,
        "raw_payload": raw_payload,
    }


async def sync_fixtures(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    league_id: int,
    season: int,
) -> FixtureSyncResult:
    if league_id < 1:
        raise ValueError("League ID must be greater than zero")

    if season < 1:
        raise ValueError("Season must be greater than zero")

    payload = await client.get(
        "/fixtures",
        params={
            "league": league_id,
            "season": season,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a fixtures list",
        )

    entries_with_payload: list[
        tuple[
            APIFootballFixtureEntry,
            dict[str, object],
        ]
    ] = []

    for raw_entry in raw_entries:
        if not isinstance(raw_entry, dict):
            raise APIFootballResponseError(
                "API-Football returned an invalid fixture entry",
            )

        entries_with_payload.append(
            (
                APIFootballFixtureEntry.model_validate(raw_entry),
                raw_entry,
            ),
        )

    team_api_ids = {
        team_id
        for entry, _ in entries_with_payload
        for team_id in (
            entry.teams.home.id,
            entry.teams.away.id,
        )
    }

    async with session.begin():
        season_id = await get_season_id(
            session,
            league_api_id=league_id,
            season_year=season,
        )

        if season_id is None:
            raise FixtureDependencyError(
                f"Season {season} for league API ID "
                f"{league_id} is missing. Synchronize leagues first.",
            )

        team_ids = await get_team_ids(
            session,
            team_api_ids,
        )

        missing_team_ids = sorted(
            team_api_ids.difference(team_ids),
        )

        if missing_team_ids:
            missing_ids = ", ".join(str(team_id) for team_id in missing_team_ids)
            raise FixtureDependencyError(
                "Teams with API IDs "
                f"{missing_ids} are missing. Synchronize teams first.",
            )

        fixture_rows = [
            build_fixture_row(
                entry,
                raw_payload=raw_payload,
                season_id=season_id,
                team_ids=team_ids,
            )
            for entry, raw_payload in entries_with_payload
        ]

        fixtures_synced = await upsert_fixtures(
            session,
            fixture_rows,
        )

    return FixtureSyncResult(
        fixtures_synced=fixtures_synced,
    )
