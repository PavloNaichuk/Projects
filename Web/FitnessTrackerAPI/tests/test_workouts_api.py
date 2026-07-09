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


def test_create_workout(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)

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

    assert response.status_code == 201

    data = response.json()

    assert data["title"] == "Chest Day"
    assert data["workout_date"] == "2026-07-05"
    assert data["notes"] == "Bench press training"
    assert len(data["sets"]) == 2
    assert data["sets"][0]["weight"] == 80
    assert data["sets"][0]["reps"] == 10


def test_create_workout_without_token(client):
    response = client.post(
        "/workouts",
        json={
            "title": "Chest Day",
            "workout_date": "2026-07-05",
            "notes": "No auth",
            "sets": [],
        },
    )

    assert response.status_code == 401


def test_create_workout_with_missing_exercise(client):
    headers = get_auth_headers(client)

    response = client.post(
        "/workouts",
        json={
            "title": "Back Day",
            "workout_date": "2026-07-05",
            "notes": "Missing exercise",
            "sets": [
                {
                    "exercise_id": 999,
                    "set_number": 1,
                    "weight": 100,
                    "reps": 5,
                }
            ],
        },
        headers=headers,
    )

    assert response.status_code == 404
    assert response.json()["detail"] == "Exercise with id 999 not found"


def test_get_workouts(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)

    client.post(
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
                }
            ],
        },
        headers=headers,
    )

    response = client.get(
        "/workouts",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert len(data) == 1
    assert data[0]["title"] == "Chest Day"
    assert len(data[0]["sets"]) == 1


def test_get_workout_by_id(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)

    create_response = client.post(
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
                }
            ],
        },
        headers=headers,
    )

    workout_id = create_response.json()["id"]

    response = client.get(
        f"/workouts/{workout_id}",
        headers=headers,
    )

    assert response.status_code == 200

    data = response.json()

    assert data["id"] == workout_id
    assert data["title"] == "Chest Day"


def test_get_workout_not_found(client):
    headers = get_auth_headers(client)

    response = client.get(
        "/workouts/999",
        headers=headers,
    )

    assert response.status_code == 404
    assert response.json()["detail"] == "Workout not found"


def test_delete_workout(client):
    headers = get_auth_headers(client)
    exercise_id = create_exercise(client, headers)

    create_response = client.post(
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
                }
            ],
        },
        headers=headers,
    )

    workout_id = create_response.json()["id"]

    delete_response = client.delete(
        f"/workouts/{workout_id}",
        headers=headers,
    )

    assert delete_response.status_code == 204

    get_response = client.get(
        f"/workouts/{workout_id}",
        headers=headers,
    )

    assert get_response.status_code == 404