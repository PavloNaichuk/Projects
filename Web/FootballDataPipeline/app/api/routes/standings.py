from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, Query, status

from app.api.dependencies import DBSession
from app.repositories.standing_queries import (
    get_league_standings,
    list_standings,
)
from app.schemas.standings import StandingResponse

router = APIRouter(
    prefix="/standings",
    tags=["Standings"],
)


@router.get(
    "",
    response_model=list[StandingResponse],
)
async def read_standings(
    session: DBSession,
    offset: Annotated[int, Query(ge=0)] = 0,
    limit: Annotated[int, Query(ge=1, le=100)] = 100,
    league_id: Annotated[int | None, Query(gt=0)] = None,
    season: Annotated[
        int | None,
        Query(ge=1800, le=9999),
    ] = None,
    team_id: Annotated[int | None, Query(gt=0)] = None,
) -> list[StandingResponse]:
    standings = await list_standings(
        session,
        offset=offset,
        limit=limit,
        league_id=league_id,
        season=season,
        team_id=team_id,
    )

    return [StandingResponse.model_validate(standing) for standing in standings]


@router.get(
    "/{league_id}/{season}",
    response_model=list[StandingResponse],
)
async def read_league_standings(
    league_id: Annotated[int, Path(gt=0)],
    season: Annotated[
        int,
        Path(ge=1800, le=9999),
    ],
    session: DBSession,
) -> list[StandingResponse]:
    standings = await get_league_standings(
        session,
        league_id=league_id,
        season=season,
    )

    if not standings:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Standings not found",
        )

    return [StandingResponse.model_validate(standing) for standing in standings]
