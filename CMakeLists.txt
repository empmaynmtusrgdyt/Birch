# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.21)

project(Birch
    VERSION 0.10.0
    DESCRIPTION "Text-to-flowchart desktop app (Qt 6 + Graphviz)"
    LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Общие настройки
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BIRCH_BUILD_TESTS        "Собирать unit-тесты"                 ON)
option(BIRCH_WARNINGS_AS_ERRORS "Считать предупреждения ошибками"     OFF)

# Встраивать плагины Graphviz (dot_layout, core) в приложение. Нужно для
# самодостаточных пакетов (.app, Windows, AppImage). Пакеты дистрибутивов Linux
# используют плагины системного Graphviz — у них нет ссылок для линковки.
if(APPLE OR WIN32)
    set(_birch_link_plugins_default ON)
else()
    set(_birch_link_plugins_default OFF)
endif()
option(BIRCH_LINK_GRAPHVIZ_PLUGINS "Встроить плагины Graphviz в приложение" ${_birch_link_plugins_default})

# ---------------------------------------------------------------------------
# Зависимости
# ---------------------------------------------------------------------------
# Qt6::Svg нужен, чтобы рендерить SVG от Graphviz в QImage с любым масштабом
# (чёткая картинка на HiDPI и не нужен cairo-плагин Graphviz).
# 6.5+ — следим за сменой светлой/тёмной темы системы на лету.
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Svg SvgWidgets LinguistTools)
qt_standard_project_setup()

# Свой модуль cmake/FindGraphviz.cmake: pkg-config -> поиск вручную.
# 2.42 — версия в Debian и Ubuntu; код совместим с ней и с актуальными 1x.x.
find_package(Graphviz 2.42 REQUIRED)

# ---------------------------------------------------------------------------
# Предупреждения компилятора
# ---------------------------------------------------------------------------
function(birch_set_warnings target)
    if(MSVC)
        # /utf-8 обязателен: в исходниках кириллица
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
        if(BIRCH_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-sign-conversion)
        if(BIRCH_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

# ---------------------------------------------------------------------------
# birch_core: парсер DSL + рендерер Graphviz (без зависимости от Widgets,
# чтобы его можно было тестировать отдельно)
# ---------------------------------------------------------------------------
add_library(birch_core STATIC
    src/core/CommandLine.h
    src/core/CommandLine.cpp
    src/core/Language.h
    src/core/Language.cpp
    src/core/Diagram.h
    src/core/MermaidExporter.h
    src/core/MermaidExporter.cpp
    src/core/Parser.h
    src/core/Parser.cpp
    src/core/Renderer.h
    src/core/Renderer.cpp
    src/core/ShellCommand.h
    src/core/ShellCommand.cpp
    src/core/Theme.h
    src/core/Theme.cpp
)
target_include_directories(birch_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Переводы интерфейса: translations/birch_<язык>.ts → .qm внутри birch_core (:/i18n).
# Исходный язык строк — русский; birch_ru.ts содержит только формы множественного
# числа. Файлы .ts обновляет translations/update.sh (lupdate), а не сборка.
# Новый язык — новый файл translations/birch_<код>.ts: сборка найдёт его сама.
file(GLOB BIRCH_TS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/translations/birch_*.ts")
qt_add_lrelease(birch_core
    TS_FILES ${BIRCH_TS_FILES}
    QM_FILES_OUTPUT_VARIABLE _birch_qm_files)
# Перевод стандартных диалогов и контекстных меню Qt — если есть в этой сборке Qt
# (иначе Birch попробует взять его из Qt при запуске). Языки — те же, что у .ts;
# английский не нужен: строки Qt и так английские.
foreach(_ts IN LISTS BIRCH_TS_FILES)
    string(REGEX REPLACE ".*/birch_(.+)\\.ts$" "\\1" _lang "${_ts}")
    if(_lang STREQUAL "en")
        continue()
    endif()
    set(_qt_qm "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_TRANSLATIONS}/qtbase_${_lang}.qm")
    if(EXISTS "${_qt_qm}")
        set_source_files_properties("${_qt_qm}" PROPERTIES QT_RESOURCE_ALIAS "qtbase_${_lang}.qm")
        list(APPEND _birch_qm_files "${_qt_qm}")
    else()
        message(STATUS "Нет ${_qt_qm}: стандартные диалоги Qt — на языке Qt по умолчанию")
    endif()
endforeach()
qt_add_resources(birch_core "birch_i18n" PREFIX "/i18n" BASE "${CMAKE_CURRENT_BINARY_DIR}" FILES ${_birch_qm_files})
target_link_libraries(birch_core
    PUBLIC  Qt6::Core Qt6::Gui
    PRIVATE Qt6::Svg Graphviz::gvc)

# Плагины Graphviz (dot_layout, core) линкуем напрямую и регистрируем в коде.
# Тогда приложению не нужен файл config и поиск плагинов в рантайме — важно
# для самодостаточных пакетов. Иначе Graphviz найдёт плагины сам (config8/config6).
if(Graphviz_PLUGINS_FOUND AND BIRCH_LINK_GRAPHVIZ_PLUGINS)
    set(BIRCH_GRAPHVIZ_BUILTIN TRUE)
    target_link_libraries(birch_core PRIVATE Graphviz::plugin_dot_layout Graphviz::plugin_core)
    target_compile_definitions(birch_core PRIVATE BIRCH_GRAPHVIZ_BUILTIN_PLUGINS=1)
else()
    set(BIRCH_GRAPHVIZ_BUILTIN FALSE)
endif()
if(WIN32)
    target_link_libraries(birch_core PRIVATE shell32)  # CommandLineToArgvW
endif()
birch_set_warnings(birch_core)

# ---------------------------------------------------------------------------
# birch_editor: редактор текста и панель поиска — отдельно от окна,
# чтобы их можно было тестировать
# ---------------------------------------------------------------------------
add_library(birch_editor STATIC
    src/ui/CodeEditor.h
    src/ui/CodeEditor.cpp
    src/ui/FindBar.h
    src/ui/FindBar.cpp
)
target_link_libraries(birch_editor PUBLIC birch_core Qt6::Widgets)
birch_set_warnings(birch_editor)

# ---------------------------------------------------------------------------
# Приложение
# ---------------------------------------------------------------------------
qt_add_executable(Birch WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/app/RenderController.h
    src/app/RenderController.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
    src/ui/InstallCommand.h
    src/ui/InstallCommand.cpp
    src/ui/PreviewView.h
    src/ui/PreviewView.cpp
    src/ui/SyntaxHighlighter.h
    src/ui/SyntaxHighlighter.cpp
)

qt_add_resources(Birch "birch_resources"
    PREFIX "/"
    BASE resources
    FILES
        resources/icons/app.svg
        resources/icons/chevron-down.svg
        resources/icons/chevron-up.svg
        resources/icons/close.svg
        resources/icons/copy.svg
        resources/icons/direction.svg
        resources/icons/download.svg
        resources/icons/fit.svg
        resources/icons/monochrome.svg
        resources/icons/render.svg
        resources/icons/zoom-in.svg
        resources/icons/zoom-out.svg
        resources/examples/welcome.birch
        resources/examples/welcome.en.birch
        resources/examples/frames.birch
        resources/examples/frames.en.birch
)

target_link_libraries(Birch PRIVATE birch_core birch_editor Qt6::Widgets Qt6::Svg Qt6::SvgWidgets)
target_compile_definitions(Birch PRIVATE BIRCH_VERSION="${PROJECT_VERSION}")
birch_set_warnings(Birch)

# ---------------------------------------------------------------------------
# Платформенные настройки приложения
# ---------------------------------------------------------------------------
if(APPLE)
    # Бандл: иконка, Info.plist (в т.ч. тип файлов .birch)
    set(BIRCH_ICNS "${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/app.icns")
    # Лицензия Birch и лицензии Qt и Graphviz, которые входят в бандл
    set(BIRCH_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/THIRD-PARTY-NOTICES.txt")
    file(GLOB BIRCH_THIRD_PARTY_LICENSES "${CMAKE_CURRENT_SOURCE_DIR}/licenses/*.txt")
    set_source_files_properties("${BIRCH_ICNS}" ${BIRCH_LICENSE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${BIRCH_THIRD_PARTY_LICENSES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/licenses)
    target_sources(Birch PRIVATE "${BIRCH_ICNS}" ${BIRCH_LICENSE} ${BIRCH_THIRD_PARTY_LICENSES})
    set_target_properties(Birch PROPERTIES
        OUTPUT_NAME                        "Birch"
        MACOSX_BUNDLE_INFO_PLIST           "${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/Info.plist.in"
        MACOSX_BUNDLE_ICON_FILE            "app.icns"
        MACOSX_BUNDLE_GUI_IDENTIFIER       "app.birch.flowchart"
        MACOSX_BUNDLE_BUNDLE_NAME          "Birch"
        MACOSX_BUNDLE_BUNDLE_VERSION       "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
elseif(WIN32)
    # Иконка .exe и сведения о версии (Свойства файла -> Подробно)
    set(BIRCH_ICO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/app.ico")
    configure_file(packaging/windows/birch.rc.in "${CMAKE_CURRENT_BINARY_DIR}/birch.rc" @ONLY)
    target_sources(Birch PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/birch.rc")
    set_target_properties(Birch PROPERTIES OUTPUT_NAME "Birch")

    # birch.com — консольная обёртка для командной строки (см. src/launcher).
    # Без Qt и со встроенной средой выполнения MSVC: лежит отдельно от DLL.
    add_executable(birch_console src/launcher/birch-com.cpp)
    set_target_properties(birch_console PROPERTIES
        OUTPUT_NAME "birch"
        SUFFIX ".com"
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    birch_set_warnings(birch_console)
    add_dependencies(Birch birch_console)
else()
    # Linux: имя бинарника в нижнем регистре; если плагины Graphviz встроены, они
    # лежат вне стандартных путей (/usr/lib/.../graphviz) — сохраняем путь в RPATH.
    set_target_properties(Birch PROPERTIES
        OUTPUT_NAME "birch"
        INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# ---------------------------------------------------------------------------
# Тесты
# ---------------------------------------------------------------------------
if(BIRCH_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Установка и пакеты: cmake --install / cpack
# ---------------------------------------------------------------------------
include(Packaging)

message(STATUS "Birch ${PROJECT_VERSION}: Qt ${Qt6_VERSION}, Graphviz ${Graphviz_VERSION}, "
               "builtin plugins: ${BIRCH_GRAPHVIZ_BUILTIN}")
