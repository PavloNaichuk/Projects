$Root = Split-Path -Parent $PSScriptRoot

Write-Host "Stopping Redis and PostgreSQL..." -ForegroundColor Yellow
Set-Location $Root
docker compose down

Write-Host "Redis and PostgreSQL stopped." -ForegroundColor Green
Write-Host "Close backend and frontend terminal windows manually with Ctrl + C."