from datetime import date, datetime

from pydantic import BaseModel, Field


class APIFootballLeague(BaseModel):
    id: int = Field(gt=0)
    name: str = Field(min_length=1, max_length=150)
    type: str = Field(min_length=1, max_length=50)
    logo: str | None = Field(max_length=500)


class APIFootballCountry(BaseModel):
    name: str | None = Field(max_length=100)
    code: str | None = Field(max_length=10)
    flag: str | None = Field(max_length=500)


class APIFootballSeason(BaseModel):
    year: int = Field(ge=1, le=9999)
    start: date | None
    end: date | None
    current: bool
    coverage: dict[str, object] | None


class APIFootballLeagueEntry(BaseModel):
    league: APIFootballLeague
    country: APIFootballCountry
    seasons: list[APIFootballSeason]


class APIFootballTeam(BaseModel):
    id: int = Field(gt=0)
    name: str = Field(min_length=1, max_length=150)
    code: str | None = Field(default=None, max_length=10)
    country: str | None = Field(default=None, max_length=100)
    founded: int | None = Field(default=None, ge=1800, le=9999)
    national: bool
    logo: str | None = Field(default=None, max_length=500)


class APIFootballTeamEntry(BaseModel):
    team: APIFootballTeam


class APIFootballVenue(BaseModel):
    id: int | None = Field(default=None, gt=0)
    name: str | None = Field(default=None, max_length=150)
    city: str | None = Field(default=None, max_length=100)


class APIFootballFixtureStatus(BaseModel):
    long: str = Field(min_length=1, max_length=50)
    short: str = Field(min_length=1, max_length=10)
    elapsed: int | None = Field(default=None, ge=0)
    extra: int | None = Field(default=None, ge=0)


class APIFootballFixtureInfo(BaseModel):
    id: int = Field(gt=0)
    referee: str | None = Field(default=None, max_length=150)
    timezone: str = Field(min_length=1, max_length=50)
    date: datetime
    venue: APIFootballVenue
    status: APIFootballFixtureStatus


class APIFootballFixtureLeague(BaseModel):
    id: int = Field(gt=0)
    season: int = Field(ge=1800, le=9999)
    round: str | None = Field(default=None, max_length=100)


class APIFootballFixtureTeam(BaseModel):
    id: int = Field(gt=0)
    name: str = Field(min_length=1, max_length=150)
    logo: str | None = Field(default=None, max_length=500)
    winner: bool | None = None


class APIFootballFixtureTeams(BaseModel):
    home: APIFootballFixtureTeam
    away: APIFootballFixtureTeam


class APIFootballScorePair(BaseModel):
    home: int | None = Field(default=None, ge=0)
    away: int | None = Field(default=None, ge=0)


class APIFootballFixtureScore(BaseModel):
    halftime: APIFootballScorePair
    fulltime: APIFootballScorePair
    extratime: APIFootballScorePair
    penalty: APIFootballScorePair


class APIFootballFixtureEntry(BaseModel):
    fixture: APIFootballFixtureInfo
    league: APIFootballFixtureLeague
    teams: APIFootballFixtureTeams
    goals: APIFootballScorePair
    score: APIFootballFixtureScore


class APIFootballStandingTeam(BaseModel):
    id: int = Field(gt=0)
    name: str = Field(min_length=1, max_length=150)
    logo: str | None = Field(default=None, max_length=500)


class APIFootballStandingGoals(BaseModel):
    goals_for: int = Field(alias="for", ge=0)
    against: int = Field(ge=0)


class APIFootballStandingStats(BaseModel):
    played: int = Field(ge=0)
    win: int = Field(ge=0)
    draw: int = Field(ge=0)
    lose: int = Field(ge=0)
    goals: APIFootballStandingGoals


class APIFootballStandingEntry(BaseModel):
    rank: int = Field(gt=0)
    team: APIFootballStandingTeam
    points: int
    goals_diff: int = Field(alias="goalsDiff")
    group: str | None = Field(default=None, max_length=100)
    form: str | None = Field(default=None, max_length=100)
    status: str | None = Field(default=None, max_length=50)
    description: str | None = Field(default=None, max_length=255)
    all: APIFootballStandingStats
    home: APIFootballStandingStats
    away: APIFootballStandingStats
    update: datetime | None = None


class APIFootballStandingLeague(BaseModel):
    id: int = Field(gt=0)
    name: str = Field(min_length=1, max_length=150)
    season: int = Field(ge=1800, le=9999)
    standings: list[list[APIFootballStandingEntry]]


class APIFootballStandingsResponseEntry(BaseModel):
    league: APIFootballStandingLeague
