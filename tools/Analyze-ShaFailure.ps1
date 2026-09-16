param([Parameter(Mandatory)][string]$Path)

$ErrorActionPreference = 'Stop'
$capture = Get-Content -Raw -LiteralPath $Path | ConvertFrom-Json
if ($capture.schema -ne 1) { throw 'Unsupported capture schema.' }
if ($capture.captured -ne $true) { throw 'No failure has been captured yet.' }

function Read-Hex([string]$Hex, [int]$Length) {
    if ($Hex -notmatch ('\A[0-9a-fA-F]{' + ($Length * 2) + '}\z')) {
        throw "Expected $Length bytes of hexadecimal data."
    }
    $bytes = New-Object byte[] $Length
    for ($i = 0; $i -lt $Length; $i++) {
        $bytes[$i] = [Convert]::ToByte($Hex.Substring($i * 2, 2), 16)
    }
    return ,$bytes
}

function Double-Sha256([byte[]]$Bytes) {
    $sha = [Security.Cryptography.SHA256]::Create()
    try {
        $hash = $sha.ComputeHash($sha.ComputeHash($Bytes))
        return [BitConverter]::ToString($hash).Replace('-', '').ToLowerInvariant()
    } finally {
        $sha.Dispose()
    }
}

$hardwareHeader = Read-Hex $capture.hardwareHeader 80
$referenceHeader = Read-Hex $capture.referenceHeader 80
foreach ($field in @('hardwareHash', 'referenceHash', 'softwareHash', 'rereadHash')) {
    $null = Read-Hex $capture.$field 32
}
$hardwareExpected = Double-Sha256 $hardwareHeader
$referenceExpected = Double-Sha256 $referenceHeader
$inputsMatch = $capture.hardwareHeader -eq $capture.referenceHeader
$hardwareMatches = $capture.hardwareHash -eq $hardwareExpected
$referenceMatches = $capture.referenceHash -eq $referenceExpected
$softwareMatches = $null
if ($capture.softwareReferenceOk -eq $true) {
    $softwareMatches = $capture.softwareHash -eq $referenceExpected
}
$readbackChanged = $null
if ($capture.rereadValid -eq $true) {
    $readbackChanged = $capture.rereadHash -ne $capture.hardwareHash
}
$expectedNext = ([uint64]$capture.startNonceSwapped + [uint64]$capture.hashes) -band 0xffffffffL
$classification = if (!$inputsMatch) { 'INPUT_MISMATCH' }
    elseif (!$hardwareMatches -and !$referenceMatches) { 'BOTH_RESULTS_MISMATCH' }
    elseif (!$hardwareMatches) { 'HARDWARE_OR_READBACK_MISMATCH' }
    elseif (!$referenceMatches) { 'AUTOMATIC_REFERENCE_MISMATCH' }
    else { 'DIGESTS_MATCH_CHECK_FILTER_OR_ACCOUNTING' }

[pscustomobject][ordered]@{
    classification = $classification
    driver = $capture.driver
    reason = $capture.reason
    timing = $capture.timing
    uptimeMs = $capture.uptimeMs
    seed = $capture.seed
    nonce = $capture.nonce
    inputsMatch = $inputsMatch
    hardwareMatchesPC = $hardwareMatches
    automaticReferenceMatchesPC = $referenceMatches
    forcedSoftwareMatchesPC = $softwareMatches
    readbackChanged = $readbackChanged
    nonceMatchesHeader = [BitConverter]::ToUInt32($hardwareHeader, 76) -eq [uint32]$capture.nonce
    nonceAccountingMatches = $expectedNext -eq [uint64]$capture.nextNonceSwapped
    candidateFilterMatchesPC = [bool]$capture.candidate -eq $hardwareExpected.EndsWith('0000')
    expectedHardwareHash = $hardwareExpected
    expectedReferenceHash = $referenceExpected
} | ConvertTo-Json
