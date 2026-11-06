# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

"""Проверяет, что у каждой программы и библиотеки в пакете Windows есть лицензия.

Каждый .exe, .com и .dll пакета должен относиться к известному компоненту, а
этот компонент — быть описан в THIRD-PARTY-NOTICES.txt (по заголовку раздела).
Новая DLL — например, после обновления Qt или Graphviz — без описания делает
проверку красной: её лицензию нужно добавить в THIRD-PARTY-NOTICES.txt.

Использование: check-windows-licenses.py ПАПКА_ПАКЕТА
"""

import re
import sys
from pathlib import Path

# (шаблон имени файла, заголовок раздела в THIRD-PARTY-NOTICES.txt)
COMPONENTS = [
    (r"birch\.(exe|com)", None),  # сам Birch — LICENSE
    (r"qt6[a-z0-9]*\.dll", "Qt 6"),
    (r"q[a-z0-9_]+\.dll", "Qt 6"),  # плагины Qt в plugins\*
    (r"(gvc|cgraph|cdt|pathplan|xdot|gvplugin_[a-z_]+)\.dll", "Graphviz"),
    (r"libexpat\.dll", "Expat"),
    (r"zlib1\.dll", "zlib"),
    (r"(msvcp140(_[a-z0-9_]+)?|vcruntime140(_[a-z0-9_]+)?|concrt140|vccorlib140)\.dll",
     "Среда выполнения Microsoft Visual C++"),
    (r"(ucrtbase|api-ms-win-[a-z0-9-]+)\.dll", "Среда выполнения Microsoft Visual C++"),
]


def main() -> int:
    package = Path(sys.argv[1])
    root = Path(__file__).resolve().parents[2]
    notices = (root / "THIRD-PARTY-NOTICES.txt").read_text(encoding="utf-8")
    headings = {line.strip() for line in notices.splitlines()}

    problems = []
    used = set()
    for path in sorted(package.rglob("*")):
        if path.suffix.lower() not in (".exe", ".com", ".dll"):
            continue
        name = path.name.lower()
        for pattern, heading in COMPONENTS:
            if re.fullmatch(pattern, name):
                if heading is not None:
                    used.add(heading)
                break
        else:
            problems.append(f"{path.relative_to(package)}: неизвестная библиотека — "
                            f"добавьте её лицензию в THIRD-PARTY-NOTICES.txt и в этот скрипт")
    for heading in sorted(used):
        if heading not in headings:
            problems.append(f"в THIRD-PARTY-NOTICES.txt нет раздела «{heading}»")

    for p in problems:
        print(f"::error::{p}")
    if problems:
        return 1
    print("Лицензии есть у всех библиотек пакета: " + ", ".join(sorted(used)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
