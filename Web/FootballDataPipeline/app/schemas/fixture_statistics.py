from pydantic import BaseModel, ConfigDict


class FixtureStatisticTeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    logo_url: str | None


class FixtureStatisticResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    team: FixtureStatisticTeamResponse
    statistics: dict[str, int | float | str | None]
