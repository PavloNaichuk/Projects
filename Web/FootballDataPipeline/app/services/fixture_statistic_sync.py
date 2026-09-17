from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.fixture_context import get_fixture_context
from app.repositories.fixture_statistics import (
    replace_fixture_statistics,
)
from app.schemas.api_football import (
    APIFootballFixtureStatisticEntry,
)


class FixtureStatisticSyncError(RuntimeError):
    pass


class FixtureNotFoundError(FixtureStatisticSyncError):
    pass


class FixtureStatisticTeamNotFoundError(
    FixtureStatisticSyncError,
):
    pass


@dataclass(frozen=True, slots=True)
class FixtureStatisticSyncResult:
    statistics_synced: int


async def sync_fixture_statistics(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    fixture_api_id: int,
) -> FixtureStatisticSyncResult:
    if fixture_api_id < 1:
        raise ValueError("Fixture ID must be greater than zero")

    payload = await client.get(
        "/fixtures/statistics",
        params={
            "fixture": fixture_api_id,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain a statistics list",
        )

    validated_entries: list[
        tuple[
            APIFootballFixtureStatisticEntry,
            dict[str, object],
        ]
    ] = []

    for raw_entry in raw_entries:
        if not isinstance(raw_entry, dict):
            raise APIFootballResponseError(
                "API-Football statistics list contains an invalid entry",
            )

        validated_entries.append(
            (
                APIFootballFixtureStatisticEntry.model_validate(
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
        statistic_rows: list[dict[str, object]] = []

        for entry, raw_entry in validated_entries:
            team_id = team_ids.get(entry.team.id)

            if team_id is None:
                raise FixtureStatisticTeamNotFoundError(
                    f"Team {entry.team.id} does not belong to fixture {fixture_api_id}",
                )

            statistics = {item.type: item.value for item in entry.statistics}

            statistic_rows.append(
                {
                    "fixture_id": fixture_id,
                    "team_id": team_id,
                    "statistics": statistics,
                    "raw_payload": raw_entry,
                },
            )

        statistics_synced = await replace_fixture_statistics(
            session,
            fixture_id=fixture_id,
            statistic_rows=statistic_rows,
        )

    return FixtureStatisticSyncResult(
        statistics_synced=statistics_synced,
    )
