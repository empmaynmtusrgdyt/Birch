# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

"""Проверяет, что пакет Birch для Windows 8.1 там запустится.

Читает заголовки и таблицы импорта всех .exe, .com и .dll пакета (без
сторонних библиотек: формат PE разбирается здесь же) и проверяет:

  * каждая нужная DLL лежит в пакете или есть в Windows 8.1;
  * каждая функция, которую файл берёт из DLL пакета, в этой DLL есть —
    так ловится несовместимость подменённого Qt со сборкой Birch;
  * ни одна функция не появилась только в Windows 10 — такой импорт не даёт
    программе даже запуститься («точка входа не найдена»);
  * версия подсистемы в заголовке .exe и .com не новее Windows 8.1 (6.3);
  * с --qt-version: библиотеки Qt ровно этой версии (подменённые и
    оставшиеся должны совпадать, иначе программа упадёт).

Список функций Windows 10 составлен вручную по документации Microsoft
(раздел Requirements у каждой функции) — он ловит самые частые причины,
но не все. Окончательная проверка — запуск на Windows 8.1.

Использование: check-win81.py ПАПКА_ПАКЕТА [--qt-version 6.8.3] [--list]
"""

import argparse
import struct
import sys
from pathlib import Path

# DLL, которые есть в Windows 8.1 (64-бит). Остальные должны лежать в пакете.
WIN81_DLLS = {
    "advapi32", "authz", "bcrypt", "cfgmgr32", "combase", "comctl32", "comdlg32",
    "crypt32", "d2d1", "d3d11", "d3d9", "d3dcompiler_47", "dbghelp", "dnsapi",
    "dwmapi", "dwrite", "dxgi", "gdi32", "glu32", "hid", "imm32", "iphlpapi",
    "imagehlp", "kernel32", "mpr", "msimg32", "msvcrt", "ncrypt", "netapi32", "normaliz", "ntdll",
    "ole32", "oleacc", "oleaut32", "opengl32", "powrprof", "propsys", "psapi",
    "rpcrt4", "secur32", "setupapi", "shcore", "shell32", "shlwapi",
    "uiautomationcore", "user32", "userenv", "usp10", "uxtheme", "version",
    "winhttp", "wininet", "winmm", "winspool.drv", "wldap32", "ws2_32",
    "wtsapi32",
    # Наборы API, которые есть в Windows 8 и 8.1
    "api-ms-win-core-synch-l1-2-0", "api-ms-win-core-winrt-l1-1-0",
    "api-ms-win-core-winrt-string-l1-1-0", "api-ms-win-core-winrt-error-l1-1-0",
    "api-ms-win-core-path-l1-1-0", "api-ms-win-core-libraryloader-l1-2-0",
    "api-ms-win-shcore-scaling-l1-1-1",
}

# Универсальная среда выполнения C (UCRT) в Windows 8.1 есть только с
# обновлением KB2999226 — поэтому она должна лежать в пакете.
UCRT_PREFIX = "api-ms-win-crt-"

# Функции, которых нет в Windows 8.1 (появились в Windows 10).
WIN10_ONLY = {
    "user32": {
        "AdjustWindowRectExForDpi", "AreDpiAwarenessContextsEqual",
        "EnableNonClientDpiScaling", "GetAwarenessFromDpiAwarenessContext",
        "GetDpiForSystem", "GetDpiForWindow", "GetDpiFromDpiAwarenessContext",
        "GetSystemDpiForProcess", "GetSystemMetricsForDpi",
        "GetThreadDpiAwarenessContext", "GetWindowDpiAwarenessContext",
        "IsValidDpiAwarenessContext", "SetProcessDpiAwarenessContext",
        "SetThreadDpiAwarenessContext", "SystemParametersInfoForDpi",
        "GetPointerDevices", "GetDialogDpiChangeBehavior",
    },
    "kernel32": {
        "SetThreadDescription", "GetThreadDescription", "VirtualAlloc2",
        "MapViewOfFile3", "GetSystemCpuSetInformation", "GetIntegratedDisplaySize",
        "SetThreadSelectedCpuSets", "SetProcessDefaultCpuSets",
    },
    "dxgi": {"DXGIGetDebugInterface1", "DXGIDeclareAdapterRemovalSupport"},
}
# DLL, которых нет в Windows 8.1 вовсе
WIN10_ONLY_DLLS = {"d3d12", "dxcompiler", "windows.storage", "dcomp_win10"}

MAX_SUBSYSTEM = (6, 3)  # Windows 8.1

# Папки плагинов Qt: их версия тоже должна совпадать с Qt
QT_PLUGIN_DIRS = {"platforms", "styles", "imageformats", "iconengines", "generic",
                  "tls", "networkinformation"}


class PE:
    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError("не PE")
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        if d[pe:pe + 4] != b"PE\0\0":
            raise ValueError("не PE")
        nsec, = struct.unpack_from("<H", d, pe + 6)
        optsize, = struct.unpack_from("<H", d, pe + 20)
        opt = pe + 24
        magic, = struct.unpack_from("<H", d, opt)
        self.is64 = magic == 0x20B
        self.subsystem_version = struct.unpack_from("<HH", d, opt + 48)
        dirs = opt + (112 if self.is64 else 96)
        self.dirs = [struct.unpack_from("<II", d, dirs + 8 * i) for i in range(16)]
        sec = opt + optsize
        self.sections = []
        for i in range(nsec):
            va_size, va, raw_size, raw = struct.unpack_from("<IIII", d, sec + 40 * i + 8)
            self.sections.append((va, max(va_size, raw_size), raw))

    def off(self, rva: int) -> int:
        for va, size, raw in self.sections:
            if va <= rva < va + size:
                return rva - va + raw
        raise ValueError(f"RVA {rva:#x} вне секций")

    def cstr(self, rva: int) -> str:
        o = self.off(rva)
        return self.data[o:self.data.index(b"\0", o)].decode("ascii", "replace")

    def _names(self, thunk_rva: int):
        size = 8 if self.is64 else 4
        flag = 1 << (63 if self.is64 else 31)
        o = self.off(thunk_rva)
        while True:
            v = int.from_bytes(self.data[o:o + size], "little")
            if v == 0:
                return
            yield f"#{v & 0xFFFF}" if v & flag else self.cstr((v & 0x7FFFFFFF) + 2)
            o += size

    def imports(self):
        """[(dll, [функции], отложенный?)]"""
        result = []
        rva, size = self.dirs[1]
        if rva:
            o = self.off(rva)
            while True:
                oft, _, _, name, ft = struct.unpack_from("<IIIII", self.data, o)
                if name == 0:
                    break
                result.append((self.cstr(name), list(self._names(oft or ft)), False))
                o += 20
        rva, size = self.dirs[13]
        if rva:
            o = self.off(rva)
            while True:
                attrs, name, _, iat, int_ = struct.unpack_from("<IIIII", self.data, o)
                if name == 0:
                    break
                result.append((self.cstr(name), list(self._names(int_)), True))
                o += 32
        return result

    def exports(self):
        """Имена экспортируемых функций."""
        rva, size = self.dirs[0]
        if not rva:
            return set()
        o = self.off(rva)
        n_names, = struct.unpack_from("<I", self.data, o + 24)
        names_rva, = struct.unpack_from("<I", self.data, o + 32)
        no = self.off(names_rva)
        return {self.cstr(struct.unpack_from("<I", self.data, no + 4 * i)[0]) for i in range(n_names)}

    def file_version(self):
        i = self.data.find(struct.pack("<I", 0xFEEF04BD))
        if i < 0:
            return None
        ms, ls = struct.unpack_from("<II", self.data, i + 8)
        return f"{ms >> 16}.{ms & 0xFFFF}.{ls >> 16}"


def base(name: str) -> str:
    name = name.lower()
    return name[:-4] if name.endswith(".dll") else name


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("package", type=Path)
    ap.add_argument("--qt-version")
    ap.add_argument("--list", action="store_true", help="напечатать импорты каждого файла")
    args = ap.parse_args()

    files = sorted(p for p in args.package.rglob("*")
                   if p.suffix.lower() in (".exe", ".com", ".dll") and p.is_file())
    in_package = {p.name.lower() for p in files}
    problems = []

    pes = {}
    exports = {}
    for path in files:
        try:
            pes[path] = PE(path)
        except ValueError:
            continue
        exports.setdefault(path.name.lower(), set()).update(pes[path].exports())

    for path in files:
        rel = path.relative_to(args.package)
        pe = pes.get(path)
        if pe is None:
            continue
        if not pe.is64:
            problems.append(f"{rel}: 32-битный файл в 64-битном пакете")
        # Версию подсистемы Windows проверяет только у программ. У библиотек её
        # не смотрит: например, UCRT из Windows SDK помечена 10.0, но Microsoft
        # поддерживает её поставку вместе с программой для Windows 7–8.1.
        if path.suffix.lower() in (".exe", ".com") and pe.subsystem_version > MAX_SUBSYSTEM:
            problems.append(f"{rel}: версия подсистемы {pe.subsystem_version[0]}."
                            f"{pe.subsystem_version[1]} новее Windows 8.1")
        is_qt = path.name.lower().startswith("qt6") or path.parent.name.lower() in QT_PLUGIN_DIRS
        if args.qt_version and is_qt:
            v = pe.file_version()
            if v != args.qt_version:
                problems.append(f"{rel}: версия Qt {v}, нужна {args.qt_version}")

        for dll, funcs, delayed in pe.imports():
            b = base(dll)
            how = " (отложенная загрузка)" if delayed else ""
            if args.list:
                print(f"{rel} -> {dll}{how}: {len(funcs)}")
            if dll.lower() in in_package:
                missing = [f for f in funcs if not f.startswith("#") and f not in exports[dll.lower()]]
                for f in missing[:5]:
                    problems.append(f"{rel}: в {dll} нет функции {f}{how}")
                if len(missing) > 5:
                    problems.append(f"{rel}: в {dll} нет ещё {len(missing) - 5} функций")
                continue
            if b in WIN10_ONLY_DLLS:
                problems.append(f"{rel}: нужна {dll}{how} — её нет в Windows 8.1")
                continue
            if b.startswith(UCRT_PREFIX):
                if "ucrtbase.dll" not in in_package:
                    problems.append(f"{rel}: нужна {dll} (UCRT), а UCRT нет в пакете")
                continue
            if b not in WIN81_DLLS:
                problems.append(f"{rel}: нужна {dll}{how} — её нет ни в пакете, "
                                f"ни в списке DLL Windows 8.1")
                continue
            for f in funcs:
                if f in WIN10_ONLY.get(b, ()):
                    problems.append(f"{rel}: {dll}!{f}{how} — функция Windows 10")

    print(f"Проверено файлов: {len(files)}")
    if problems:
        for p in problems:
            print(f"::error::{p}")
        return 1
    print("Пакет не требует ничего новее Windows 8.1")
    return 0


if __name__ == "__main__":
    sys.exit(main())
