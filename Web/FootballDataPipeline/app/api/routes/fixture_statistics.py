from typing import Annotated

from fastapi import APIRouter, HTTPException, Path, status

from app.api.dependencies import DBSession
from app.repositories.fixture_context import fixture_exists
from app.repositories.fixture_statistic_queries import (
    list_fixture_statistics,
)
from app.schemas.fixture_statistics import FixtureStatisticResponse

router = APIRouter(
    prefix="/fixtures",
    tags=["Fixture Statistics"],
)


@router.get(
    "/{fixture_api_id}/statistics",
    response_model=list[FixtureStatisticResponse],
)
async def read_fixture_statistics(
    fixture_api_id: Annotated[int, Path(gt=0)],
    session: DBSession,
) -> list[FixtureStatisticResponse]:
    statistics = await list_fixture_statistics(
        session,
        fixture_api_id,
    )

    if not statistics and not await fixture_exists(
        session,
        fixture_api_id,
    ):
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Fixture not found",
        )

    return [
        FixtureStatisticResponse.model_validate(statistic) for statistic in statistics
    ]
