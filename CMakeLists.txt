cmake_minimum_required(VERSION 3.21)

project(Birch
    VERSION 0.1.0
    DESCRIPTION "Text-to-flowchart app for macOS (Qt 6 + Graphviz)"
    LANGUAGES CXX)

if(NOT APPLE)
    message(FATAL_ERROR "Birch поддерживает только macOS.")
endif()

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

# ---------------------------------------------------------------------------
# Зависимости
# ---------------------------------------------------------------------------
# Qt6::Svg нужен, чтобы рендерить SVG от Graphviz в QImage с любым масштабом
# (чёткая картинка на Retina и не нужен cairo-плагин Graphviz).
# 6.5+ — следим за сменой светлой/тёмной темы macOS на лету.
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Svg SvgWidgets)
qt_standard_project_setup()

# Свой модуль cmake/FindGraphviz.cmake: pkg-config -> поиск вручную.
find_package(Graphviz 9.0 REQUIRED)

# ---------------------------------------------------------------------------
# Предупреждения компилятора
# ---------------------------------------------------------------------------
function(birch_set_warnings target)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-sign-conversion)
    if(BIRCH_WARNINGS_AS_ERRORS)
        target_compile_options(${target} PRIVATE -Werror)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# birch_core: парсер DSL + рендерер Graphviz (без зависимости от Widgets,
# чтобы его можно было тестировать отдельно)
# ---------------------------------------------------------------------------
add_library(birch_core STATIC
    src/core/Diagram.h
    src/core/Parser.h
    src/core/Parser.cpp
    src/core/Renderer.h
    src/core/Renderer.cpp
    src/core/Theme.h
    src/core/Theme.cpp
)
target_include_directories(birch_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(birch_core
    PUBLIC  Qt6::Core Qt6::Gui
    PRIVATE Qt6::Svg Graphviz::gvc)

# Если найдены плагины Graphviz (dot_layout, core), линкуем их напрямую и
# регистрируем в коде. Тогда приложению не нужен файл config и поиск
# плагинов в рантайме — важно для самодостаточного .app-бандла.
if(Graphviz_PLUGINS_FOUND)
    target_link_libraries(birch_core PRIVATE Graphviz::plugin_dot_layout Graphviz::plugin_core)
    target_compile_definitions(birch_core PRIVATE BIRCH_GRAPHVIZ_BUILTIN_PLUGINS=1)
endif()
birch_set_warnings(birch_core)

# ---------------------------------------------------------------------------
# Приложение
# ---------------------------------------------------------------------------
qt_add_executable(Birch MACOSX_BUNDLE
    src/main.cpp
    src/app/RenderController.h
    src/app/RenderController.cpp
    src/ui/CodeEditor.h
    src/ui/CodeEditor.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
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
        resources/icons/copy.svg
        resources/icons/direction.svg
        resources/icons/download.svg
        resources/icons/fit.svg
        resources/icons/render.svg
        resources/icons/zoom-in.svg
        resources/icons/zoom-out.svg
        resources/examples/welcome.birch
)

target_link_libraries(Birch PRIVATE birch_core Qt6::Widgets Qt6::Svg Qt6::SvgWidgets)
target_compile_definitions(Birch PRIVATE BIRCH_VERSION="${PROJECT_VERSION}")
birch_set_warnings(Birch)

# ---------------------------------------------------------------------------
# Бандл macOS: иконка, Info.plist (в т.ч. тип файлов .birch)
# ---------------------------------------------------------------------------
set(BIRCH_ICNS "${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/app.icns")
set_source_files_properties("${BIRCH_ICNS}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
target_sources(Birch PRIVATE "${BIRCH_ICNS}")
set_target_properties(Birch PROPERTIES
    OUTPUT_NAME                        "Birch"
    MACOSX_BUNDLE_INFO_PLIST           "${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/Info.plist.in"
    MACOSX_BUNDLE_ICON_FILE            "app.icns"
    MACOSX_BUNDLE_GUI_IDENTIFIER       "app.birch.flowchart"
    MACOSX_BUNDLE_BUNDLE_NAME          "Birch"
    MACOSX_BUNDLE_BUNDLE_VERSION       "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")

# ---------------------------------------------------------------------------
# Тесты
# ---------------------------------------------------------------------------
if(BIRCH_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Установка и .dmg: cmake --install / cpack
# ---------------------------------------------------------------------------
include(Packaging)

message(STATUS "Birch ${PROJECT_VERSION}: Qt ${Qt6_VERSION}, Graphviz ${Graphviz_VERSION}, "
               "builtin plugins: ${Graphviz_PLUGINS_FOUND}, deploy: ${BIRCH_DEPLOY_QT}")
