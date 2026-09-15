from datetime import datetime

from pydantic import BaseModel, ConfigDict


class StandingLeagueResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str


class StandingSeasonResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    year: int
    league: StandingLeagueResponse


class StandingTeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    logo_url: str | None


class StandingResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    season: StandingSeasonResponse
    team: StandingTeamResponse

    position: int
    points: int
    goals_diff: int

    group_name: str | None
    form: str | None
    status: str | None
    description: str | None

    played: int
    wins: int
    draws: int
    losses: int
    goals_for: int
    goals_against: int

    home_stats: dict[str, object]
    away_stats: dict[str, object]
    source_updated_at: datetime | None
