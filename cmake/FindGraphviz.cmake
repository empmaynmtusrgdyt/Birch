# FindGraphviz.cmake
# -------------------
# Ищет библиотеки Graphviz (gvc, cgraph, cdt) и, по возможности, плагины
# gvplugin_dot_layout и gvplugin_core.
#
# Порядок поиска:
#   1. pkg-config (Homebrew, Debian/Ubuntu, Arch);
#   2. обычный find_path/find_library по CMAKE_PREFIX_PATH и GRAPHVIZ_ROOT.
#
# Результат:
#   Graphviz_FOUND, Graphviz_VERSION, Graphviz_INCLUDE_DIR
#   Graphviz_PLUGINS_FOUND     - найдены оба плагина
#   Imported targets:
#     Graphviz::cdt, Graphviz::cgraph, Graphviz::gvc
#     Graphviz::plugin_dot_layout, Graphviz::plugin_core (если найдены)
#
# Подсказка: -DGRAPHVIZ_ROOT=/путь/к/graphviz или переменная окружения GRAPHVIZ_ROOT.

include(FindPackageHandleStandardArgs)

set(_gv_hints "")
foreach(_root IN ITEMS "${GRAPHVIZ_ROOT}" "$ENV{GRAPHVIZ_ROOT}")
    if(_root)
        list(APPEND _gv_hints "${_root}")
    endif()
endforeach()

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_GVC QUIET libgvc)
endif()

# Заголовки: pkg-config отдаёт .../include/graphviz, поэтому в коде пишем #include <gvc.h>
find_path(Graphviz_INCLUDE_DIR
    NAMES gvc.h
    HINTS ${PC_GVC_INCLUDE_DIRS} ${_gv_hints}
    PATH_SUFFIXES include/graphviz graphviz)

set(_gv_lib_hints ${PC_GVC_LIBRARY_DIRS})
foreach(_h IN LISTS _gv_hints)
    list(APPEND _gv_lib_hints "${_h}/lib")
endforeach()

foreach(_lib IN ITEMS gvc cgraph cdt)
    find_library(Graphviz_${_lib}_LIBRARY
        NAMES ${_lib} lib${_lib}
        HINTS ${_gv_lib_hints})
endforeach()

# Плагины лежат в lib/graphviz (Homebrew, Linux) или рядом с основными библиотеками (Windows)
set(_gv_plugin_hints "")
foreach(_d IN LISTS _gv_lib_hints)
    list(APPEND _gv_plugin_hints "${_d}" "${_d}/graphviz")
endforeach()
if(Graphviz_gvc_LIBRARY)
    get_filename_component(_gv_libdir "${Graphviz_gvc_LIBRARY}" DIRECTORY)
    list(APPEND _gv_plugin_hints "${_gv_libdir}" "${_gv_libdir}/graphviz")
endif()

foreach(_plugin IN ITEMS dot_layout core)
    find_library(Graphviz_plugin_${_plugin}_LIBRARY
        # В Debian/Ubuntu у плагинов нет неверсионных .so-ссылок
        NAMES gvplugin_${_plugin} libgvplugin_${_plugin}
              libgvplugin_${_plugin}.so.8 libgvplugin_${_plugin}.so.6
        HINTS ${_gv_plugin_hints}
        NO_DEFAULT_PATH)
endforeach()

# Версия
if(PC_GVC_VERSION)
    set(Graphviz_VERSION "${PC_GVC_VERSION}")
elseif(Graphviz_INCLUDE_DIR AND EXISTS "${Graphviz_INCLUDE_DIR}/graphviz_version.h")
    file(STRINGS "${Graphviz_INCLUDE_DIR}/graphviz_version.h" _gv_ver_line
         REGEX "^#define[ \t]+PACKAGE_VERSION[ \t]+\"[^\"]+\"")
    string(REGEX REPLACE ".*\"([^\"]+)\".*" "\\1" Graphviz_VERSION "${_gv_ver_line}")
endif()

find_package_handle_standard_args(Graphviz
    REQUIRED_VARS Graphviz_gvc_LIBRARY Graphviz_cgraph_LIBRARY Graphviz_cdt_LIBRARY Graphviz_INCLUDE_DIR
    VERSION_VAR   Graphviz_VERSION)

if(Graphviz_plugin_dot_layout_LIBRARY AND Graphviz_plugin_core_LIBRARY)
    set(Graphviz_PLUGINS_FOUND TRUE)
else()
    set(Graphviz_PLUGINS_FOUND FALSE)
endif()

if(Graphviz_FOUND)
    macro(_gv_add_target name lib_var)
        if(NOT TARGET Graphviz::${name})
            add_library(Graphviz::${name} UNKNOWN IMPORTED)
            set_target_properties(Graphviz::${name} PROPERTIES
                IMPORTED_LOCATION             "${${lib_var}}"
                INTERFACE_INCLUDE_DIRECTORIES "${Graphviz_INCLUDE_DIR}")
        endif()
    endmacro()

    _gv_add_target(cdt    Graphviz_cdt_LIBRARY)
    _gv_add_target(cgraph Graphviz_cgraph_LIBRARY)
    _gv_add_target(gvc    Graphviz_gvc_LIBRARY)
    set_property(TARGET Graphviz::cgraph APPEND PROPERTY INTERFACE_LINK_LIBRARIES Graphviz::cdt)
    if(WIN32)
        # Graphviz на Windows — DLL: с GVDLL заголовки помечают данные
        # (Agdirected и т.п.) как __declspec(dllimport), иначе ошибка линковки.
        set_property(TARGET Graphviz::cdt APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS GVDLL)
    endif()
    set_property(TARGET Graphviz::gvc    APPEND PROPERTY INTERFACE_LINK_LIBRARIES Graphviz::cgraph Graphviz::cdt)

    if(Graphviz_PLUGINS_FOUND)
        _gv_add_target(plugin_dot_layout Graphviz_plugin_dot_layout_LIBRARY)
        _gv_add_target(plugin_core       Graphviz_plugin_core_LIBRARY)
        set_property(TARGET Graphviz::plugin_dot_layout APPEND PROPERTY INTERFACE_LINK_LIBRARIES Graphviz::gvc)
        set_property(TARGET Graphviz::plugin_core       APPEND PROPERTY INTERFACE_LINK_LIBRARIES Graphviz::gvc)
    endif()
endif()

mark_as_advanced(Graphviz_INCLUDE_DIR
    Graphviz_gvc_LIBRARY Graphviz_cgraph_LIBRARY Graphviz_cdt_LIBRARY
    Graphviz_plugin_dot_layout_LIBRARY Graphviz_plugin_core_LIBRARY)
