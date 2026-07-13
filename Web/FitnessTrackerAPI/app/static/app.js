let accessToken = localStorage.getItem("accessToken") || "";
let currentUsername = localStorage.getItem("currentUsername") || "";

updatePage();

if (accessToken) {
    loadCurrentUser();
}

function updatePage() {
    const status = document.getElementById("loginStatus");
    const logoutButton = document.getElementById("logoutButton");
    const authPanel = document.getElementById("authPanel");
    const dashboardPanel = document.getElementById("dashboardPanel");

    if (accessToken) {
        status.textContent = currentUsername
            ? `${currentUsername}`
            : "Logged in";

        status.classList.add("logged");
        logoutButton.classList.add("show");
        authPanel.classList.add("hidden");
        dashboardPanel.classList.remove("hidden");
    } else {
        status.textContent = "Not logged in";
        status.classList.remove("logged");
        logoutButton.classList.remove("show");
        authPanel.classList.remove("hidden");
        dashboardPanel.classList.add("hidden");
    }
}

let messageTimer = null;

function showMessage(text, type = "success") {
    const box = document.getElementById("globalMessage");

    box.textContent = text;
    box.className = `message show ${type}`;

    clearTimeout(messageTimer);

    messageTimer = setTimeout(() => {
        clearMessage();
    }, 2500);
}

function clearMessage() {
    const box = document.getElementById("globalMessage");
    box.textContent = "";
    box.className = "message";
}

function logoutUser() {
    accessToken = "";
    currentUsername = "";

    localStorage.removeItem("accessToken");
    localStorage.removeItem("currentUsername");

    clearAllResults();
    clearMessage();
    updatePage();
}

function clearAllResults() {
    setEmpty("exerciseResult");
    setEmpty("workoutResult");
    setEmpty("measurementResult");
    setEmpty("progressResult");
    setEmpty("reportResult");
}

function setEmpty(elementId) {
    document.getElementById(elementId).innerHTML = '<span class="empty-text">No data loaded yet.</span>';
}

function headersWithAuth() {
    return {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${accessToken}`
    };
}

async function parseResponse(response) {
    const data = await response.json().catch(() => ({}));

    if (!response.ok) {
        const message = data.detail || "Request failed";
        throw new Error(message);
    }

    return data;
}

async function loadCurrentUser() {
    try {
        const response = await fetch("/auth/me", {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        currentUsername = data.username;
        localStorage.setItem("currentUsername", currentUsername);
        updatePage();
    } catch (error) {
        logoutUser();
    }
}

async function registerUser() {
    clearMessage();

    try {
        const response = await fetch("/auth/register", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                email: document.getElementById("registerEmail").value,
                username: document.getElementById("registerUsername").value,
                password: document.getElementById("registerPassword").value
            })
        });

        await parseResponse(response);
        showMessage("User registered successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loginUser() {
    clearMessage();

    try {
        const formData = new URLSearchParams();
        formData.append("username", document.getElementById("loginUsername").value);
        formData.append("password", document.getElementById("loginPassword").value);

        const response = await fetch("/auth/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: formData
        });

        const data = await parseResponse(response);

        accessToken = data.access_token;
        localStorage.setItem("accessToken", accessToken);

        await loadCurrentUser();
        showMessage("Logged in successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function createExercise() {
    try {
        const response = await fetch("/exercises", {
            method: "POST",
            headers: headersWithAuth(),
            body: JSON.stringify({
                name: document.getElementById("exerciseName").value,
                muscle_group: document.getElementById("muscleGroup").value,
                equipment: document.getElementById("equipment").value
            })
        });

        await parseResponse(response);
        showMessage("Exercise created successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadExercises() {
    try {
        const response = await fetch("/exercises");
        const data = await parseResponse(response);

        if (!data.length) {
            setEmpty("exerciseResult");
            return;
        }

        document.getElementById("exerciseResult").innerHTML = `
            <ul class="list">
                ${data.map(item => `
                    <li><strong>#${item.id}</strong> ${item.name} — ${item.muscle_group} (${item.equipment})</li>
                `).join("")}
            </ul>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function createWorkout() {
    try {
        const response = await fetch("/workouts", {
            method: "POST",
            headers: headersWithAuth(),
            body: JSON.stringify({
                title: document.getElementById("workoutTitle").value,
                workout_date: document.getElementById("workoutDate").value,
                notes: document.getElementById("workoutNotes").value,
                sets: [
                    {
                        exercise_id: Number(document.getElementById("workoutExerciseId").value),
                        set_number: 1,
                        weight: Number(document.getElementById("workoutWeight").value),
                        reps: Number(document.getElementById("workoutReps").value)
                    }
                ]
            })
        });

        await parseResponse(response);
        showMessage("Workout created successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadWorkouts() {
    try {
        const response = await fetch("/workouts", {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        if (!data.length) {
            setEmpty("workoutResult");
            return;
        }

        document.getElementById("workoutResult").innerHTML = `
            <ul class="list">
                ${data.map(item => `
                    <li><strong>#${item.id}</strong> ${item.title} — ${item.workout_date} (${item.sets.length} sets)</li>
                `).join("")}
            </ul>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function createMeasurement() {
    try {
        const response = await fetch("/measurements", {
            method: "POST",
            headers: headersWithAuth(),
            body: JSON.stringify({
                measured_at: document.getElementById("measuredAt").value,
                body_weight: Number(document.getElementById("bodyWeight").value),
                body_fat: Number(document.getElementById("bodyFat").value),
                waist: Number(document.getElementById("waist").value)
            })
        });

        await parseResponse(response);
        showMessage("Measurement created successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadMeasurements() {
    try {
        const response = await fetch("/measurements", {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        if (!data.length) {
            setEmpty("measurementResult");
            return;
        }

        document.getElementById("measurementResult").innerHTML = `
            <ul class="list">
                ${data.map(item => `
                    <li>${item.measured_at} — weight: ${item.body_weight}, body fat: ${item.body_fat ?? "-"}, waist: ${item.waist ?? "-"}</li>
                `).join("")}
            </ul>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadLatestMeasurement() {
    try {
        const response = await fetch("/measurements/latest", {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        document.getElementById("measurementResult").innerHTML = `
            <div class="small-info">
                <strong>Latest Date:</strong> ${data.measured_at}<br>
                <strong>Body Weight:</strong> ${data.body_weight}<br>
                <strong>Body Fat:</strong> ${data.body_fat ?? "-"}<br>
                <strong>Waist:</strong> ${data.waist ?? "-"}
            </div>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadWeeklyProgress() {
    try {
        const weekStart = document.getElementById("weekStart").value;

        const response = await fetch(`/progress/weekly?week_start=${weekStart}`, {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        document.getElementById("progressResult").innerHTML = `
            <div class="small-info">
                <strong>Week Start:</strong> ${data.week_start}<br>
                <strong>Week End:</strong> ${data.week_end}<br>
                <strong>Workouts:</strong> ${data.workouts_count}<br>
                <strong>Total Sets:</strong> ${data.total_sets}<br>
                <strong>Total Reps:</strong> ${data.total_reps}<br>
                <strong>Total Volume:</strong> ${data.total_volume}
            </div>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadExerciseRecords() {
    try {
        const exerciseId = document.getElementById("recordExerciseId").value;

        const response = await fetch(`/progress/exercises/${exerciseId}/records`, {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        document.getElementById("progressResult").innerHTML = `
            <div class="small-info">
                <strong>Exercise ID:</strong> ${data.exercise_id}<br>
                <strong>Max Weight:</strong> ${data.max_weight}<br>
                <strong>Max Reps:</strong> ${data.max_reps}<br>
                <strong>Max Volume:</strong> ${data.max_volume}<br>
                <strong>Estimated 1RM:</strong> ${data.estimated_one_rep_max}
            </div>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function generateWeeklyReport() {
    try {
        const weekStart = document.getElementById("reportWeekStart").value;

        const response = await fetch(`/reports/weekly/generate?week_start=${weekStart}`, {
            method: "POST",
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        await parseResponse(response);
        showMessage("Weekly report generated successfully.", "success");
    } catch (error) {
        showMessage(error.message, "error");
    }
}

async function loadWeeklyReports() {
    try {
        const response = await fetch("/reports/weekly", {
            headers: {
                "Authorization": `Bearer ${accessToken}`
            }
        });

        const data = await parseResponse(response);

        if (!data.length) {
            setEmpty("reportResult");
            return;
        }

        document.getElementById("reportResult").innerHTML = `
            <ul class="list">
                ${data.map(item => `
                    <li>${item.week_start} → ${item.week_end} | workouts: ${item.workouts_count}, volume: ${item.total_volume}</li>
                `).join("")}
            </ul>
        `;
    } catch (error) {
        showMessage(error.message, "error");
    }
}