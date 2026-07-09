def get_auth_headers(client):
    client.post(
        "/auth/register",
        json={
            "email": "test@example.com",
            "username": "testuser",
            "password": "password123",
        },
    )

    login_response = client.post(
        "/auth/login",
        data={
            "username": "testuser",
            "password": "password123",
        },
    )

    token = login_response.json()["access_token"]

    return {
        "Authorization": f"Bearer {token}",
    }


def create_exercise(client, headers):
    response = client.post(
        "/exercises",
        json={
            "name": "Bench Press",
            "muscle_group": "Chest",
            "equipment": "Barbell",
        },
        headers=headers,
    )

    return response.json()["id"]


def create_workout(client, headers, exercise_id):
    response = client.post(
        "/workouts",
        json={
            "title": "Chest Day",
            "workout_date": "2026-07-05",
            "notes": "Bench press training",
            "sets": [
                {
                    "exercise_id": exercise_id,
                    "set_number": 1,
                    "weight": 80,
                    "reps": 10,
                },
                {
                    "exercise_id": exercise_id,
                    "set_number": 2,
                    "weight": 85,
                    "reps": 8,
                },
            ],
        },
        headers=headers,
    )

    return response.json()["id"]


def test_get_weekly_progress(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)
    create_workout(client, headers, exercise_id)

    response = client.get(
        "/progress/weekly?week_start=2026-07-05",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert data["week_start"] == "2026-07-05"
    assert data["week_end"] == "2026-07-11"
    assert data["workouts_count"] == 1
    assert data["total_sets"] == 2
    assert data["total_reps"] == 18
    assert data["total_volume"] == 1480


def test_get_weekly_progress_without_token(client):
    response = client.get("/progress/weekly?week_start=2026-07-05")

    assert response.status_code == 401


def test_get_exercise_records(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)
    create_workout(client, headers, exercise_id)

    response = client.get(
        f"/progress/exercises/{exercise_id}/records",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert data["exercise_id"] == exercise_id
    assert data["max_weight"] == 85
    assert data["max_reps"] == 10
    assert data["max_volume"] == 800
    assert data["estimated_one_rep_max"] == 107.67


def test_get_exercise_records_without_sets(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)

    response = client.get(
        f"/progress/exercises/{exercise_id}/records",
        headers=headers,
    )

    assert response.status_code == 404
    assert response.json()["detail"] == "No workout sets found for this exercise"