from pydantic import BaseModel, ConfigDict


class FixtureEventTeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    logo_url: str | None


class FixtureEventResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    event_order: int
    elapsed: int
    extra_time: int | None

    team: FixtureEventTeamResponse

    player_api_id: int | None
    player_name: str | None
    assist_api_id: int | None
    assist_name: str | None

    event_type: str
    detail: str
    comments: str | None
