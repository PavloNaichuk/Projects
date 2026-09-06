from datetime import date

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
