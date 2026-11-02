param(
    [string]$Root = "D:\RMUA2026-re"
)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path $Root | Out-Null
$zip = Join-Path $Root "simulator_12.0.0.5.zip"
$url = "https://sz-rm-rmua-dispatch-prod.oss-cn-shenzhen.aliyuncs.com/b39a0194e982f0d987153c6016feb325/simulator_12.0.0.5.zip"
$expected = "3FA858942D58DF1FD37D3779A2F1942333F61CD01E87EFC25AD216EEA988295E"

if (-not (Test-Path $zip)) {
    Write-Host "Downloading $url"
    curl.exe -L --retry 5 --retry-all-errors --continue-at - -o $zip $url
}
if (-not (Test-Path $zip)) { throw "Download failed: $zip" }

$hash = (Get-FileHash $zip -Algorithm SHA256).Hash
Write-Host "SHA256: $hash"
if ($hash -ne $expected) {
    throw "SHA256 mismatch. expected=$expected actual=$hash"
}

$sim = Join-Path $Root "sim"
New-Item -ItemType Directory -Force -Path $sim | Out-Null
if (-not (Test-Path (Join-Path $sim "simulator_12.0.0.5"))) {
    tar.exe -xf $zip -C $sim
}
Write-Host "Extracted to $sim"
