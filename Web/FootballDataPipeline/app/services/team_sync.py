from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.teams import upsert_teams
from app.schemas.api_football import APIFootballTeamEntry


@dataclass(frozen=True, slots=True)
class TeamSyncResult:
    teams_synced: int


async def sync_teams(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    league_id: int,
    season: int,
) -> TeamSyncResult:
    if league_id < 1:
        raise ValueError("League ID must be greater than zero")

    if season < 1:
        raise ValueError("Season must be greater than zero")

    payload = await client.get(
        "/teams",
        params={
            "league": league_id,
            "season": season,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a teams list",
        )

    entries = [
        APIFootballTeamEntry.model_validate(raw_entry) for raw_entry in raw_entries
    ]

    teams = [entry.team for entry in entries]

    async with session.begin():
        teams_synced = await upsert_teams(
            session,
            teams,
        )

    return TeamSyncResult(
        teams_synced=teams_synced,
    )
