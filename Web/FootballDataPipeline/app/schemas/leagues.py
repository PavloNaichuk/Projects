from datetime import date

from pydantic import BaseModel, ConfigDict


class SeasonResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    year: int
    start_date: date | None
    end_date: date | None
    is_current: bool
    coverage: dict[str, object] | None


class LeagueResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    league_type: str
    country_name: str | None
    country_code: str | None
    logo_url: str | None
    country_flag_url: str | None


class LeagueDetailResponse(LeagueResponse):
    seasons: list[SeasonResponse]
