# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Добавляет папку cmd с birch.com в PATH или убирает её оттуда.
#
#   birch-path.ps1                    добавить для текущего пользователя
#   birch-path.ps1 -Scope Machine     добавить для всех (нужны права администратора)
#   birch-path.ps1 -Remove [-Scope …] убрать
#
# Установщик вызывает его с -Scope Machine. Для .zip-версии достаточно запустить
# без параметров. Остальные записи PATH не меняются: значение читается без
# раскрытия переменных (%SystemRoot% остаётся как есть) и пишется того же типа.
param(
    [switch] $Remove,
    [ValidateSet('User', 'Machine')] [string] $Scope = 'User',
    [string] $Dir = (Join-Path $PSScriptRoot 'cmd')
)
$ErrorActionPreference = 'Stop'

if ($Scope -eq 'Machine') {
    $key = [Microsoft.Win32.Registry]::LocalMachine.OpenSubKey(
        'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', $true)
} else {
    $key = [Microsoft.Win32.Registry]::CurrentUser.CreateSubKey('Environment')
}

$dirFull = [IO.Path]::GetFullPath($Dir).TrimEnd('\')
$hasPath = $key.GetValueNames() -contains 'Path'
$old = if ($hasPath) { [string] $key.GetValue('Path', '', 'DoNotExpandEnvironmentNames') } else { '' }
$kind = if ($hasPath) { $key.GetValueKind('Path') } else { [Microsoft.Win32.RegistryValueKind]::ExpandString }

$entries = @($old -split ';')
$isOurs = { param($e) $e.Trim().TrimEnd('\') -eq $dirFull }  # -eq без учёта регистра

if ($Remove) {
    $new = (@($entries | Where-Object { -not (& $isOurs $_) })) -join ';'
} elseif (@($entries | Where-Object { & $isOurs $_ }).Count -gt 0) {
    $new = $old
} else {
    $base = $old.TrimEnd(';')
    $new = if ($base -eq '') { $dirFull } else { "$base;$dirFull" }
}

if ($new -ne $old) {
    $key.SetValue('Path', $new, $kind)
    # Сообщаем Проводнику, что PATH изменился: новые окна консоли увидят его сразу.
    try {
        Add-Type -Namespace BirchPath -Name Win32 -MemberDefinition @'
[DllImport("user32.dll", CharSet = CharSet.Unicode)]
public static extern IntPtr SendMessageTimeout(IntPtr hWnd, uint msg, UIntPtr wParam,
    string lParam, uint flags, uint timeout, out UIntPtr result);
'@
        $result = [UIntPtr]::Zero
        [void] [BirchPath.Win32]::SendMessageTimeout([IntPtr] 0xffff, 0x1A, [UIntPtr]::Zero,
            'Environment', 2, 5000, [ref] $result)
    } catch { }
}
$key.Close()

$action = if ($Remove) { 'убрана из' } else { 'добавлена в' }
Write-Output "Папка $dirFull $action PATH ($Scope)."
