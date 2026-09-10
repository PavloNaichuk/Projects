from datetime import datetime

from pydantic import BaseModel, ConfigDict


class FixtureLeagueResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str


class FixtureSeasonResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    year: int
    league: FixtureLeagueResponse


class FixtureTeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    logo_url: str | None


class FixtureResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    season: FixtureSeasonResponse

    home_team: FixtureTeamResponse
    away_team: FixtureTeamResponse

    referee: str | None
    kickoff_at: datetime
    timezone: str
    round_name: str | None

    venue_api_id: int | None
    venue_name: str | None
    venue_city: str | None

    status_long: str
    status_short: str
    elapsed: int | None
    extra_time: int | None

    home_goals: int | None
    away_goals: int | None

    halftime_home: int | None
    halftime_away: int | None
    fulltime_home: int | None
    fulltime_away: int | None
    extra_home: int | None
    extra_away: int | None
    penalty_home: int | None
    penalty_away: int | None
