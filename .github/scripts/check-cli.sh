#!/usr/bin/env bash
# Проверяет командную строку Birch на настоящей сборке: справку и версию,
# коды выхода, рендер в PNG, SVG и Mermaid, чтение со стандартного ввода.
# Запускается без экрана и без QT_QPA_PLATFORM — так, как на сервере или в CI.
#
# Использование: check-cli.sh <команда запуска Birch> [аргументы...]
#   check-cli.sh ./build/linux/birch
#   APPIMAGE_EXTRACT_AND_RUN=1 check-cli.sh ./Birch.AppImage
set -uo pipefail

root="$(cd "$(dirname "$0")/../.." && pwd)"
version=$(sed -n 's/^ *VERSION \([0-9.]*\)$/\1/p' "$root/CMakeLists.txt" | head -1)
example="$root/resources/examples/welcome.birch"
frames="$root/resources/examples/frames.birch"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

failed=0
fail() { echo "::error::$*"; failed=1; }
same() { [ "$(cksum < "$1")" = "$(cksum < "$2")" ]; }  # cmp есть не во всех контейнерах
birch() { env -u QT_QPA_PLATFORM -u DISPLAY -u WAYLAND_DISPLAY "$@"; }
cmd=("$@")

# Запуск: код выхода в $code, вывод в $work/out и $work/err.
run() {
    birch "${cmd[@]}" "$@" > "$work/out" 2> "$work/err"
    code=$?
}
expect_code() {
    [ "$code" -eq "$1" ] || fail "$2: код $code, ожидался $1. stderr: $(cat "$work/err")"
}

# --- справка и версия ---------------------------------------------------------
run --help
expect_code 0 "--help"
head -1 "$work/out" | grep -q '^Использование: ' || fail "--help: первая строка — $(head -1 "$work/out")"
cp "$work/out" "$work/help"
run -h
same "$work/out" "$work/help" || fail "-h и --help выводят разное"

run --version
expect_code 0 "--version"
[ "$(head -1 "$work/out")" = "birch (Birch) $version" ] || fail "--version: первая строка — $(head -1 "$work/out")"
grep -q 'Marth Scoobert' "$work/out" || fail "--version: нет автора"
cp "$work/out" "$work/version"
run -v
same "$work/out" "$work/version" || fail "-v и --version выводят разное"
echo "$(head -1 "$work/version") — $(tail -1 "$work/version")"

# --- неверные ключи -----------------------------------------------------------
run --colour
expect_code 3 "--colour"
grep -q -- '--help' "$work/err" || fail "--colour: нет подсказки про --help"
run -t dark "$example"
expect_code 3 "-t без -o"
grep -q 'только вместе с -o' "$work/err" || fail "-t без -o: $(cat "$work/err")"

# --- рендер -------------------------------------------------------------------
run "$frames" -o "$work/a.png"
expect_code 0 "рендер PNG"
head -c 8 "$work/a.png" 2>/dev/null | od -c | grep -q 'P   N   G' || fail "a.png — не PNG"
[ -s "$work/err" ] && fail "рендер PNG: лишний вывод в stderr: $(cat "$work/err")"

run "$frames" -o "$work/a.svg"
expect_code 0 "рендер SVG"
grep -q '<svg' "$work/a.svg" 2>/dev/null || fail "a.svg — не SVG"

run "$frames" -o "$work/a.mmd" -H
expect_code 0 "рендер Mermaid"
[ "$(head -1 "$work/a.mmd" 2>/dev/null)" = "flowchart LR" ] || fail "a.mmd: $(head -1 "$work/a.mmd")"

# Короткие и длинные ключи дают одну и ту же картинку.
run "$example" -t dark -ma -s 1 -o "$work/short.png"
expect_code 0 "короткие ключи"
run "$example" --theme=dark --monochrome --transparent --scale=1 --output="$work/long.png"
expect_code 0 "длинные ключи"
same "$work/short.png" "$work/long.png" || fail "короткие и длинные ключи дали разные картинки"

birch "${cmd[@]}" - -o "$work/stdin.svg" < "$example" > /dev/null 2>&1
code=$?
expect_code 0 "чтение со стандартного ввода"
grep -q '<svg' "$work/stdin.svg" 2>/dev/null || fail "stdin.svg — не SVG"

# --- ошибки в схеме и файлах ---------------------------------------------------
printf '[A] -> [B\n' > "$work/bad.birch"
run "$work/bad.birch" -o "$work/bad.png"
expect_code 1 "ошибка в схеме"
grep -q 'bad.birch:1:8: ошибка: ' "$work/err" || fail "ошибка в схеме: $(cat "$work/err")"
[ -e "$work/bad.png" ] && fail "при ошибке в схеме файл не должен появляться"

run "$work/нет такого.birch" -o "$work/x.png"
expect_code 2 "нет входного файла"

[ "$failed" -eq 0 ] && echo "Командная строка: все проверки пройдены"
exit "$failed"
