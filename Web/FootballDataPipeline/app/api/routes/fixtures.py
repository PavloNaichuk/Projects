from datetime import date
from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, Query, status

from app.api.dependencies import DBSession
from app.repositories.fixture_queries import (
    get_fixture_by_api_id,
    list_fixtures,
)
from app.schemas.fixtures import FixtureResponse

router = APIRouter(
    prefix="/fixtures",
    tags=["Fixtures"],
)


@router.get(
    "",
    response_model=list[FixtureResponse],
)
async def read_fixtures(
    session: DBSession,
    offset: Annotated[int, Query(ge=0)] = 0,
    limit: Annotated[int, Query(ge=1, le=100)] = 50,
    league_id: Annotated[int | None, Query(gt=0)] = None,
    season: Annotated[
        int | None,
        Query(ge=1800, le=9999),
    ] = None,
    team_id: Annotated[int | None, Query(gt=0)] = None,
    status_short: Annotated[
        str | None,
        Query(
            alias="status",
            min_length=1,
            max_length=10,
        ),
    ] = None,
    date_from: Annotated[date | None, Query()] = None,
    date_to: Annotated[date | None, Query()] = None,
) -> list[FixtureResponse]:
    if date_from is not None and date_to is not None and date_from > date_to:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="date_from must not be later than date_to",
        )

    fixtures = await list_fixtures(
        session,
        offset=offset,
        limit=limit,
        league_id=league_id,
        season=season,
        team_id=team_id,
        status_short=status_short,
        date_from=date_from,
        date_to=date_to,
    )

    return [FixtureResponse.model_validate(fixture) for fixture in fixtures]


@router.get(
    "/{api_id}",
    response_model=FixtureResponse,
)
async def read_fixture(
    api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> FixtureResponse:
    fixture = await get_fixture_by_api_id(
        session,
        api_id,
    )

    if fixture is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Fixture not found",
        )

    return FixtureResponse.model_validate(fixture)
