# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Пакет для Windows 8.1: заменяет разложенные windeployqt библиотеки Qt
# версиями с исправлениями qt6windows7 (https://github.com/crystalidea/qt6windows7).
# Qt 6 официально работает только на Windows 10+, эти исправления добавляют
# запасные пути для старых Windows и ничего не меняют на новых.
#
# Заменяются только файлы, которые уже есть в пакете и есть в архиве: Qt*.dll
# и плагины. Среда выполнения MSVC из архива не берётся — своя у нас новее.
#
# Входные переменные:
#   BIRCH_QT_WINDOWS8_DIR — распакованный архив qt6windows7 той же версии Qt
#   BIRCH_PACKAGE_ROOT    — корень устанавливаемого пакета

file(GLOB_RECURSE _birch_package_files LIST_DIRECTORIES false "${BIRCH_PACKAGE_ROOT}/*.dll")
file(GLOB_RECURSE _birch_qt_files LIST_DIRECTORIES false RELATIVE "${BIRCH_QT_WINDOWS8_DIR}"
     "${BIRCH_QT_WINDOWS8_DIR}/*.dll")

set(_birch_replaced "")
foreach(_rel IN LISTS _birch_qt_files)
    get_filename_component(_name "${_rel}" NAME)
    if(NOT _name MATCHES "^Qt6" AND NOT _rel MATCHES "/")
        continue()  # msvcp140.dll, vcruntime140.dll и т. п.
    endif()
    foreach(_target IN LISTS _birch_package_files)
        string(LENGTH "${_target}" _len)
        string(LENGTH "/${_rel}" _tail_len)
        if(_len LESS _tail_len)
            continue()
        endif()
        math(EXPR _start "${_len} - ${_tail_len}")
        string(SUBSTRING "${_target}" ${_start} -1 _tail)
        string(TOLOWER "${_tail}" _tail)
        string(TOLOWER "/${_rel}" _want)
        if(_tail STREQUAL _want)
            file(COPY_FILE "${BIRCH_QT_WINDOWS8_DIR}/${_rel}" "${_target}")
            file(RELATIVE_PATH _shown "${BIRCH_PACKAGE_ROOT}" "${_target}")
            message(STATUS "Qt для Windows 8.1: ${_shown}")
            list(APPEND _birch_replaced "${_rel}")
        endif()
    endforeach()
endforeach()

# Без этих файлов на Windows 8.1 программа не запустится вовсе
foreach(_must IN ITEMS Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll platforms/qwindows.dll)
    if(NOT _must IN_LIST _birch_replaced)
        message(FATAL_ERROR "Не заменён ${_must}: нет в архиве qt6windows7 или в пакете")
    endif()
endforeach()
