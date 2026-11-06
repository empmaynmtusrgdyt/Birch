# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Проверяет, что в настоящем окне консоли кириллица из birch видна правильно.
# Обычные проверки перенаправляют вывод в файлы и этот путь не задевают.
#
# Запускает программу с выводом прямо в консоль (CONOUT$), затем читает символы
# с экрана консоли (ReadConsoleOutputCharacterW) и ищет ожидаемый текст.
#
# Использование: check-console-text.ps1 C:\BirchTest
param([Parameter(Mandatory)] [string] $InstallDir)
$ErrorActionPreference = "Stop"

Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
using System.Text;

public static class BirchConsole {
    [StructLayout(LayoutKind.Sequential)]
    struct SECURITY_ATTRIBUTES { public int nLength; public IntPtr lpSecurityDescriptor; public int bInheritHandle; }
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    struct STARTUPINFO {
        public int cb; public string lpReserved, lpDesktop, lpTitle;
        public int dwX, dwY, dwXSize, dwYSize, dwXCountChars, dwYCountChars, dwFillAttribute, dwFlags;
        public short wShowWindow, cbReserved2; public IntPtr lpReserved2, hStdInput, hStdOutput, hStdError;
    }
    [StructLayout(LayoutKind.Sequential)]
    struct PROCESS_INFORMATION { public IntPtr hProcess, hThread; public int dwProcessId, dwThreadId; }
    [StructLayout(LayoutKind.Sequential)]
    struct COORD { public short X, Y; }
    [StructLayout(LayoutKind.Sequential)]
    struct CSBI { public COORD size, cursor; public short attributes, left, top, right, bottom; public COORD max; }

    [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    static extern IntPtr CreateFileW(string name, uint access, uint share, ref SECURITY_ATTRIBUTES sa,
                                     uint disposition, uint flags, IntPtr template);
    [DllImport("kernel32.dll")] static extern bool AllocConsole();
    [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    static extern bool CreateProcessW(string app, StringBuilder cmd, IntPtr pa, IntPtr ta, bool inherit,
                                      uint flags, IntPtr env, string dir, ref STARTUPINFO si,
                                      out PROCESS_INFORMATION pi);
    [DllImport("kernel32.dll")] static extern uint WaitForSingleObject(IntPtr h, uint ms);
    [DllImport("kernel32.dll")] static extern bool TerminateProcess(IntPtr h, uint code);
    [DllImport("kernel32.dll")] static extern bool GetExitCodeProcess(IntPtr h, out uint code);
    [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr h);
    [DllImport("kernel32.dll")] static extern bool GetConsoleScreenBufferInfo(IntPtr h, out CSBI info);
    [DllImport("kernel32.dll")] static extern bool FillConsoleOutputCharacterW(IntPtr h, char c, uint n, COORD at, out uint written);
    [DllImport("kernel32.dll")] static extern bool SetConsoleCursorPosition(IntPtr h, COORD at);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
    static extern bool ReadConsoleOutputCharacterW(IntPtr h, StringBuilder buf, uint n, COORD at, out uint read);

    static readonly IntPtr Invalid = new IntPtr(-1);

    static IntPtr OpenConsole() {
        var sa = new SECURITY_ATTRIBUTES { nLength = Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES)), bInheritHandle = 1 };
        // GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING
        return CreateFileW("CONOUT$", 0xC0000000, 3, ref sa, 3, 0, IntPtr.Zero);
    }

    // Запускает программу с stdout и stderr прямо в консоль и возвращает текст с экрана.
    public static string Run(string exe, string args, out uint code) {
        IntPtr con = OpenConsole();
        if (con == Invalid) { AllocConsole(); con = OpenConsole(); }
        if (con == Invalid) throw new Exception("нет консоли: " + Marshal.GetLastWin32Error());

        CSBI info;
        GetConsoleScreenBufferInfo(con, out info);
        uint written;
        FillConsoleOutputCharacterW(con, ' ', (uint)(info.size.X * info.size.Y), new COORD(), out written);
        SetConsoleCursorPosition(con, new COORD());

        var si = new STARTUPINFO { cb = Marshal.SizeOf(typeof(STARTUPINFO)), dwFlags = 0x100, // STARTF_USESTDHANDLES
                                   hStdOutput = con, hStdError = con };
        PROCESS_INFORMATION pi;
        var cmd = new StringBuilder("\"" + exe + "\" " + args);
        if (!CreateProcessW(exe, cmd, IntPtr.Zero, IntPtr.Zero, true, 0, IntPtr.Zero, null, ref si, out pi))
            throw new Exception("не удалось запустить " + exe + ": " + Marshal.GetLastWin32Error());
        if (WaitForSingleObject(pi.hProcess, 60000) != 0) TerminateProcess(pi.hProcess, 99);
        GetExitCodeProcess(pi.hProcess, out code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        var text = new StringBuilder();
        for (short y = 0; y < 40 && y < info.size.Y; y++) {
            var line = new StringBuilder(info.size.X);
            uint read;
            ReadConsoleOutputCharacterW(con, line, (uint)info.size.X, new COORD { X = 0, Y = y }, out read);
            text.AppendLine(line.ToString(0, (int)read).TrimEnd());
        }
        CloseHandle(con);
        return text.ToString().TrimEnd();
    }
}
'@

$failed = $false
function Check([string] $exe, [string] $arguments, [string[]] $expected, [int] $expectedCode) {
    $code = [uint32] 0
    $screen = [BirchConsole]::Run($exe, $arguments, [ref] $code)
    $name = "$(Split-Path $exe -Leaf) $arguments"
    if ($code -ne $expectedCode) { Write-Host "::error::$name — код $code, ожидался $expectedCode"; $script:failed = $true }
    foreach ($e in $expected) {
        if (-not $screen.Contains($e)) {
            Write-Host "::error::$name — в консоли нет «$e». На экране:"
            Write-Host $screen
            $script:failed = $true
            return
        }
    }
    Write-Host "OK   $name"
}

$com = Join-Path $InstallDir "cmd\birch.com"
$exe = Join-Path $InstallDir "bin\Birch.exe"
foreach ($program in $com, $exe) {
    Check $program "--version" @("Лицензия MPL 2.0", "Это свободное ПО") 0
    Check $program "--colour" @("неизвестный ключ «--colour»", "Попробуйте") 3
}

if ($failed) { exit 1 }
Write-Host "Кириллица в консоли видна правильно"
