from pydantic import BaseModel, ConfigDict, Field


class FixtureLineupTeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    logo_url: str | None


class FixtureLineupPlayerResponse(BaseModel):
    id: int | None = None
    name: str | None = None
    number: int | None = None
    position: str | None = Field(
        default=None,
        validation_alias="pos",
    )
    grid: str | None = None


class FixtureLineupResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    team: FixtureLineupTeamResponse
    formation: str | None
    coach_api_id: int | None
    coach_name: str | None
    coach_photo_url: str | None
    colors: dict[str, object] | None
    starting_xi: list[FixtureLineupPlayerResponse]
    substitutes: list[FixtureLineupPlayerResponse]
