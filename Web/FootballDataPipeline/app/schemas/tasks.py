from pydantic import BaseModel, Field


class LeagueSyncRequest(BaseModel):
    league_id: int = Field(gt=0)


class TaskSubmittedResponse(BaseModel):
    task_id: str
    status: str


class TaskStatusResponse(BaseModel):
    task_id: str
    status: str
    result: dict[str, int] | None = None
    error: str | None = None
