# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Собирает лицензии библиотек, которые лежат в Birch.app.
#
# Для каждой библиотеки и фреймворка в Contents/Frameworks находит пакет
# Homebrew, из которого она взята, копирует его файлы лицензий (LICENSE,
# COPYING, NOTICE…) в Contents/Resources/licenses/homebrew/<пакет> и пишет
# сводку Contents/Resources/licenses/LIBRARIES.txt: пакет, версия, лицензия
# (SPDX из brew info) и какие библиотеки из него взяты.
#
# Если в пакете Homebrew нет файла лицензии (так у Qt и GLib), берётся
# стандартный текст из папки licenses репозитория по SPDX-имени лицензии.
# Если и так не нашлось — упаковка завершается ошибкой: в бандле не должно
# быть библиотек без лицензии.
#
# Ожидает переменные BIRCH_APP и BIRCH_SOURCE_DIR.

set(_contents "${BIRCH_APP}/Contents")
set(_out "${_contents}/Resources/licenses")

find_program(_brew brew HINTS /opt/homebrew/bin /usr/local/bin)
if(NOT _brew)
    message(FATAL_ERROR "Лицензии библиотек: не найден brew — бандл собирается из пакетов Homebrew")
endif()
execute_process(COMMAND "${_brew}" --prefix OUTPUT_VARIABLE _hb OUTPUT_STRIP_TRAILING_WHITESPACE)

# Стандартные тексты для пакетов без файла лицензии: SPDX-имя → файл в licenses/
# (LGPL дополняет GPL — к LGPL-3.0 нужны оба текста). Порядок важен: LGPL раньше GPL.
set(_known_texts
    "LGPL-3.0=LGPL-3.0.txt, GPL-3.0.txt"
    "LGPL-2.1=LGPL-2.1.txt"
    "GPL-3.0=GPL-3.0.txt"
    "EPL-2.0=EPL-2.0.txt")

# --- откуда взята каждая библиотека ------------------------------------------
file(GLOB _items LIST_DIRECTORIES true "${_contents}/Frameworks/lib*.dylib" "${_contents}/Frameworks/Qt*.framework")
file(GLOB _opt_libs LIST_DIRECTORIES true "${_hb}/opt/*/lib")
set(_formulas "")
foreach(_item IN LISTS _items)
    get_filename_component(_name "${_item}" NAME)
    set(_source "")
    foreach(_dir IN ITEMS "${_hb}/lib" "${_hb}/lib/graphviz" ${_opt_libs})
        if(EXISTS "${_dir}/${_name}")
            file(REAL_PATH "${_dir}/${_name}" _source)
            break()
        endif()
    endforeach()
    if(NOT _source MATCHES "^${_hb}/Cellar/([^/]+)/([^/]+)/")
        message(FATAL_ERROR "Лицензии библиотек: не нашёл пакет Homebrew для ${_name}")
    endif()
    set(_f "${CMAKE_MATCH_1}")
    set(_keg_${_f} "${_hb}/Cellar/${CMAKE_MATCH_1}/${CMAKE_MATCH_2}")
    set(_version_${_f} "${CMAKE_MATCH_2}")
    list(APPEND _libs_${_f} "${_name}")
    list(APPEND _formulas "${_f}")
endforeach()
list(REMOVE_DUPLICATES _formulas)
list(SORT _formulas)

# --- лицензии пакетов по данным Homebrew -------------------------------------
execute_process(COMMAND "${_brew}" info --json=v2 ${_formulas}
                OUTPUT_VARIABLE _json RESULT_VARIABLE _rc ERROR_QUIET)
if(NOT _rc EQUAL 0)
    message(FATAL_ERROR "Лицензии библиотек: brew info завершился с ошибкой ${_rc}")
endif()
string(JSON _count LENGTH "${_json}" formulae)
math(EXPR _last "${_count} - 1")
foreach(_i RANGE ${_last})
    string(JSON _name GET "${_json}" formulae ${_i} name)
    string(JSON _license ERROR_VARIABLE _err GET "${_json}" formulae ${_i} license)
    if(_err OR _license STREQUAL "" OR _license STREQUAL "null")
        set(_license "не указана в Homebrew — см. файлы лицензии")
    endif()
    set(_license_${_name} "${_license}")
endforeach()

# --- копируем тексты и пишем сводку ------------------------------------------
file(REMOVE_RECURSE "${_out}/homebrew")
set(_summary "Библиотеки внутри Birch.app и их лицензии
=========================================

Список собран автоматически при упаковке: для каждой библиотеки указан пакет
Homebrew, из которого она взята, его версия и лицензия. Тексты лицензий лежат
рядом, в папке homebrew/<пакет>; если пакет Homebrew их не содержит — в общих
файлах этой папки. Исходный код пакетов: https://formulae.brew.sh/formula/<пакет>
")
foreach(_f IN LISTS _formulas)
    file(GLOB _top LIST_DIRECTORIES false RELATIVE "${_keg_${_f}}" "${_keg_${_f}}/*")
    set(_texts "")
    foreach(_file IN LISTS _top)
        string(TOUPPER "${_file}" _upper)
        if(_upper MATCHES "^(LICEN[CS]E|COPYING|NOTICE|COPYRIGHT)")
            file(COPY "${_keg_${_f}}/${_file}" DESTINATION "${_out}/homebrew/${_f}")
            list(APPEND _texts "homebrew/${_f}/${_file}")
        endif()
    endforeach()
    if(NOT _texts)
        foreach(_entry IN LISTS _known_texts)
            # if(MATCHES) перезаписывает CMAKE_MATCH_*, поэтому сначала сохраняем
            string(REGEX MATCH "^([^=]+)=(.+)$" _ "${_entry}")
            set(_spdx "${CMAKE_MATCH_1}")
            set(_files "${CMAKE_MATCH_2}")
            if(_license_${_f} MATCHES "${_spdx}")
                list(APPEND _texts "${_files}")
                break()
            endif()
        endforeach()
    endif()
    if(NOT _texts)
        message(FATAL_ERROR "Лицензии библиотек: у пакета ${_f} (${_license_${_f}}) нет файла лицензии")
    endif()
    list(JOIN _libs_${_f} ", " _libs)
    list(JOIN _texts ", " _texts)
    string(APPEND _summary "
${_f} ${_version_${_f}}
  Лицензия: ${_license_${_f}}
  Текст: ${_texts}
  Библиотеки: ${_libs}
")
endforeach()
file(WRITE "${_out}/LIBRARIES.txt" "${_summary}")
list(LENGTH _formulas _n)
message(STATUS "Лицензии библиотек: ${_n} пакетов Homebrew → Resources/licenses/LIBRARIES.txt")
