# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Снимки окна на каждом языке интерфейса (по файлам translations\birch_*.ts):
# панель поиска — с проверкой, что она помещается в окно, — и пример с рамками
# в светлой, тёмной и чёрно-белой теме. Снимки — в текущей папке.
#
# Использование: screenshots.ps1 C:\BirchTest\bin\Birch.exe windows
param([Parameter(Mandatory)] [string] $Exe, [Parameter(Mandatory)] [string] $System)
$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$examples = Join-Path $root "resources\examples"
$failed = $false

function Shot([string] $name, [string] $file, [string[]] $options) {
    $shot = Join-Path $PWD $name
    # Birch.exe — приложение с окном: ждём его завершения явно
    $argList = $options + @("--screenshot", "`"$shot`"", "`"$examples\$file`"")
    $p = Start-Process $Exe -Wait -PassThru -ArgumentList $argList
    if ($p.ExitCode -ne 0) { throw "Birch.exe ($name) завершился с кодом $($p.ExitCode)" }
    return $shot
}

foreach ($ts in Get-ChildItem (Join-Path $root "translations") -Filter "birch_*.ts") {
    $lang = $ts.BaseName.Substring(6)
    $suffix = if ($lang -eq "ru") { "" } else { ".$lang" }  # примеры на исходном языке — без суффикса
    $env:BIRCH_LANGUAGE = $lang

    $shot = Shot "find-$System-$lang.png" "welcome$suffix.birch" @("--find=->")
    if (-not (Select-String -Path "$shot.txt" -Pattern "^find_bar_fits=1" -Quiet)) {
        Write-Host "::error::Панель поиска не помещается в окно ($lang)"
        $failed = $true
    }
    Shot "frames-$lang-light.png" "frames$suffix.birch" @("--screenshot-theme", "light") | Out-Null
    Shot "frames-$lang-dark.png" "frames$suffix.birch" @("--screenshot-theme", "dark") | Out-Null
    Shot "frames-$lang-mono.png" "frames$suffix.birch" @("--screenshot-theme", "light", "--screenshot-monochrome") | Out-Null
    Write-Host "Снимки: $lang"
}
if ($failed) { exit 1 }
