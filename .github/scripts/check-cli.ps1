# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Проверяет командную строку Birch на Windows: справку и версию, коды выхода,
# рендер в PNG, SVG и Mermaid. Birch.exe — оконное приложение, поэтому вывод
# перенаправляется в файлы (так же его получают скрипты).
#
# Использование: check-cli.ps1 C:\BirchTest\bin\Birch.exe
#                check-cli.ps1 C:\BirchTest\cmd\birch.com
param([Parameter(Mandatory)] [string] $Birch)
$ErrorActionPreference = "Stop"
# Основные проверки — по-русски; английский проверяется отдельно в конце
$env:BIRCH_LANGUAGE = "ru"

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$version = (Select-String -Path "$root\CMakeLists.txt" -Pattern '^\s*VERSION ([0-9.]+)$').Matches[0].Groups[1].Value
$example = "$root\resources\examples\welcome.birch"
$frames = "$root\resources\examples\frames.birch"
$work = Join-Path $env:RUNNER_TEMP "birch-cli"
Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory $work | Out-Null
$failed = $false
function Fail([string] $message) { Write-Host "::error::$message"; $script:failed = $true }

# Запуск: возвращает код выхода; вывод — в $work\out.txt и $work\err.txt (UTF-8).
function Run([string[]] $arguments) {
    $quoted = $arguments | ForEach-Object { if ($_ -match '[\s"]') { '"' + $_ + '"' } else { $_ } }
    $p = Start-Process $Birch -ArgumentList $quoted -Wait -PassThru -NoNewWindow `
           -RedirectStandardOutput "$work\out.txt" -RedirectStandardError "$work\err.txt"
    return $p.ExitCode
}
function Out { [IO.File]::ReadAllText("$work\out.txt", [Text.Encoding]::UTF8) }
function Err { [IO.File]::ReadAllText("$work\err.txt", [Text.Encoding]::UTF8) }
function Expect([int] $code, [int] $expected, [string] $what) {
    if ($code -ne $expected) { Fail "${what}: код $code, ожидался $expected. stderr: $(Err)" }
}

# --- справка и версия ---------------------------------------------------------
Expect (Run @("--help")) 0 "--help"
$help = Out
# Имя — как вызвали: Birch.exe напрямую или birch через birch.com
if (-not ($help -cmatch '^Использование: [Bb]irch ')) { Fail "--help: $($help.Split("`n")[0])" }
Expect (Run @("-h")) 0 "-h"
if ((Out) -ne $help) { Fail "-h и --help выводят разное" }

Expect (Run @("--version")) 0 "--version"
$ver = Out
$first = $ver.Split("`n")[0].TrimEnd("`r")
if ($first -ne "birch (Birch) $version") { Fail "--version: первая строка — $first" }
if ($ver -notmatch "Marth Scoobert") { Fail "--version: нет автора" }
Expect (Run @("-v")) 0 "-v"
if ((Out) -ne $ver) { Fail "-v и --version выводят разное" }
Write-Host "$first — $(($ver.Trim() -split "`n")[-1])"

# --- неверные ключи -----------------------------------------------------------
Expect (Run @("--colour")) 3 "--colour"
if ((Err) -notmatch "--help") { Fail "--colour: нет подсказки про --help" }
Expect (Run @("-t", "dark", $example)) 3 "-t без -o"

# --- рендер -------------------------------------------------------------------
Expect (Run @($frames, "-o", "$work\a.png")) 0 "рендер PNG"
$bytes = [IO.File]::ReadAllBytes("$work\a.png")
if ($bytes[1] -ne 0x50 -or $bytes[2] -ne 0x4E -or $bytes[3] -ne 0x47) { Fail "a.png — не PNG" }
if ((Err).Length -gt 0) { Fail "рендер PNG: лишний вывод в stderr: $(Err)" }

Expect (Run @($frames, "-o", "$work\a.svg")) 0 "рендер SVG"
if (-not (Select-String -Path "$work\a.svg" -Pattern "<svg" -Quiet)) { Fail "a.svg — не SVG" }

Expect (Run @($frames, "-o", "$work\a.mmd", "-H")) 0 "рендер Mermaid"
if ((Get-Content "$work\a.mmd" -TotalCount 1) -ne "flowchart LR") { Fail "a.mmd: неверная первая строка" }

Expect (Run @($example, "-t", "dark", "-ma", "-s", "1", "-o", "$work\short.png")) 0 "короткие ключи"
Expect (Run @($example, "--theme=dark", "--monochrome", "--transparent", "--scale=1", "--output=$work\long.png")) 0 "длинные ключи"
if ((Get-FileHash "$work\short.png").Hash -ne (Get-FileHash "$work\long.png").Hash) {
    Fail "короткие и длинные ключи дали разные картинки"
}

# --- ошибки в схеме и файлах ---------------------------------------------------
[IO.File]::WriteAllText("$work\bad.birch", "[A] -> [B`n")
Expect (Run @("$work\bad.birch", "-o", "$work\bad.png")) 1 "ошибка в схеме"
if ((Err) -notmatch "bad\.birch:1:8: ошибка: ") { Fail "ошибка в схеме: $(Err)" }
if (Test-Path "$work\bad.png") { Fail "при ошибке в схеме файл не должен появляться" }

Expect (Run @("$work\нет такого.birch", "-o", "$work\x.png")) 2 "нет входного файла"

# --- английский --------------------------------------------------------------
$env:BIRCH_LANGUAGE = "en"
Expect (Run @("--help")) 0 "--help по-английски"
if (-not ((Out) -cmatch '^Usage: [Bb]irch ')) { Fail "--help по-английски: $((Out).Split("`n")[0])" }
Expect (Run @("--colour")) 3 "--colour по-английски"
if ((Err) -notmatch "unrecognized option '--colour'") { Fail "--colour по-английски: $(Err)" }
Expect (Run @("$work\bad.birch", "-o", "$work\bad.png")) 1 "ошибка в схеме по-английски"
if ((Err) -notmatch "bad\.birch:1:8: error: Unclosed") { Fail "ошибка в схеме по-английски: $(Err)" }
$env:BIRCH_LANGUAGE = "ru"

if ($failed) { exit 1 }
Write-Host "Командная строка: все проверки пройдены (по-русски и по-английски)"
exit 0
