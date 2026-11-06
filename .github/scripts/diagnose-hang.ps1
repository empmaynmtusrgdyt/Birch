# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Запускает Birch.exe в нескольких режимах с ограничением по времени. Если
# программа зависла, печатает стеки всех её потоков (отладчик cdb из Windows
# SDK) и завершает её. Нужен, чтобы понять, где именно зависает сборка.
#
# Использование: diagnose-hang.ps1 C:\путь\к\Birch.exe
param([Parameter(Mandatory)] [string] $Birch)
$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$example = "$root\resources\examples\welcome.birch"
$work = Join-Path $env:RUNNER_TEMP "birch-hang"
New-Item -ItemType Directory $work -Force | Out-Null
$cdb = Get-ChildItem "${env:ProgramFiles(x86)}\Windows Kits\10\Debuggers\x64\cdb.exe" -ErrorAction SilentlyContinue |
       Select-Object -First 1
$hung = $false

function Try-Run([string] $name, [string[]] $arguments, [string] $platform) {
    if ($platform) { $env:QT_QPA_PLATFORM = $platform } else { Remove-Item Env:QT_QPA_PLATFORM -ErrorAction SilentlyContinue }
    $quoted = $arguments | ForEach-Object { if ($_ -match '\s') { "`"$_`"" } else { $_ } }
    $p = Start-Process $Birch -ArgumentList $quoted -PassThru -NoNewWindow `
           -RedirectStandardOutput "$work\$name.out" -RedirectStandardError "$work\$name.err"
    if ($p.WaitForExit(60000)) {
        Write-Host "OK   $name — код $($p.ExitCode)"
        return
    }
    Write-Host "::error::ЗАВИС $name (60 с)"
    $script:hung = $true
    if ($cdb) {
        $sym = "srv*$env:RUNNER_TEMP\symbols*https://msdl.microsoft.com/download/symbols"
        & $cdb.FullName -p $p.Id -y $sym -c ".lines -d; ~*kn 40; qd" 2>&1 |
            Select-String -NotMatch "^(Symbol search path|Executable search path|\*\*\*|ModLoad)" |
            ForEach-Object { $_.Line }
    } else {
        Write-Host "cdb не найден — стеков не будет"
    }
    Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue
    Get-Content "$work\$name.err" -ErrorAction SilentlyContinue | Select-Object -First 20
}

Try-Run "version" @("--version") ""
Try-Run "render-cli" @($example, "-o", "$work\cli.png") ""
Try-Run "window-offscreen" @("--screenshot", "$work\offscreen.png", $example) "offscreen"
Try-Run "window" @("--screenshot", "$work\window.png", $example) ""

if ($hung) { exit 1 }
