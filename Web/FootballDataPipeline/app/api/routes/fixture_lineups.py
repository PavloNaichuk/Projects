from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, status

from app.api.dependencies import DBSession
from app.repositories.fixture_context import fixture_exists
from app.repositories.fixture_lineup_queries import (
    list_fixture_lineups,
)
from app.schemas.fixture_lineups import FixtureLineupResponse

router = APIRouter(
    prefix="/fixtures",
    tags=["Fixture lineups"],
)


@router.get(
    "/{fixture_api_id}/lineups",
    response_model=list[FixtureLineupResponse],
)
async def read_fixture_lineups(
    fixture_api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> list[FixtureLineupResponse]:
    if not await fixture_exists(
        session,
        fixture_api_id=fixture_api_id,
    ):
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Fixture not found",
        )

    lineups = await list_fixture_lineups(
        session,
        fixture_api_id,
    )

    return [FixtureLineupResponse.model_validate(lineup) for lineup in lineups]
