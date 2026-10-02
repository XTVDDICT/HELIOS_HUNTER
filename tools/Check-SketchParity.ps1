param(
  [string]$Root = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'
$ili = Join-Path $Root 'HELIOS_HUNTER_ILI9341'
$st = Join-Path $Root 'HELIOS_HUNTER_ST7789'
$commonFiles = @(
  'HeliosAssets.h',
  'HeliosBalances.cpp',
  'HeliosBalances.h',
  'HeliosCoins.cpp',
  'HeliosCoins.h',
  'HeliosMiner.h',
  'HeliosSettings.cpp',
  'HeliosSettings.h',
  'HeliosWeb.cpp',
  'HeliosWeb.h',
  'HeliosWebPageGzip.h',
  'SoloHunterMiner.cpp',
  'SoloHunterMiner.h',
  'SoloHunterSha256.cpp',
  'SoloHunterSha256.h'
)

$mismatches = @()
foreach ($name in $commonFiles) {
  $iliPath = Join-Path $ili $name
  $stPath = Join-Path $st $name
  if (!(Test-Path -LiteralPath $iliPath) -or !(Test-Path -LiteralPath $stPath)) {
    $mismatches += "$name (missing)"
    continue
  }
  if ((Get-FileHash -LiteralPath $iliPath -Algorithm SHA256).Hash -ne
      (Get-FileHash -LiteralPath $stPath -Algorithm SHA256).Hash) {
    $mismatches += $name
  }
}

$iliSketch = Join-Path $ili 'HELIOS_HUNTER_ILI9341.ino'
$stSketch = Join-Path $st 'HELIOS_HUNTER_ST7789.ino'
if ((Get-FileHash -LiteralPath $iliSketch -Algorithm SHA256).Hash -ne
    (Get-FileHash -LiteralPath $stSketch -Algorithm SHA256).Hash) {
  $mismatches += 'main sketch (.ino)'
}

if ($mismatches.Count -gt 0) {
  Write-Error ('Screen variants have drifted: ' + ($mismatches -join ', '))
  exit 1
}

Write-Host "Sketch parity OK: $($commonFiles.Count + 1) shared files match."
