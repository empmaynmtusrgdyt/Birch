# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Проверяет команду birch после установки на Windows: папка cmd в PATH,
# birch.com ждёт Birch.exe и возвращает его вывод и код выхода, а с окном —
# сразу отпускает консоль. Затем полная проверка командной строки через birch.com.
#
# Использование: check-console.ps1 C:\BirchTest
param([Parameter(Mandatory)] [string] $InstallDir)
$ErrorActionPreference = "Stop"
$env:BIRCH_LANGUAGE = "ru"  # проверяется русский текст; английский — в check-cli.ps1
[Console]::OutputEncoding = [Text.Encoding]::UTF8

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$example = "$root\resources\examples\welcome.birch"
$work = Join-Path $env:RUNNER_TEMP "birch-console"
Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory $work | Out-Null
$failed = $false
function Fail([string] $message) { Write-Host "::error::$message"; $script:failed = $true }

# --- PATH ---------------------------------------------------------------------
$cmdDir = Join-Path $InstallDir "cmd"
$machinePath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$entries = $machinePath -split ";" | ForEach-Object { $_.TrimEnd("\") }
if ($entries -notcontains $cmdDir) { Fail "папки $cmdDir нет в PATH" }
if ($entries -contains (Join-Path $InstallDir "bin")) { Fail "в PATH попала папка bin с DLL" }

# Как в новом окне консоли: PATH из реестра, без окружения сборки.
$env:Path = "$machinePath;" + [Environment]::GetEnvironmentVariable("Path", "User")
Remove-Item Env:QT_QPA_PLATFORM -ErrorAction SilentlyContinue
$found = (Get-Command birch -ErrorAction SilentlyContinue).Source
if ($found -ne (Join-Path $cmdDir "birch.com")) { Fail "команда birch находит «$found», а не birch.com" }

# --- PowerShell: вывод и код выхода -------------------------------------------
$out = & birch --version
if ($LASTEXITCODE -ne 0) { Fail "birch --version: код $LASTEXITCODE" }
if (-not ($out[0] -match '^birch \(Birch\) ')) { Fail "birch --version: $($out[0])" }
Write-Host "PowerShell: $($out[0])"

$help = & birch --help
if (-not ($help[0] -match '^Использование: birch ')) { Fail "birch --help: $($help[0])" }

$err = & birch --colour 2>&1 | Out-String
if ($LASTEXITCODE -ne 3) { Fail "birch --colour: код $LASTEXITCODE, ожидался 3" }
if ($err -notmatch "неизвестный ключ") { Fail "birch --colour: stderr — $err" }

# Консоль ждёт рендер: файл уже есть, как только команда вернулась.
& birch $example -o "$work\a.png"
if ($LASTEXITCODE -ne 0) { Fail "birch -o: код $LASTEXITCODE" }
if (-not (Test-Path "$work\a.png")) { Fail "birch -o вернулся раньше, чем появился файл" }

[IO.File]::WriteAllText("$work\bad.birch", "[A] -> [B`n")
$err = & birch "$work\bad.birch" -o "$work\bad.png" 2>&1 | Out-String
if ($LASTEXITCODE -ne 1) { Fail "ошибка в схеме: код $LASTEXITCODE, ожидался 1" }
if ($err -notmatch "bad\.birch:1:8: ошибка: ") { Fail "ошибка в схеме: stderr — $err" }

# --- cmd ----------------------------------------------------------------------
cmd /c "birch `"$example`" -o `"$work\cmd.svg`""
if ($LASTEXITCODE -ne 0) { Fail "cmd: код $LASTEXITCODE" }
if (-not (Test-Path "$work\cmd.svg")) { Fail "cmd: нет cmd.svg" }
cmd /c "birch --theme=blue -o `"$work\x.png`" `"$example`" 2>nul"
if ($LASTEXITCODE -ne 3) { Fail "cmd: --theme=blue — код $LASTEXITCODE, ожидался 3" }
Write-Host "cmd: коды выхода доходят"

# --- окно: консоль сразу свободна ---------------------------------------------
Get-Process Birch -ErrorAction SilentlyContinue | Stop-Process -Force
$launcher = Start-Process (Join-Path $cmdDir "birch.com") -ArgumentList "`"$example`"" -PassThru
if (-not $launcher.WaitForExit(30000)) {
    Fail "birch.com не отпустил консоль, когда открылось окно"
    $launcher.Kill()
} elseif ($launcher.ExitCode -ne 0) {
    Fail "birch.com с окном: код $($launcher.ExitCode)"
} elseif (-not (Get-Process Birch -ErrorAction SilentlyContinue)) {
    Fail "окно Birch закрылось вместе с birch.com"
} else {
    Write-Host "Окно: консоль свободна, Birch открыт"
}
Get-Process Birch -ErrorAction SilentlyContinue | Stop-Process -Force

if ($failed) { exit 1 }

# --- все проверки командной строки — через birch.com --------------------------
& "$PSScriptRoot\check-cli.ps1" (Join-Path $cmdDir "birch.com")
exit $LASTEXITCODE
