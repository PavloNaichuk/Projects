from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, Response, status

from app.api.dependencies import CacheClient, DBSession
from app.cache.fixture_details import (
    get_cached_fixture_details,
    set_cached_fixture_details,
)
from app.core.config import get_settings
from app.repositories.fixture_event_queries import (
    list_fixture_events,
)
from app.repositories.fixture_lineup_queries import (
    list_fixture_lineups,
)
from app.repositories.fixture_queries import get_fixture_by_api_id
from app.repositories.fixture_statistic_queries import (
    list_fixture_statistics,
)
from app.schemas.fixture_details import FixtureDetailsResponse
from app.schemas.fixture_events import FixtureEventResponse
from app.schemas.fixture_lineups import FixtureLineupResponse
from app.schemas.fixture_statistics import FixtureStatisticResponse
from app.schemas.fixtures import FixtureResponse

router = APIRouter(
    prefix="/fixtures",
    tags=["Fixture details"],
)


@router.get(
    "/{fixture_api_id}/details",
    response_model=FixtureDetailsResponse,
)
async def read_fixture_details(
    fixture_api_id: Annotated[int, Path(gt=0)],
    response: Response,
    session: DBSession,
    cache_client: CacheClient,
) -> FixtureDetailsResponse:
    cached_details = await get_cached_fixture_details(
        cache_client,
        fixture_api_id,
    )

    if cached_details is not None:
        response.headers["X-Cache"] = "HIT"
        return cached_details

    fixture = await get_fixture_by_api_id(
        session,
        fixture_api_id,
    )

    if fixture is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Fixture not found",
        )

    events = await list_fixture_events(
        session,
        fixture_api_id,
    )
    statistics = await list_fixture_statistics(
        session,
        fixture_api_id,
    )
    lineups = await list_fixture_lineups(
        session,
        fixture_api_id,
    )

    fixture_details = FixtureDetailsResponse(
        fixture=FixtureResponse.model_validate(fixture),
        events=[FixtureEventResponse.model_validate(event) for event in events],
        statistics=[
            FixtureStatisticResponse.model_validate(statistic)
            for statistic in statistics
        ],
        lineups=[FixtureLineupResponse.model_validate(lineup) for lineup in lineups],
    )

    settings = get_settings()

    await set_cached_fixture_details(
        cache_client,
        fixture_api_id,
        fixture_details,
        ttl_seconds=settings.cache_ttl_seconds,
    )

    response.headers["X-Cache"] = "MISS"

    return fixture_details
