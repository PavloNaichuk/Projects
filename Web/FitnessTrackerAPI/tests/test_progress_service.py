from types import SimpleNamespace

import pytest

from app.services.progress_service import (
    calculate_set_volume,
    estimate_one_rep_max,
)


def test_calculate_set_volume():
    workout_set = SimpleNamespace(
        weight=80,
        reps=10,
    )

    result = calculate_set_volume(workout_set)

    assert result == 800


def test_calculate_set_volume_with_zero_weight():
    workout_set = SimpleNamespace(
        weight=0,
        reps=12,
    )

    result = calculate_set_volume(workout_set)

    assert result == 0


def test_estimate_one_rep_max():
    result = estimate_one_rep_max(weight=80, reps=10)

    assert result == pytest.approx(106.67, rel=1e-2)


def test_estimate_one_rep_max_rounded():
    result = estimate_one_rep_max(weight=85, reps=8)

    assert round(result, 2) == 107.67