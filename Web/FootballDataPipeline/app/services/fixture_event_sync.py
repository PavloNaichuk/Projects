from dataclasses import dataclass

from sqlalchemy.ext.asyncio import AsyncSession

from app.clients.api_football import (
    APIFootballClient,
    APIFootballResponseError,
)
from app.repositories.fixture_events import (
    get_fixture_event_context,
    replace_fixture_events,
)
from app.schemas.api_football import APIFootballFixtureEvent


class FixtureEventSyncError(RuntimeError):
    pass


class FixtureNotFoundError(FixtureEventSyncError):
    pass


class FixtureEventTeamNotFoundError(FixtureEventSyncError):
    pass


@dataclass(frozen=True, slots=True)
class FixtureEventSyncResult:
    events_synced: int


async def sync_fixture_events(
    session: AsyncSession,
    client: APIFootballClient,
    *,
    fixture_api_id: int,
) -> FixtureEventSyncResult:
    if fixture_api_id < 1:
        raise ValueError("Fixture ID must be greater than zero")

    payload = await client.get(
        "/fixtures/events",
        params={
            "fixture": fixture_api_id,
        },
    )

    raw_entries = payload.get("response")

    if not isinstance(raw_entries, list):
        raise APIFootballResponseError(
            "API-Football response does not contain an events list",
        )

    validated_entries: list[tuple[APIFootballFixtureEvent, dict[str, object]]] = []

    for raw_entry in raw_entries:
        if not isinstance(raw_entry, dict):
            raise APIFootballResponseError(
                "API-Football events list contains an invalid entry",
            )

        validated_entries.append(
            (
                APIFootballFixtureEvent.model_validate(raw_entry),
                raw_entry,
            ),
        )

    async with session.begin():
        context = await get_fixture_event_context(
            session,
            fixture_api_id,
        )

        if context is None:
            raise FixtureNotFoundError(
                f"Fixture {fixture_api_id} was not found",
            )

        fixture_id, team_ids = context
        event_rows: list[dict[str, object]] = []

        for event_order, (event, raw_entry) in enumerate(
            validated_entries,
        ):
            team_id = team_ids.get(event.team.id)

            if team_id is None:
                raise FixtureEventTeamNotFoundError(
                    f"Team {event.team.id} does not belong to fixture {fixture_api_id}",
                )

            event_rows.append(
                {
                    "fixture_id": fixture_id,
                    "team_id": team_id,
                    "event_order": event_order,
                    "elapsed": event.time.elapsed,
                    "extra_time": event.time.extra,
                    "player_api_id": event.player.id,
                    "player_name": event.player.name,
                    "assist_api_id": event.assist.id,
                    "assist_name": event.assist.name,
                    "event_type": event.type,
                    "detail": event.detail,
                    "comments": event.comments,
                    "raw_payload": raw_entry,
                },
            )

        events_synced = await replace_fixture_events(
            session,
            fixture_id=fixture_id,
            event_rows=event_rows,
        )

    return FixtureEventSyncResult(
        events_synced=events_synced,
    )
