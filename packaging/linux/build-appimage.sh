#!/usr/bin/env bash
# Собирает переносимый Birch-<версия>-x86_64.AppImage: Qt, Graphviz и его
# плагины лежат внутри, поэтому файл запускается и там, где системный Qt
# старше 6.5 (Ubuntu 24.04, Debian 12).
#
# Запускать на самой старой поддерживаемой системе: AppImage работает на
# дистрибутивах с glibc не старше, чем у сборочной машины. CI собирает на Ubuntu 22.04.
#
# Нужны: Qt >= 6.5 (QT_ROOT_DIR или системный), libgraphviz-dev, cmake, file, wget.
#   QT_ROOT_DIR=~/Qt/6.8.3/gcc_64 ./packaging/linux/build-appimage.sh
set -euo pipefail

root="$(cd "$(dirname "$0")/../.." && pwd)"
build="$root/build/appimage"
appdir="$build/AppDir"
tools="$build/tools"
mkdir -p "$tools"

prefix_args=()
if [ -n "${QT_ROOT_DIR:-}" ]; then
    prefix_args=(-DCMAKE_PREFIX_PATH="$QT_ROOT_DIR")
fi

cmake -S "$root" -B "$build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DBIRCH_BUILD_TESTS=OFF \
    -DBIRCH_LINK_GRAPHVIZ_PLUGINS=ON \
    "${prefix_args[@]}"
cmake --build "$build" -j"$(nproc)"
rm -rf "$appdir"
DESTDIR="$appdir" cmake --install "$build"

fetch() {
    [ -x "$tools/$1" ] || { wget -qO "$tools/$1" "$2" && chmod +x "$tools/$1"; }
}
fetch linuxdeploy-x86_64.AppImage \
    https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
fetch linuxdeploy-plugin-qt-x86_64.AppImage \
    https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

if [ -n "${QT_ROOT_DIR:-}" ]; then
    export QMAKE="$QT_ROOT_DIR/bin/qmake"
    export LD_LIBRARY_PATH="$QT_ROOT_DIR/lib:${LD_LIBRARY_PATH:-}"
else
    export QMAKE="${QMAKE:-$(command -v qmake6 || echo /usr/lib/qt6/bin/qmake)}"
fi
export EXTRA_QT_MODULES="svg"
export VERSION="$(sed -n 's/^ *VERSION \([0-9.]*\)$/\1/p' "$root/CMakeLists.txt" | head -1)"
# Внутри CI нет FUSE — запускаем инструменты через распаковку
export APPIMAGE_EXTRACT_AND_RUN=1

cd "$build"
"$tools/linuxdeploy-x86_64.AppImage" \
    --appdir "$appdir" \
    --desktop-file "$appdir/usr/share/applications/birch.desktop" \
    --icon-file "$appdir/usr/share/icons/hicolor/scalable/apps/birch.svg" \
    --plugin qt \
    --output appimage

echo "Готово: $(ls "$build"/Birch*.AppImage)"
