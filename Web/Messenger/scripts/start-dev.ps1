$Root = Split-Path -Parent $PSScriptRoot

Write-Host "Starting Redis and PostgreSQL..." -ForegroundColor Green
Set-Location $Root
docker compose up -d redis postgres

Write-Host "Starting backend..." -ForegroundColor Green
Start-Process powershell -ArgumentList @(
    "-NoExit",
    "-Command",
    "cd `"$Root\backend`"; .\venv\Scripts\Activate.ps1; python manage.py runserver"
)

Write-Host "Starting frontend..." -ForegroundColor Green
Start-Process powershell -ArgumentList @(
    "-NoExit",
    "-Command",
    "cd `"$Root\frontend`"; npm run dev"
)

Write-Host ""
Write-Host "Development environment started." -ForegroundColor Green
Write-Host "Backend:    http://127.0.0.1:8000/"
Write-Host "Frontend:   http://localhost:5173/"
Write-Host "PostgreSQL: 127.0.0.1:5432"
Write-Host "Redis:      127.0.0.1:6379"