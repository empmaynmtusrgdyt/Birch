# Проверяет, что тёмная и светлая темы применяются ко всему окну Birch на
# Windows: запускает Birch.exe с --theme и --screenshot и сверяет яркость фона
# редактора на снимке (Birch пишет её в <снимок>.txt).
#
# Использование: check-theme.ps1 C:\BirchTest\bin\Birch.exe
param([Parameter(Mandatory)] [string] $Birch)
$ErrorActionPreference = "Stop"

$example = Join-Path $PWD "resources\examples\welcome.birch"
foreach ($theme in "dark", "light") {
    $shot = Join-Path $PWD "theme-$theme.png"
    # Birch.exe — GUI-приложение: ждём его завершения явно
    $p = Start-Process $Birch -Wait -PassThru `
           -ArgumentList "--theme", $theme, "--screenshot", "`"$shot`"", "`"$example`""
    if ($p.ExitCode -ne 0) { throw "Birch.exe --theme $theme завершился с кодом $($p.ExitCode)" }
    $info = Get-Content "$shot.txt"
    "${theme}: $($info -join ' ')"
    $lightness = [int](($info | Where-Object { $_ -like "lightness=*" }) -replace "lightness=", "")
    if ($theme -eq "dark" -and $lightness -ge 100) { throw "Тёмная тема не применилась к редактору (яркость фона $lightness)" }
    if ($theme -eq "light" -and $lightness -le 155) { throw "Светлая тема не применилась к редактору (яркость фона $lightness)" }
}
