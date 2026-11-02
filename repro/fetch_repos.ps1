param(
    [string]$Root = "D:\RMUA2026-re"
)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path $Root | Out-Null

if (-not (Test-Path (Join-Path $Root "Simulator\.git"))) {
    git clone --depth 1 -b RMUA2026-01 https://github.com/RoboMaster/IntelligentUAVChampionshipSimulator (Join-Path $Root "Simulator")
}
if (-not (Test-Path (Join-Path $Root "Base\.git"))) {
    git clone --depth 1 -b RMUA2026 https://github.com/RoboMaster/IntelligentUAVChampionshipBase (Join-Path $Root "Base")
}

Write-Host "Simulator branch:"
git -C (Join-Path $Root "Simulator") branch --show-current
Write-Host "Base branch:"
git -C (Join-Path $Root "Base") branch --show-current
