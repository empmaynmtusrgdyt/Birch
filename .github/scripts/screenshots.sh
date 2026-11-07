#!/usr/bin/env bash
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Снимки окна на каждом языке интерфейса (по файлам translations/birch_*.ts):
# панель поиска — с проверкой, что она помещается в окно, — и пример с рамками
# в светлой, тёмной и чёрно-белой теме. Снимки — в текущей папке.
#
# Использование: screenshots.sh <имя системы> <команда запуска Birch>...
#   например: screenshots.sh macos /Volumes/Birch/Birch.app/Contents/MacOS/Birch
set -euo pipefail

system="$1"
shift
root="$(cd "$(dirname "$0")/../.." && pwd)"
examples="$root/resources/examples"
failed=0

for ts in "$root"/translations/birch_*.ts; do
    lang="$(basename "$ts" .ts)"
    lang="${lang#birch_}"
    suffix=".$lang"
    [ "$lang" = ru ] && suffix=""  # примеры на исходном языке — без суффикса
    export BIRCH_LANGUAGE="$lang"

    "$@" --find=-\> --screenshot "find-$system-$lang.png" "$examples/welcome$suffix.birch" > /dev/null
    if ! grep -q "^find_bar_fits=1" "find-$system-$lang.png.txt"; then
        echo "::error::Панель поиска не помещается в окно ($lang)"
        failed=1
    fi
    "$@" --screenshot-theme light --screenshot "frames-$lang-light.png" "$examples/frames$suffix.birch" > /dev/null
    "$@" --screenshot-theme dark --screenshot "frames-$lang-dark.png" "$examples/frames$suffix.birch" > /dev/null
    "$@" --screenshot-theme light --screenshot-monochrome --screenshot "frames-$lang-mono.png" "$examples/frames$suffix.birch" > /dev/null
    echo "Снимки: $lang"
done
exit "$failed"
