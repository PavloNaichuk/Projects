from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, status

from app.api.dependencies import DBSession
from app.repositories.fixture_event_queries import (
    fixture_exists,
    list_fixture_events,
)
from app.schemas.fixture_events import FixtureEventResponse

router = APIRouter(
    prefix="/fixtures",
    tags=["Fixture Events"],
)


@router.get(
    "/{fixture_api_id}/events",
    response_model=list[FixtureEventResponse],
)
async def read_fixture_events(
    fixture_api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> list[FixtureEventResponse]:
    events = await list_fixture_events(
        session,
        fixture_api_id,
    )

    if not events and not await fixture_exists(
        session,
        fixture_api_id,
    ):
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Fixture not found",
        )

    return [FixtureEventResponse.model_validate(event) for event in events]
