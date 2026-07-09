from fastapi import FastAPI

from app.api.routes.auth import router as auth_router
from app.api.routes.exercises import router as exercises_router
from app.api.routes.measurements import router as measurements_router
from app.api.routes.progress import router as progress_router
from app.api.routes.reports import router as reports_router
from app.api.routes.workouts import router as workouts_router
from app.core.config import settings

app = FastAPI(
    title=settings.PROJECT_NAME,
    description="Backend API for tracking workouts, exercises, and progress.",
    version="0.1.0",
)

app.include_router(auth_router)
app.include_router(exercises_router)
app.include_router(workouts_router)
app.include_router(progress_router)
app.include_router(measurements_router)
app.include_router(reports_router)


@app.get("/")
def root():
    return {
        "message": "Fitness Tracker API is running"
    }


@app.get("/health")
def health_check():
    return {
        "status": "ok"
    }