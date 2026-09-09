from pydantic import BaseModel, ConfigDict


class TeamResponse(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    api_id: int
    name: str
    code: str | None
    country_name: str | None
    founded: int | None
    is_national: bool
    logo_url: str | None
