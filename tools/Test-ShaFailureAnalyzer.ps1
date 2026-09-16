$ErrorActionPreference = 'Stop'
$analyzer = Join-Path $PSScriptRoot 'Analyze-ShaFailure.ps1'
$temp = Join-Path ([IO.Path]::GetTempPath()) ('helios-sha-test-' + [guid]::NewGuid() + '.json')
$header = '01000000' + ('00' * 32) +
    '3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a' +
    '29ab5f49ffff001d1dac2b7c'
$hash = '6fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000'
$fixture = [ordered]@{
    schema = 1; captured = $true; driver = 'TEST'; reason = 'TEST'; timing = 'TEST'
    uptimeMs = 1; seed = 0; nonce = 0x7c2bac1d; hashes = 1
    startNonceSwapped = 0x1dac2b7c; nextNonceSwapped = 0x1dac2b7d
    candidate = $true; softwareReferenceOk = $true; rereadValid = $true
    hardwareHeader = $header; referenceHeader = $header
    hardwareHash = $hash; referenceHash = $hash; softwareHash = $hash; rereadHash = $hash
}
function Analyze-Fixture {
    $fixture | ConvertTo-Json | Set-Content -LiteralPath $temp -Encoding UTF8
    return (& $analyzer -Path $temp | ConvertFrom-Json)
}
try {
    $r = Analyze-Fixture
    if (!$r.hardwareMatchesPC -or !$r.automaticReferenceMatchesPC -or
        !$r.forcedSoftwareMatchesPC -or !$r.nonceMatchesHeader -or
        !$r.nonceAccountingMatches -or !$r.candidateFilterMatchesPC) {
        throw 'Genesis known-answer case failed.'
    }
    $fixture.hardwareHash = '00' * 32
    $r = Analyze-Fixture
    if ($r.classification -ne 'HARDWARE_OR_READBACK_MISMATCH' -or !$r.readbackChanged) {
        throw 'Hardware corruption case failed.'
    }
    $fixture.hardwareHash = $hash
    $fixture.referenceHash = '00' * 32
    $r = Analyze-Fixture
    if ($r.classification -ne 'AUTOMATIC_REFERENCE_MISMATCH') {
        throw 'Reference corruption case failed.'
    }
    $fixture.referenceHash = $hash
    $fixture.referenceHeader = '02000000' + $header.Substring(8)
    if ((Analyze-Fixture).classification -ne 'INPUT_MISMATCH') {
        throw 'Input mismatch case failed.'
    }
    $fixture.referenceHeader = $header
    $fixture.startNonceSwapped = 4294967295L
    $fixture.nextNonceSwapped = 0
    if (!(Analyze-Fixture).nonceAccountingMatches) { throw 'Nonce wrap case failed.' }
    $fixture.hardwareHash = 'invalid'
    $rejected = $false
    try { $null = Analyze-Fixture } catch { $rejected = $true }
    if (!$rejected) { throw 'Invalid data was accepted.' }
    Write-Output 'PASS: known hash, hardware corruption, reference corruption, input mismatch, nonce wrap, malformed capture.'
} finally {
    Remove-Item -LiteralPath $temp -ErrorAction SilentlyContinue
}
