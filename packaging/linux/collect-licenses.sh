#!/usr/bin/env bash
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Собирает лицензии библиотек, которые linuxdeploy положил в AppDir.
#
# Для каждой библиотеки в usr/lib находит пакет Ubuntu (dpkg -S), из которого
# она взята, копирует его файл /usr/share/doc/<пакет>/copyright — там Debian и
# Ubuntu хранят лицензии — и пишет сводку LIBRARIES.txt: пакет, версия,
# библиотеки. Qt и ICU из сборки Qt описаны в THIRD-PARTY-NOTICES.txt, их
# тексты — в licenses/LGPL-3.0.txt и licenses/ICU.txt.
#
# Библиотека, для которой лицензия не нашлась, — ошибка: в AppImage не должно
# быть библиотек без лицензии.
#
# Использование: collect-licenses.sh <AppDir>
set -euo pipefail

appdir="$1"
out="$appdir/usr/share/doc/birch/licenses"
mkdir -p "$out/ubuntu"
declare -A libs version
failed=0

for lib in "$appdir"/usr/lib/*.so*; do
    [ -f "$lib" ] || continue
    name="$(basename "$lib")"
    case "$name" in
        libQt6*|libicu*) continue ;;  # Qt и его ICU — в THIRD-PARTY-NOTICES.txt
    esac
    pkg=""
    for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/graphviz; do
        [ -e "$dir/$name" ] || continue
        # dpkg знает файл под тем путём, под которым его установил пакет;
        # на системах с объединённым /usr это может быть и /lib, и /usr/lib.
        real="$(readlink -f "$dir/$name")"
        for candidate in "$dir/$name" "$real" "${real#/usr}" "/usr$real"; do
            pkg="$(dpkg -S "$candidate" 2>/dev/null | head -1 | cut -d: -f1)" || true
            [ -n "$pkg" ] && break 2
        done
    done
    if [ -z "$pkg" ] || [ ! -f "/usr/share/doc/$pkg/copyright" ]; then
        echo "::error::Лицензии библиотек: не нашёл пакет Ubuntu с лицензией для $name"
        failed=1
        continue
    fi
    libs[$pkg]="${libs[$pkg]:+${libs[$pkg]}, }$name"
    version[$pkg]="$(dpkg-query -W -f='${Version}' "$pkg")"
    cp "/usr/share/doc/$pkg/copyright" "$out/ubuntu/$pkg.copyright"
done
[ "$failed" -eq 0 ] || exit 1

{
    echo "Библиотеки внутри AppImage и их лицензии"
    echo "========================================"
    echo
    echo "Список собран автоматически при сборке AppImage: для каждой библиотеки"
    echo "указан пакет Ubuntu $(. /etc/os-release && echo "$VERSION_ID"), из которого она взята, и его версия."
    echo "Лицензия и авторы — в файле ubuntu/<пакет>.copyright рядом с этим списком."
    echo "Исходный код пакетов: https://launchpad.net/ubuntu/+source/<пакет>"
    echo
    echo "Qt и библиотеки ICU из сборки Qt описаны в THIRD-PARTY-NOTICES.txt;"
    echo "их лицензии — LGPL-3.0.txt (с GPL-3.0.txt) и ICU.txt."
    for pkg in $(printf '%s\n' "${!libs[@]}" | sort); do
        echo
        echo "$pkg ${version[$pkg]}"
        echo "  Лицензия: ubuntu/$pkg.copyright"
        echo "  Библиотеки: ${libs[$pkg]}"
    done
} > "$out/LIBRARIES.txt"

echo "Лицензии библиотек: ${#libs[@]} пакетов Ubuntu → usr/share/doc/birch/licenses/LIBRARIES.txt"
