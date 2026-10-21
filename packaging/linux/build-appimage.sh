#!/usr/bin/env bash
# Собирает переносимый Birch-<версия>-x86_64.AppImage (Qt и Graphviz внутри).
# Запускать на самой старой поддерживаемой системе (например, Ubuntu 22.04):
# AppImage работает на дистрибутивах с glibc не старше, чем у сборочной машины.
#
#   sudo apt install qt6-base-dev qt6-svg-dev libgraphviz-dev cmake ninja-build file wget
#   ./packaging/linux/build-appimage.sh
set -euo pipefail

root="$(cd "$(dirname "$0")/../.." && pwd)"
build="$root/build/appimage"
appdir="$build/AppDir"
tools="$build/tools"
mkdir -p "$tools"

cmake -S "$root" -B "$build" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DBIRCH_DEPLOY_QT=OFF \
    -DBIRCH_BUILD_TESTS=OFF
cmake --build "$build"
rm -rf "$appdir"
DESTDIR="$appdir" cmake --install "$build"

fetch() {
    [ -x "$tools/$1" ] || { wget -qO "$tools/$1" "$2" && chmod +x "$tools/$1"; }
}
fetch linuxdeploy-x86_64.AppImage \
    https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
fetch linuxdeploy-plugin-qt-x86_64.AppImage \
    https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

export QMAKE="${QMAKE:-$(command -v qmake6 || echo /usr/lib/qt6/bin/qmake)}"
export EXTRA_QT_MODULES="svg"
# Для Wayland: установите qt6-wayland и добавьте EXTRA_PLATFORM_PLUGINS="libqwayland-generic.so"
export VERSION="$(sed -n 's/^ *VERSION \([0-9.]*\)$/\1/p' "$root/CMakeLists.txt" | head -1)"

cd "$build"
"$tools/linuxdeploy-x86_64.AppImage" \
    --appdir "$appdir" \
    --desktop-file "$appdir/usr/share/applications/birch.desktop" \
    --icon-file "$appdir/usr/share/icons/hicolor/scalable/apps/birch.svg" \
    --plugin qt \
    --output appimage

echo "Готово: $(ls "$build"/Birch*.AppImage)"
