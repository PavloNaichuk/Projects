from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, Query, status

from app.api.dependencies import DBSession
from app.repositories.team_queries import (
    get_team_by_api_id,
    list_teams,
)
from app.schemas.teams import TeamResponse

router = APIRouter(
    prefix="/teams",
    tags=["Teams"],
)


@router.get(
    "",
    response_model=list[TeamResponse],
)
async def read_teams(
    session: DBSession,
    offset: Annotated[int, Query(ge=0)] = 0,
    limit: Annotated[int, Query(ge=1, le=100)] = 50,
    country: Annotated[
        str | None,
        Query(min_length=1, max_length=100),
    ] = None,
    search: Annotated[
        str | None,
        Query(min_length=2, max_length=100),
    ] = None,
) -> list[TeamResponse]:
    teams = await list_teams(
        session,
        offset=offset,
        limit=limit,
        country=country,
        search=search,
    )

    return [TeamResponse.model_validate(team) for team in teams]


@router.get(
    "/{api_id}",
    response_model=TeamResponse,
)
async def read_team(
    api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> TeamResponse:
    team = await get_team_by_api_id(
        session,
        api_id,
    )

    if team is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Team not found",
        )

    return TeamResponse.model_validate(team)
