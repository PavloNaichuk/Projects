from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, Query, status

from app.api.dependencies import DBSession
from app.repositories.league_queries import (
    get_league_by_api_id,
    list_leagues,
)
from app.schemas.leagues import (
    LeagueDetailResponse,
    LeagueResponse,
)

router = APIRouter(
    prefix="/leagues",
    tags=["Leagues"],
)


@router.get(
    "",
    response_model=list[LeagueResponse],
)
async def read_leagues(
    session: DBSession,
    offset: Annotated[int, Query(ge=0)] = 0,
    limit: Annotated[int, Query(ge=1, le=100)] = 50,
) -> list[LeagueResponse]:
    leagues = await list_leagues(
        session,
        offset=offset,
        limit=limit,
    )

    return [LeagueResponse.model_validate(league) for league in leagues]


@router.get(
    "/{api_id}",
    response_model=LeagueDetailResponse,
)
async def read_league(
    api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> LeagueDetailResponse:
    league = await get_league_by_api_id(
        session,
        api_id,
    )

    if league is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="League not found",
        )

    return LeagueDetailResponse.model_validate(league)
