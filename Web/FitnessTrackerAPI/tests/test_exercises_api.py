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


def test_create_exercise(client):
    headers = get_auth_headers(client)

    response = client.post(
        "/exercises",
        json={
            "name": "Bench Press",
            "muscle_group": "Chest",
            "equipment": "Barbell",
        },
        headers=headers,
    )

    assert response.status_code == 201

    data = response.json()

    assert data["name"] == "Bench Press"
    assert data["muscle_group"] == "Chest"
    assert data["equipment"] == "Barbell"
    assert "id" in data
    assert "created_at" in data


def test_create_exercise_without_token(client):
    response = client.post(
        "/exercises",
        json={
            "name": "Squat",
            "muscle_group": "Legs",
            "equipment": "Barbell",
        },
    )

    assert response.status_code == 401


def test_create_duplicate_exercise(client):
    headers = get_auth_headers(client)

    exercise_data = {
        "name": "Deadlift",
        "muscle_group": "Back",
        "equipment": "Barbell",
    }

    first_response = client.post(
        "/exercises",
        json=exercise_data,
        headers=headers,
    )

    second_response = client.post(
        "/exercises",
        json=exercise_data,
        headers=headers,
    )

    assert first_response.status_code == 201
    assert second_response.status_code == 400
    assert second_response.json()["detail"] == "Exercise already exists"


def test_get_exercises(client):
    headers = get_auth_headers(client)

    client.post(
        "/exercises",
        json={
            "name": "Bench Press",
            "muscle_group": "Chest",
            "equipment": "Barbell",
        },
        headers=headers,
    )

    client.post(
        "/exercises",
        json={
            "name": "Push Up",
            "muscle_group": "Chest",
            "equipment": "Bodyweight",
        },
        headers=headers,
    )

    response = client.get("/exercises?muscle_group=Chest")

    assert response.status_code == 200

    data = response.json()

    assert len(data) == 2
    assert data[0]["muscle_group"] == "Chest"


def test_get_exercise_by_id(client):
    headers = get_auth_headers(client)

    create_response = client.post(
        "/exercises",
        json={
            "name": "Pull Up",
            "muscle_group": "Back",
            "equipment": "Bodyweight",
        },
        headers=headers,
    )

    exercise_id = create_response.json()["id"]

    response = client.get(f"/exercises/{exercise_id}")

    assert response.status_code == 200

    data = response.json()

    assert data["id"] == exercise_id
    assert data["name"] == "Pull Up"


def test_get_exercise_not_found(client):
    response = client.get("/exercises/999")

    assert response.status_code == 404
    assert response.json()["detail"] == "Exercise not found"