# Установка (cmake --install) и сборка пакетов (cpack).
#
#   macOS:   Birch.app + .dmg (DragNDrop); Qt и Graphviz кладутся внутрь бандла
#   Windows: папка с Birch.exe и всеми DLL + .zip; установщик NSIS, если найден makensis
#   Linux:   стандартная раскладка /usr (бинарник, .desktop, иконка, MIME) + .tar.gz;
#            .deb, если есть dpkg-shlibdeps. Переносимый AppImage —
#            packaging/linux/build-appimage.sh

include(GNUInstallDirs)

if(APPLE OR WIN32)
    set(_birch_deploy_default ON)
else()
    set(_birch_deploy_default OFF)  # в Linux библиотеки берутся из системы (или AppImage)
endif()
option(BIRCH_DEPLOY_QT "При установке копировать Qt и другие библиотеки рядом с приложением"
       ${_birch_deploy_default})

set(BIRCH_MAINTAINER "Birch Maintainers <birch@example.com>" CACHE STRING "Контакт для пакетов (.deb)")

# ---------------------------------------------------------------------------
# Установка приложения
# ---------------------------------------------------------------------------
if(WIN32 AND BIRCH_DEPLOY_QT)
    # Qt копирует windeployqt, а DLL Graphviz и его зависимостей (expat, zlib, ltdl…)
    # находим через анализ импорта Birch.exe.
    set(_birch_dll_dirs "")
    if(DEFINED VCPKG_INSTALLED_DIR AND DEFINED VCPKG_TARGET_TRIPLET)
        list(APPEND _birch_dll_dirs "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
    endif()
    get_filename_component(_birch_gv_libdir "${Graphviz_gvc_LIBRARY}" DIRECTORY)
    list(APPEND _birch_dll_dirs "${_birch_gv_libdir}/../bin")

    install(TARGETS Birch
        RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "^api-ms-" "^ext-ms-" "^[Qq]t6"
            POST_EXCLUDE_REGEXES "[Ss]ystem32/"
            DIRECTORIES ${_birch_dll_dirs}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Если плагины не слинкованы напрямую, Graphviz загрузит их сам по файлу config*.
    if(NOT Graphviz_PLUGINS_FOUND)
        foreach(_dir IN LISTS _birch_dll_dirs)
            file(GLOB _birch_gv_plugins "${_dir}/gvplugin_*.dll" "${_dir}/config[0-9]*")
            install(FILES ${_birch_gv_plugins} DESTINATION ${CMAKE_INSTALL_BINDIR})
        endforeach()
    endif()
else()
    install(TARGETS Birch
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(UNIX AND NOT APPLE)
    install(FILES packaging/linux/birch.desktop
            DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install(FILES resources/icons/app.svg
            DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
            RENAME birch.svg)
    install(FILES packaging/linux/birch-mime.xml
            DESTINATION ${CMAKE_INSTALL_DATADIR}/mime/packages
            RENAME birch.xml)
endif()

# ---------------------------------------------------------------------------
# Деплой Qt (macdeployqt / windeployqt / встроенный деплой Qt для Linux)
# ---------------------------------------------------------------------------
if(BIRCH_DEPLOY_QT)
    if(APPLE)
        # macdeployqt копирует только зависимости, которые он смог разрешить, а
        # @rpath-зависимости второго уровня (QtDBus у QtGui в Qt из Homebrew)
        # пропускает. Явная линковка делает QtDBus прямой зависимостью Birch.
        find_package(Qt6 QUIET COMPONENTS DBus)
        if(TARGET Qt6::DBus)
            target_link_libraries(Birch PRIVATE Qt6::DBus)
        endif()

        # macdeployqt ищет зависимости фреймворков Qt, подключённые через @rpath
        # (например, QtDBus у QtGui из Homebrew), по rpath самого бинарника, а CMake
        # при установке его стирает. Оставляем путь к Qt; после деплоя
        # prune-bundle.cmake удалит все абсолютные rpath из бандла.
        set_target_properties(Birch PROPERTIES INSTALL_RPATH "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS}")
    endif()
    qt_generate_deploy_app_script(
        TARGET Birch
        OUTPUT_SCRIPT _birch_deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${_birch_deploy_script})

    if(APPLE)
        # macdeployqt переписывает пути в бинарниках, и подпись становится недействительной.
        # На Apple Silicon неподписанный код не запускается — подписываем ad-hoc.
        # Для распространения замените "-" на "Developer ID Application: …" и нотаризуйте.
        # Где искать библиотеки, которые macdeployqt пропустил
        get_filename_component(_birch_gv_libdir "${Graphviz_gvc_LIBRARY}" DIRECTORY)
        set(_birch_lib_dirs "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS}" "${_birch_gv_libdir}")
        foreach(_p IN LISTS CMAKE_PREFIX_PATH)
            list(APPEND _birch_lib_dirs "${_p}/lib")
        endforeach()
        install(CODE "set(_birch_prune_script \"${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/prune-bundle.cmake\")
                      set(BIRCH_LIB_DIRS \"${_birch_lib_dirs}\")")
        install(CODE [[
            set(_app "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Birch.app")
            set(BIRCH_APP "${_app}")
            include("${_birch_prune_script}")
            message(STATUS "Ad-hoc подпись: ${_app}")
            execute_process(COMMAND codesign --force --deep --sign - "${_app}"
                            RESULT_VARIABLE _rc)
            if(NOT _rc EQUAL 0)
                message(FATAL_ERROR "codesign завершился с ошибкой ${_rc}")
            endif()
        ]])
    endif()
endif()

# ---------------------------------------------------------------------------
# CPack
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "Birch")
set(CPACK_PACKAGE_VENDOR "Birch")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Birch")
set(CPACK_PACKAGE_CONTACT "${BIRCH_MAINTAINER}")
set(CPACK_STRIP_FILES ON)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _birch_arch)

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "Birch")
    set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-macos-${_birch_arch}")
elseif(WIN32)
    set(CPACK_GENERATOR "ZIP")
    find_program(BIRCH_MAKENSIS makensis)
    if(BIRCH_MAKENSIS)
        list(APPEND CPACK_GENERATOR "NSIS")
    endif()
    set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-windows-x64")
    set(CPACK_PACKAGE_EXECUTABLES "Birch" "Birch")  # ярлык в меню «Пуск»
    set(CPACK_CREATE_DESKTOP_LINKS "Birch")
    set(CPACK_NSIS_DISPLAY_NAME "Birch")
    set(CPACK_NSIS_PACKAGE_NAME "Birch")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/app.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/app.ico")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Birch.exe")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "Birch.exe")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
else()
    set(CPACK_GENERATOR "TGZ")
    find_program(BIRCH_DPKG_SHLIBDEPS dpkg-shlibdeps)
    if(BIRCH_DPKG_SHLIBDEPS)
        list(APPEND CPACK_GENERATOR "DEB")
    endif()
    set(CPACK_PACKAGE_FILE_NAME "birch-${PROJECT_VERSION}-linux-${_birch_arch}")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
    set(CPACK_DEBIAN_PACKAGE_NAME "birch")
    set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
    set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)  # зависимости (Qt, Graphviz) — автоматически
endif()

include(CPack)
