from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.standings import (
    get_standing_season_id,
    get_standing_team_ids,
    upsert_standings,
)
from app.schemas.api_football import (
    APIFootballStandingEntry,
    APIFootballStandingsResponseEntry,
)


class StandingDependencyError(RuntimeError):
    """Raised when the required season or teams are missing."""


@dataclass(frozen=True, slots=True)
class StandingSyncResult:
    standings_synced: int


def build_standing_row(
    entry: APIFootballStandingEntry,
    *,
    season_id: int,
    team_ids: dict[int, int],
) -> dict[str, object]:
    return {
        "season_id": season_id,
        "team_id": team_ids[entry.team.id],
        "position": entry.rank,
        "points": entry.points,
        "goals_diff": entry.goals_diff,
        "group_name": entry.group,
        "form": entry.form,
        "status": entry.status,
        "description": entry.description,
        "played": entry.all.played,
        "wins": entry.all.win,
        "draws": entry.all.draw,
        "losses": entry.all.lose,
        "goals_for": entry.all.goals.goals_for,
        "goals_against": entry.all.goals.against,
        "home_stats": entry.home.model_dump(
            mode="json",
            by_alias=True,
        ),
        "away_stats": entry.away.model_dump(
            mode="json",
            by_alias=True,
        ),
        "raw_payload": entry.model_dump(
            mode="json",
            by_alias=True,
        ),
        "source_updated_at": entry.update,
    }


async def sync_standings(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    league_id: int,
    season: int,
) -> StandingSyncResult:
    if league_id < 1:
        raise ValueError("League ID must be greater than zero")

    if season < 1:
        raise ValueError("Season must be greater than zero")

    payload = await client.get(
        "/standings",
        params={
            "league": league_id,
            "season": season,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a standings list",
        )

    response_entries = [
        APIFootballStandingsResponseEntry.model_validate(
            raw_entry,
        )
        for raw_entry in raw_entries
    ]

    standings: list[APIFootballStandingEntry] = []

    for response_entry in response_entries:
        league = response_entry.league

        if league.id != league_id or league.season != season:
            raise APIFootballResponseError(
                "API-Football returned standings for an unexpected league or season",
            )

        for group in league.standings:
            standings.extend(group)

    team_api_ids = {standing.team.id for standing in standings}

    async with session.begin():
        season_id = await get_standing_season_id(
            session,
            league_api_id=league_id,
            season_year=season,
        )

        if season_id is None:
            raise StandingDependencyError(
                f"Season {season} for league API ID "
                f"{league_id} is missing. Synchronize leagues first.",
            )

        team_ids = await get_standing_team_ids(
            session,
            team_api_ids,
        )

        missing_team_ids = sorted(
            team_api_ids.difference(team_ids),
        )

        if missing_team_ids:
            missing_ids = ", ".join(str(team_id) for team_id in missing_team_ids)
            raise StandingDependencyError(
                "Teams with API IDs "
                f"{missing_ids} are missing. Synchronize teams first.",
            )

        standing_rows = [
            build_standing_row(
                standing,
                season_id=season_id,
                team_ids=team_ids,
            )
            for standing in standings
        ]

        standings_synced = await upsert_standings(
            session,
            standing_rows,
        )

    return StandingSyncResult(
        standings_synced=standings_synced,
    )
