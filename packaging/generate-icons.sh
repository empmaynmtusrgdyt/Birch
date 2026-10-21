#!/usr/bin/env bash
# Генерирует app.icns (macOS) и app.ico (Windows) из resources/icons/app.svg.
# Результат лежит в репозитории, поэтому запускать нужно только после правки иконки.
# Нужны: rsvg-convert (brew install librsvg), python3; для .icns — iconutil (macOS).
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
svg="$root/resources/icons/app.svg"
out="$root/packaging"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

render() { rsvg-convert -w "$1" -h "$1" "$svg" -o "$2"; }

# --- macOS .icns ---
if command -v iconutil >/dev/null; then
    set_dir="$tmp/app.iconset"
    mkdir -p "$set_dir"
    for s in 16 32 128 256 512; do
        render "$s" "$set_dir/icon_${s}x${s}.png"
        render "$((s * 2))" "$set_dir/icon_${s}x${s}@2x.png"
    done
    iconutil -c icns "$set_dir" -o "$out/macos/app.icns"
    echo "-> packaging/macos/app.icns"
fi

# --- Windows .ico (PNG-кадры внутри ICO, поддерживается с Vista) ---
sizes=(16 24 32 48 64 128 256)
for s in "${sizes[@]}"; do render "$s" "$tmp/ico_$s.png"; done
python3 - "$out/windows/app.ico" "$tmp" "${sizes[@]}" <<'EOF'
import struct, sys, pathlib
dest, tmp, sizes = sys.argv[1], pathlib.Path(sys.argv[2]), [int(s) for s in sys.argv[3:]]
frames = [(s, (tmp / f"ico_{s}.png").read_bytes()) for s in sizes]
header = struct.pack("<HHH", 0, 1, len(frames))
offset = 6 + 16 * len(frames)
entries, blobs = b"", b""
for size, data in frames:
    dim = 0 if size >= 256 else size  # 0 означает 256
    entries += struct.pack("<BBBBHHII", dim, dim, 0, 0, 1, 32, len(data), offset)
    blobs += data
    offset += len(data)
pathlib.Path(dest).write_bytes(header + entries + blobs)
EOF
echo "-> packaging/windows/app.ico"
