from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.fixture_context import get_fixture_context
from app.repositories.fixture_lineups import (
    replace_fixture_lineups,
)
from app.schemas.api_football import (
    APIFootballFixtureLineupEntry,
)


class FixtureLineupSyncError(RuntimeError):
    pass


class FixtureNotFoundError(FixtureLineupSyncError):
    pass


class FixtureLineupTeamNotFoundError(FixtureLineupSyncError):
    pass


@dataclass(frozen=True, slots=True)
class FixtureLineupSyncResult:
    lineups_synced: int


async def sync_fixture_lineups(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    fixture_api_id: int,
) -> FixtureLineupSyncResult:
    if fixture_api_id < 1:
        raise ValueError("Fixture ID must be greater than zero")

    payload = await client.get(
        "/fixtures/lineups",
        params={
            "fixture": fixture_api_id,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a lineups list",
        )

    validated_entries: list[
        tuple[
            APIFootballFixtureLineupEntry,
            dict[str, object],
        ]
    ] = []

    for raw_entry in raw_entries:
        if not isinstance(raw_entry, dict):
            raise APIFootballResponseError(
                "API-Football lineups list contains an invalid entry",
            )

        validated_entries.append(
            (
                APIFootballFixtureLineupEntry.model_validate(
                    raw_entry,
                ),
                raw_entry,
            ),
        )

    async with session.begin():
        context = await get_fixture_context(
            session,
            fixture_api_id,
        )

        if context is None:
            raise FixtureNotFoundError(
                f"Fixture {fixture_api_id} was not found",
            )

        fixture_id, team_ids = context
        lineup_rows: list[dict[str, object]] = []

        for entry, raw_entry in validated_entries:
            team_id = team_ids.get(entry.team.id)

            if team_id is None:
                raise FixtureLineupTeamNotFoundError(
                    f"Team {entry.team.id} does not belong to fixture {fixture_api_id}",
                )

            starting_xi = [
                player_entry.player.model_dump(
                    by_alias=True,
                )
                for player_entry in entry.starting_xi
            ]
            substitutes = [
                player_entry.player.model_dump(
                    by_alias=True,
                )
                for player_entry in entry.substitutes
            ]

            lineup_rows.append(
                {
                    "fixture_id": fixture_id,
                    "team_id": team_id,
                    "formation": entry.formation,
                    "coach_api_id": entry.coach.id,
                    "coach_name": entry.coach.name,
                    "coach_photo_url": entry.coach.photo,
                    "colors": entry.team.colors,
                    "starting_xi": starting_xi,
                    "substitutes": substitutes,
                    "raw_payload": raw_entry,
                },
            )

        lineups_synced = await replace_fixture_lineups(
            session,
            fixture_id=fixture_id,
            lineup_rows=lineup_rows,
        )

    return FixtureLineupSyncResult(
        lineups_synced=lineups_synced,
    )
