from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.leagues import upsert_league_entry
from app.schemas.api_football import APIFootballLeagueEntry


@dataclass(frozen=True, slots=True)
class LeagueSyncResult:
    leagues_synced: int
    seasons_synced: int


async def sync_leagues(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    league_id: int | None = None,
    current_only: bool = False,
) -> LeagueSyncResult:
    if league_id is not None and league_id < 1:
        raise ValueError("League ID must be greater than zero")

    params: dict[str, str | int] = {}

    if league_id is not None:
        params["id"] = league_id

    if current_only:
        params["current"] = "true"

    payload = await client.get(
        "/leagues",
        params=params or None,
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a leagues list",
        )

    entries = [
        APIFootballLeagueEntry.model_validate(raw_entry) for raw_entry in raw_entries
    ]

    seasons_synced = 0

    async with session.begin():
        for entry in entries:
            seasons_synced += await upsert_league_entry(
                session,
                entry,
            )

    return LeagueSyncResult(
        leagues_synced=len(entries),
        seasons_synced=seasons_synced,
    )
