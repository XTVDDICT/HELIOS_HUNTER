param(
    [Parameter(Mandatory = $true)]
    [string]$ElfPath,

    [string]$NmPath
)

$ErrorActionPreference = 'Stop'

$elf = (Resolve-Path -LiteralPath $ElfPath).Path

if (-not $NmPath) {
    $toolsRoot = Join-Path $env:LOCALAPPDATA 'Arduino15\packages\esp32\tools'
    $nm = Get-ChildItem -LiteralPath $toolsRoot -Filter 'xtensa-esp32-elf-nm.exe' -File -Recurse |
        Sort-Object LastWriteTime -Descending |
        Select-Object -First 1
    if (-not $nm) {
        throw 'xtensa-esp32-elf-nm.exe was not found. Supply its path with -NmPath.'
    }
    $NmPath = $nm.FullName
}

$output = & $NmPath --defined-only --print-size -C $elf
if ($LASTEXITCODE -ne 0) {
    throw "nm could not inspect $elf"
}

$line = $output |
    Where-Object { $_ -match 'runReferenceNativePipeline\(.*PipelineArgs&\)$' } |
    Select-Object -First 1

if (-not $line -or $line -notmatch '^\s*(?<address>[0-9a-fA-F]+)\s+(?<size>[0-9a-fA-F]+)\s+\S\s+') {
    throw 'The native SHA loop symbol was not found in the ELF.'
}

$address = [Convert]::ToUInt64($Matches.address, 16)
$size = [Convert]::ToUInt64($Matches.size, 16)
$expectedSize = 0x27c

if (($address % 32) -ne 0) {
    throw ('FAST SHA release gate failed: function address 0x{0:x} is not 32-byte aligned.' -f $address)
}

if ($size -ne $expectedSize) {
    throw ('FAST SHA release gate failed: function size is 0x{0:x}; expected 0x{1:x}.' -f $size, $expectedSize)
}

$requiredIramSymbols = @(
    'pipelined_hw_mine_classic_v2',
    'pipelined_hw_mine_classic_v2_margin',
    'nerd_sha256d_prefilter'
)
$iramSymbols = @{}
foreach ($symbol in $requiredIramSymbols) {
    $symbolLine = $output |
        Where-Object { $_ -match ('\s' + [regex]::Escape($symbol) + '$') } |
        Select-Object -First 1
    if (-not $symbolLine -or
        $symbolLine -notmatch '^\s*(?<address>[0-9a-fA-F]+)\s+(?<size>[0-9a-fA-F]+)\s+\S\s+') {
        throw "R22 SHA release gate failed: $symbol was not found in the ELF."
    }
    $symbolAddress = [Convert]::ToUInt64($Matches.address, 16)
    $symbolSize = [Convert]::ToUInt64($Matches.size, 16)
    if ($symbolAddress -lt 0x40080000 -or $symbolAddress -ge 0x400C0000) {
        throw ('R22 SHA release gate failed: {0} is not in ESP32 IRAM (address 0x{1:x}).' -f
            $symbol, $symbolAddress)
    }
    if ($symbolSize -eq 0) {
        throw "R22 SHA release gate failed: $symbol has zero size."
    }
    $iramSymbols[$symbol] = ('0x{0:x} / 0x{1:x} bytes' -f
        $symbolAddress, $symbolSize)
}

[pscustomobject]@{
    Result = 'PASS'
    Elf = $elf
    FunctionAddress = ('0x{0:x}' -f $address)
    FunctionSize = ('0x{0:x}' -f $size)
    CacheLineOffset = ($address % 32)
    UsefulAssemblyOffset = 17
    PipelinedV2Fast = $iramSymbols['pipelined_hw_mine_classic_v2']
    PipelinedV2Margin = $iramSymbols['pipelined_hw_mine_classic_v2_margin']
    SoftwarePrefilter = $iramSymbols['nerd_sha256d_prefilter']
}
