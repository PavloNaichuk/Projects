$Root = Split-Path -Parent $PSScriptRoot

Write-Host "Stopping Redis..." -ForegroundColor Yellow
Set-Location $Root
docker compose down

Write-Host "Redis stopped." -ForegroundColor Green
Write-Host "Close backend and frontend terminal windows manually with Ctrl + C."