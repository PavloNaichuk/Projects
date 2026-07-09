from datetime import date

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from app.core.database import Base
from app.models.exercise import Exercise
from app.models.report import WeeklyReport
from app.models.user import User
from app.models.workout import Workout, WorkoutSet
from app.services.report_service import generate_weekly_report


def create_test_db_session():
    engine = create_engine("sqlite:///:memory:")
    TestingSessionLocal = sessionmaker(
        autocommit=False,
        autoflush=False,
        bind=engine,
    )

    Base.metadata.create_all(bind=engine)

    return TestingSessionLocal()


def test_generate_weekly_report():
    db = create_test_db_session()

    user = User(
        email="test@example.com",
        username="testuser",
        password_hash="hashed-password",
    )

    exercise = Exercise(
        name="Bench Press",
        muscle_group="Chest",
        equipment="Barbell",
    )

    db.add(user)
    db.add(exercise)
    db.commit()
    db.refresh(user)
    db.refresh(exercise)

    workout = Workout(
        user_id=user.id,
        title="Chest Day",
        workout_date=date(2026, 7, 5),
        notes="Good workout",
    )

    db.add(workout)
    db.commit()
    db.refresh(workout)

    first_set = WorkoutSet(
        workout_id=workout.id,
        exercise_id=exercise.id,
        set_number=1,
        weight=80,
        reps=10,
    )

    second_set = WorkoutSet(
        workout_id=workout.id,
        exercise_id=exercise.id,
        set_number=2,
        weight=85,
        reps=8,
    )

    db.add(first_set)
    db.add(second_set)
    db.commit()

    report = generate_weekly_report(
        db=db,
        user_id=user.id,
        week_start=date(2026, 7, 5),
    )

    assert report.user_id == user.id
    assert report.week_start == date(2026, 7, 5)
    assert report.week_end == date(2026, 7, 11)
    assert report.workouts_count == 1
    assert report.total_sets == 2
    assert report.total_reps == 18
    assert report.total_volume == 1480
    assert report.new_records_count == 0

    db.close()


def test_generate_weekly_report_updates_existing_report():
    db = create_test_db_session()

    user = User(
        email="test2@example.com",
        username="testuser2",
        password_hash="hashed-password",
    )

    db.add(user)
    db.commit()
    db.refresh(user)

    existing_report = WeeklyReport(
        user_id=user.id,
        week_start=date(2026, 7, 5),
        week_end=date(2026, 7, 11),
        workouts_count=5,
        total_sets=20,
        total_reps=100,
        total_volume=5000,
        new_records_count=2,
    )

    db.add(existing_report)
    db.commit()
    db.refresh(existing_report)

    report = generate_weekly_report(
        db=db,
        user_id=user.id,
        week_start=date(2026, 7, 5),
    )

    assert report.id == existing_report.id
    assert report.workouts_count == 0
    assert report.total_sets == 0
    assert report.total_reps == 0
    assert report.total_volume == 0
    assert report.new_records_count == 0

    db.close()