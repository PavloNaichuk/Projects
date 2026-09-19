from pydantic import BaseModel

from app.schemas.fixture_events import FixtureEventResponse
from app.schemas.fixture_lineups import FixtureLineupResponse
from app.schemas.fixture_statistics import FixtureStatisticResponse
from app.schemas.fixtures import FixtureResponse


class FixtureDetailsResponse(BaseModel):
    fixture: FixtureResponse
    events: list[FixtureEventResponse]
    statistics: list[FixtureStatisticResponse]
    lineups: list[FixtureLineupResponse]
