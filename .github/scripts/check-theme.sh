#!/usr/bin/env bash
# Проверяет, что тёмная и светлая темы применяются ко всему интерфейсу, а не
# только к схеме: запускает Birch с --theme и --screenshot и сверяет фактический
# цвет фона редактора на снимке (Birch пишет его в <снимок>.txt).
#
# Использование: check-theme.sh <команда запуска Birch> [аргументы...]
#   check-theme.sh ./build/linux/birch
#   check-theme.sh xvfb-run -a ./Birch.AppImage
set -euo pipefail

example="${BIRCH_EXAMPLE:-resources/examples/welcome.birch}"
for theme in dark light; do
    shot="theme-$theme.png"
    "$@" --theme "$theme" --screenshot "$shot" "$example"
    lightness=$(sed -n 's/^lightness=//p' "$shot.txt")
    echo "$theme: $(tr '\n' ' ' < "$shot.txt")"
    if [ "$theme" = dark ] && [ "$lightness" -ge 100 ]; then
        echo "::error::Тёмная тема не применилась к редактору (яркость фона $lightness)"
        exit 1
    fi
    if [ "$theme" = light ] && [ "$lightness" -le 155 ]; then
        echo "::error::Светлая тема не применилась к редактору (яркость фона $lightness)"
        exit 1
    fi
done
