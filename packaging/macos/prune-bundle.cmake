# Убирает из Birch.app то, что macdeployqt (особенно Qt из Homebrew) копирует лишнего:
#  - плагины, которые приложению не нужны и тянут QtQuick/QtPdf/QtQml;
#  - фреймворки Qt, на которые после этого никто не ссылается;
#  - абсолютные rpath (пути машины сборки): без них бандл гарантированно
#    не подхватит библиотеки из /opt/homebrew, даже на машине разработчика.
#  В конце проверяет, что бандл самодостаточен: все зависимости лежат внутри,
#  и нет ссылок на пути машины сборки. Иначе установка завершается ошибкой.
# Вызывается при установке до подписи; ожидает переменные BIRCH_APP и BIRCH_LIB_DIRS.

set(_contents "${BIRCH_APP}/Contents")

file(REMOVE_RECURSE
    "${_contents}/PlugIns/platforminputcontexts"   # виртуальная клавиатура (QtQuick)
    "${_contents}/PlugIns/imageformats/libqpdf.dylib")

# Все Mach-O файлы бандла: исполняемый файл, плагины, dylib и бинарники фреймворков.
function(_birch_macho_files out)
    file(GLOB_RECURSE _all LIST_DIRECTORIES false "${_contents}/MacOS/*" "${_contents}/PlugIns/*.dylib"
         "${_contents}/Frameworks/*.dylib")
    file(GLOB _fw LIST_DIRECTORIES true "${_contents}/Frameworks/*.framework")
    foreach(_f IN LISTS _fw)
        get_filename_component(_name "${_f}" NAME_WE)
        if(EXISTS "${_f}/Versions/A/${_name}")
            list(APPEND _all "${_f}/Versions/A/${_name}")
        endif()
    endforeach()
    set(${out} ${_all} PARENT_SCOPE)
endfunction()

# Повторяем, пока что-то удаляется: удаление одного фреймворка может «освободить» другой.
set(_changed TRUE)
while(_changed)
    set(_changed FALSE)
    _birch_macho_files(_files)

    set(_refs "")
    foreach(_bin IN LISTS _files)
        execute_process(COMMAND otool -L "${_bin}" OUTPUT_VARIABLE _out ERROR_QUIET)
        # Первая строка — сам файл, первая зависимость у фреймворка — его собственный id.
        string(REGEX MATCHALL "Qt[A-Za-z0-9]+\\.framework" _found "${_out}")
        get_filename_component(_self "${_bin}" NAME)
        list(REMOVE_ITEM _found "${_self}.framework")
        list(APPEND _refs ${_found})
    endforeach()
    list(REMOVE_DUPLICATES _refs)

    file(GLOB _frameworks LIST_DIRECTORIES true "${_contents}/Frameworks/Qt*.framework")
    foreach(_fw IN LISTS _frameworks)
        get_filename_component(_name "${_fw}" NAME)
        if(NOT _name IN_LIST _refs)
            message(STATUS "Удаляю неиспользуемый ${_name}")
            file(REMOVE_RECURSE "${_fw}")
            set(_changed TRUE)
        endif()
    endforeach()
endwhile()

# Удаляем rpath, указывающие за пределы бандла.
_birch_macho_files(_files)
foreach(_bin IN LISTS _files)
    execute_process(COMMAND otool -l "${_bin}" OUTPUT_VARIABLE _out ERROR_QUIET)
    string(REGEX MATCHALL "path (/[^ ]+) \\(offset" _rpaths "${_out}")
    foreach(_m IN LISTS _rpaths)
        string(REGEX REPLACE "path (/[^ ]+) \\(offset" "\\1" _rpath "${_m}")
        message(STATUS "Удаляю rpath ${_rpath} из ${_bin}")
        execute_process(COMMAND install_name_tool -delete_rpath "${_rpath}" "${_bin}" ERROR_QUIET)
    endforeach()
endforeach()

# Докопируем .dylib, подключённые через @rpath, которые macdeployqt пропустил
# (зависимости второго уровня, например libbrotlicommon у libbrotlidec).
# Ищем их в BIRCH_LIB_DIRS; повторяем, пока что-то добавляется.
set(_changed TRUE)
while(_changed)
    set(_changed FALSE)
    _birch_macho_files(_files)
    foreach(_bin IN LISTS _files)
        execute_process(COMMAND otool -L "${_bin}" OUTPUT_VARIABLE _out ERROR_QUIET)
        string(REGEX MATCHALL "@rpath/[^/ ]+\\.dylib" _deps "${_out}")
        foreach(_dep IN LISTS _deps)
            string(REPLACE "@rpath/" "" _name "${_dep}")
            if(EXISTS "${_contents}/Frameworks/${_name}")
                continue()
            endif()
            foreach(_dir IN LISTS BIRCH_LIB_DIRS)
                if(EXISTS "${_dir}/${_name}")
                    file(REAL_PATH "${_dir}/${_name}" _src)
                    message(STATUS "Докопирую ${_name} (${_src})")
                    file(COPY_FILE "${_src}" "${_contents}/Frameworks/${_name}")
                    file(CHMOD "${_contents}/Frameworks/${_name}"
                         PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
                    execute_process(COMMAND install_name_tool -id "@rpath/${_name}"
                                            "${_contents}/Frameworks/${_name}" ERROR_QUIET)
                    set(_changed TRUE)
                    break()
                endif()
            endforeach()
        endforeach()
    endforeach()
endwhile()

# Проверка: каждая зависимость либо системная, либо лежит внутри бандла.
_birch_macho_files(_files)
set(_problems "")
foreach(_bin IN LISTS _files)
    execute_process(COMMAND otool -L "${_bin}" OUTPUT_VARIABLE _out ERROR_QUIET)
    string(REPLACE "\n" ";" _lines "${_out}")
    list(REMOVE_AT _lines 0)  # первая строка — имя самого файла
    foreach(_line IN LISTS _lines)
        string(STRIP "${_line}" _line)
        string(REGEX REPLACE " \\(compatibility.*" "" _dep "${_line}")
        if(_dep STREQUAL "" OR _dep MATCHES "^(/System/|/usr/lib/)")
            continue()
        endif()
        if(_dep MATCHES "^@(rpath|executable_path/\\.\\./Frameworks|loader_path/\\.\\./\\.\\./\\.\\.)/(.+)$")
            if(NOT EXISTS "${_contents}/Frameworks/${CMAKE_MATCH_2}")
                list(APPEND _problems "${_bin}: нет ${_dep}")
            endif()
        elseif(NOT _dep MATCHES "^@")
            list(APPEND _problems "${_bin}: ссылка за пределы бандла ${_dep}")
        endif()
    endforeach()
endforeach()
if(_problems)
    list(REMOVE_DUPLICATES _problems)
    string(REPLACE ";" "\n  " _msg "${_problems}")
    message(FATAL_ERROR "Бандл несамодостаточен:\n  ${_msg}")
endif()
message(STATUS "Проверка бандла: все зависимости внутри")
