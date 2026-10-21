#!/usr/bin/env bash
# Генерирует packaging/macos/app.icns из resources/icons/app.svg.
# Результат лежит в репозитории, поэтому запускать нужно только после правки иконки.
# Нужны: rsvg-convert (brew install librsvg) и iconutil (входит в macOS).
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
svg="$root/resources/icons/app.svg"
out="$root/packaging"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

render() { rsvg-convert -w "$1" -h "$1" "$svg" -o "$2"; }

set_dir="$tmp/app.iconset"
mkdir -p "$set_dir"
for s in 16 32 128 256 512; do
    render "$s" "$set_dir/icon_${s}x${s}.png"
    render "$((s * 2))" "$set_dir/icon_${s}x${s}@2x.png"
done
iconutil -c icns "$set_dir" -o "$out/macos/app.icns"
echo "-> packaging/macos/app.icns"
