from app.models.workout import WorkoutSet


def calculate_set_volume(workout_set: WorkoutSet) -> float:
    return workout_set.weight * workout_set.reps


def estimate_one_rep_max(weight: float, reps: int) -> float:
    return weight * (1 + reps / 30)