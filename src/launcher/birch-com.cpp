/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// birch.com — консольная обёртка над оконным Birch.exe для командной строки Windows.
//
// Birch.exe собран как оконное приложение, поэтому cmd и PowerShell не ждут его
// завершения и теряют код выхода. birch.com — консольная программа: консоль ждёт
// её, а она запускает Birch.exe с теми же аргументами, ждёт и возвращает его код.
// Набранное «birch» находит именно birch.com: в PATHEXT .COM стоит раньше .EXE.
//
// Без Qt и с CRT внутри: лежит в папке cmd отдельно от DLL программы, чтобы в
// PATH попадала только эта папка, а не Qt и Graphviz.

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <cstdint>

#include <string>

namespace {

std::wstring modulePath() {
    std::wstring path(MAX_PATH, L'\0');
    for (;;) {
        const DWORD n = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
        if (n == 0) return {};
        if (n < path.size()) {
            path.resize(n);
            return path;
        }
        path.resize(path.size() * 2);
    }
}

std::wstring directoryOf(const std::wstring& path) {
    const auto i = path.find_last_of(L"\\/");
    return i == std::wstring::npos ? std::wstring(L".") : path.substr(0, i);
}

bool isFile(const std::wstring& path) {
    const DWORD attrs = GetFileAttributesW(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

bool isConsole(HANDLE h) {
    DWORD mode = 0;
    return GetConsoleMode(h, &mode) != 0;
}

// Файл или канал передаём Birch.exe как есть. Консоль — нет: Birch.exe сам
// подключится к ней (AttachConsole) и будет писать в неё через WriteConsoleW.
HANDLE passThrough(DWORD id) {
    const HANDLE h = GetStdHandle(id);
    if (h == nullptr || h == INVALID_HANDLE_VALUE || isConsole(h)) return nullptr;
    SetHandleInformation(h, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    return h;
}

void printError(const std::wstring& text) {
    const HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
    DWORD written = 0;
    if (isConsole(h)) {
        WriteConsoleW(h, text.c_str(), static_cast<DWORD>(text.size()), &written, nullptr);
        return;
    }
    const int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()),
                                         nullptr, 0, nullptr, nullptr);
    std::string utf8(static_cast<size_t>(size), '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), utf8.data(), size,
                        nullptr, nullptr);
    WriteFile(h, utf8.data(), static_cast<DWORD>(utf8.size()), &written, nullptr);
}

}  // namespace

int wmain() {
    // В установленной программе birch.com лежит в cmd\, а Birch.exe — в bin\.
    const std::wstring dir = directoryOf(modulePath());
    std::wstring exe = dir + L"\\Birch.exe";
    if (!isFile(exe)) exe = dir + L"\\..\\bin\\Birch.exe";
    if (!isFile(exe)) {
        printError(L"birch: не найден Birch.exe (искал рядом с birch.com и в ..\\bin)\n");
        return 2;
    }

    // Когда Birch.exe открывает окно, ждать его не нужно: он даёт знать через это
    // событие, и консоль сразу свободна — как при запуске из меню «Пуск».
    SECURITY_ATTRIBUTES inherit{static_cast<DWORD>(sizeof(inherit)), nullptr, TRUE};
    const HANDLE windowOpened = CreateEventW(&inherit, TRUE, FALSE, nullptr);
    if (windowOpened != nullptr) {
        SetEnvironmentVariableW(L"BIRCH_WINDOW_EVENT",
                                std::to_wstring(reinterpret_cast<uintptr_t>(windowOpened)).c_str());
    }

    STARTUPINFOW si{};
    si.cb = static_cast<DWORD>(sizeof(si));
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = passThrough(STD_INPUT_HANDLE);
    si.hStdOutput = passThrough(STD_OUTPUT_HANDLE);
    si.hStdError = passThrough(STD_ERROR_HANDLE);

    // Командная строка — как есть, вместе с именем, которым программу вызвали:
    // по нему Birch пишет «Использование: birch …» в справке.
    std::wstring commandLine = GetCommandLineW();
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(exe.c_str(), commandLine.data(), nullptr, nullptr, TRUE, 0, nullptr,
                        nullptr, &si, &pi)) {
        printError(L"birch: не удалось запустить " + exe + L" (код " +
                   std::to_wstring(GetLastError()) + L")\n");
        return 2;
    }
    CloseHandle(pi.hThread);

    const HANDLE waitFor[] = {pi.hProcess, windowOpened};
    const DWORD count = windowOpened != nullptr ? 2 : 1;
    const DWORD which = WaitForMultipleObjects(count, waitFor, FALSE, INFINITE);

    DWORD code = 0;
    if (which == WAIT_OBJECT_0) GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    if (windowOpened != nullptr) CloseHandle(windowOpened);
    return static_cast<int>(code);
}
