#!/usr/bin/env bash
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Обновляет файлы перевода translations/birch_*.ts по строкам в коде (lupdate).
# Запускайте после того, как добавили или изменили строку интерфейса, затем
# переведите новые строки (они помечены type="unfinished") — например, в Qt Linguist.
#
# Исходный язык строк — русский, поэтому birch_ru.ts содержит только фразы с
# числами (%n): для них нужны формы множественного числа.
#
# Использование: translations/update.sh [--check [--allow-unfinished]]
#   --check             ничего не меняет; ошибка, если файлы .ts устарели или в
#                       них есть непереведённые строки (так проверяет CI)
#   --allow-unfinished  с --check: непереведённые строки — не ошибка
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
lupdate="${LUPDATE:-$(command -v lupdate || command -v lupdate-qt6 || true)}"
if [ -z "$lupdate" ]; then
    for dir in /opt/homebrew/opt/qt/bin /usr/lib/qt6/bin /usr/lib/qt6/libexec; do
        [ -x "$dir/lupdate" ] && lupdate="$dir/lupdate" && break
    done
fi
[ -n "$lupdate" ] || { echo "Не найден lupdate (задайте LUPDATE=…)" >&2; exit 2; }

check=0
allow_unfinished=0
for arg in "$@"; do
    case "$arg" in
        --check) check=1 ;;
        --allow-unfinished) allow_unfinished=1 ;;
        *) echo "Неизвестный ключ: $arg" >&2; exit 2 ;;
    esac
done

work="$root/translations"
if [ "$check" -eq 1 ]; then
    work="$(mktemp -d)"
    trap 'rm -rf "$work"' EXIT
    cp "$root"/translations/birch_*.ts "$work/"
fi

cd "$root"
# Без номеров строк (-locations none): иначе файлы меняются от любой правки кода.
options=(-locations none -no-obsolete -source-language ru -recursive src)
others=()
for ts in "$work"/birch_*.ts; do
    [ "$(basename "$ts")" = birch_ru.ts ] || others+=("$ts")
done
"$lupdate" -silent "${options[@]}" -ts "${others[@]}"
"$lupdate" -silent "${options[@]}" -pluralonly -target-language ru -ts "$work/birch_ru.ts"

if [ "$check" -eq 1 ]; then
    failed=0
    for ts in "$work"/birch_*.ts; do
        name="$(basename "$ts")"
        if ! cmp -s "$ts" "$root/translations/$name"; then
            echo "::error file=translations/$name::устарел — запустите translations/update.sh и переведите новые строки"
            diff -u "$root/translations/$name" "$ts" | head -40
            failed=1
        fi
        unfinished=$(grep -c 'type="unfinished"' "$ts" || true)
        if [ "$unfinished" -gt 0 ] && [ "$allow_unfinished" -eq 0 ]; then
            echo "::error file=translations/$name::непереведённых строк: $unfinished"
            failed=1
        fi
    done
    [ "$failed" -eq 0 ] && echo "Переводы в порядке: $(ls "$work"/birch_*.ts | xargs -n1 basename | tr '\n' ' ')"
    exit "$failed"
fi
echo "Обновлены: $(ls "$work"/birch_*.ts | xargs -n1 basename | tr '\n' ' ')"
