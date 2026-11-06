# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Windows: папка с Birch.exe и всеми DLL, .zip и установщик NSIS (cpack).
#
# Qt копирует windeployqt. Из Graphviz нужны только библиотеки ниже — это
# замыкание зависимостей gvc + плагинов core и dot_layout (проверено по таблицам
# импорта официальной сборки); остальные ~50 DLL (cairo, pango, Tcl…) не нужны.
# Среда выполнения MSVC кладётся рядом (app-local) через InstallRequiredSystemLibraries.

option(BIRCH_DEPLOY_QT "При установке копировать Qt и другие библиотеки рядом с Birch.exe" ON)

# Пакет для Windows 8.1: папка с распакованным архивом qt6windows7 той же версии
# Qt. Его библиотеки заменяют официальные (packaging/windows/replace-qt.cmake),
# а универсальная среда выполнения C (UCRT) кладётся в пакет — в Windows 8.1
# её нет без обновления KB2999226.
set(BIRCH_QT_WINDOWS8_DIR "" CACHE PATH "Qt с исправлениями qt6windows7 — собрать пакет для Windows 8.1")

install(TARGETS Birch RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
# Командная строка: birch.com в отдельной папке cmd — в PATH попадает только
# она, а не DLL из bin. birch-path.ps1 добавляет её в PATH (установщик или вручную).
install(TARGETS birch_console RUNTIME DESTINATION cmd)
install(FILES packaging/windows/birch-path.ps1 DESTINATION .)
install(FILES LICENSE DESTINATION . RENAME LICENSE.txt)
install(FILES THIRD-PARTY-NOTICES.txt DESTINATION .)
install(DIRECTORY licenses DESTINATION .)  # Qt (LGPL) и Graphviz (EPL) входят в пакет

if(BIRCH_DEPLOY_QT)
    # Только нужное: Birch рисует виджетами без OpenGL и не работает с сетью, а из
    # форматов картинок ему нужен SVG (PNG встроен в QtGui). Без этих ключей
    # windeployqt кладёт программный OpenGL (Mesa, ~20 МБ), компилятор шейдеров
    # D3D, Qt6Network с плагинами TLS и сетевого статуса и плагины GIF/ICO/JPEG.
    qt_generate_deploy_app_script(
        TARGET Birch
        OUTPUT_SCRIPT _birch_deploy_script
        NO_TRANSLATIONS
        DEPLOY_TOOL_OPTIONS
            --no-opengl-sw --no-system-d3d-compiler --no-system-dxc-compiler
            --skip-plugin-types generic,networkinformation,tls
            --exclude-plugins qgif,qico,qjpeg)
    install(SCRIPT ${_birch_deploy_script})

    get_filename_component(_birch_gv_libdir "${Graphviz_gvc_LIBRARY}" DIRECTORY)
    get_filename_component(_birch_gv_bindir "${_birch_gv_libdir}/../bin" ABSOLUTE)
    set(_birch_gv_dlls gvc cgraph cdt pathplan xdot gvplugin_core gvplugin_dot_layout libexpat zlib1)
    foreach(_dll IN LISTS _birch_gv_dlls)
        if(NOT EXISTS "${_birch_gv_bindir}/${_dll}.dll")
            message(FATAL_ERROR "Не найдена ${_birch_gv_bindir}/${_dll}.dll (задайте GRAPHVIZ_ROOT)")
        endif()
        install(FILES "${_birch_gv_bindir}/${_dll}.dll" DESTINATION ${CMAKE_INSTALL_BINDIR})
    endforeach()

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})
    if(BIRCH_QT_WINDOWS8_DIR)
        set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    endif()
    include(InstallRequiredSystemLibraries)

    if(BIRCH_QT_WINDOWS8_DIR)
        file(TO_CMAKE_PATH "${BIRCH_QT_WINDOWS8_DIR}" _birch_qt8_dir)  # «\» в пути — не escape-последовательности
        if(NOT EXISTS "${_birch_qt8_dir}/Qt6Core.dll")
            message(FATAL_ERROR "В BIRCH_QT_WINDOWS8_DIR нет Qt6Core.dll: ${BIRCH_QT_WINDOWS8_DIR}")
        endif()
        # После windeployqt: install(SCRIPT) выше выполняется раньше
        install(CODE "set(BIRCH_QT_WINDOWS8_DIR \"${_birch_qt8_dir}\")
                      set(BIRCH_PACKAGE_ROOT \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}\")
                      include(\"${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/replace-qt.cmake\")")
    endif()
endif()

# ---------------------------------------------------------------------------
# CPack: .zip всегда, установщик — если найден NSIS (makensis)
# ---------------------------------------------------------------------------
set(CPACK_GENERATOR "ZIP")
set(_birch_pf86 "ProgramFiles(x86)")
find_program(BIRCH_MAKENSIS makensis PATHS "$ENV{${_birch_pf86}}/NSIS" "$ENV{ProgramFiles}/NSIS")
if(BIRCH_MAKENSIS)
    list(APPEND CPACK_GENERATOR "NSIS")
endif()
if(BIRCH_QT_WINDOWS8_DIR)
    set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-windows8-x64")
else()
    set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-windows-x64")
endif()

set(CPACK_PACKAGE_EXECUTABLES "Birch" "Birch")  # ярлык в меню «Пуск»
set(CPACK_CREATE_DESKTOP_LINKS "Birch")
set(CPACK_NSIS_DISPLAY_NAME "Birch")
set(CPACK_NSIS_PACKAGE_NAME "Birch")
set(CPACK_NSIS_URL_INFO_ABOUT "${BIRCH_HOMEPAGE}")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/app.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows/app.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\Birch.exe")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "Birch.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")  # страница лицензии в установщике

# Файлы .birch открываются в Birch двойным щелчком; папка cmd — в PATH для всех
# пользователей, чтобы команда birch сразу работала в cmd и PowerShell.
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS [=[
  nsExec::ExecToLog 'powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "$INSTDIR\birch-path.ps1" -Scope Machine'
  Pop $0
  WriteRegStr HKCR ".birch" "" "Birch.Flowchart"
  WriteRegStr HKCR "Birch.Flowchart" "" "Birch Flowchart"
  WriteRegStr HKCR "Birch.Flowchart\DefaultIcon" "" "$INSTDIR\bin\Birch.exe,0"
  WriteRegStr HKCR "Birch.Flowchart\shell\open\command" "" '"$INSTDIR\bin\Birch.exe" "%1"'
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, p 0, p 0)'
]=])
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS [=[
  nsExec::ExecToLog 'powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "$INSTDIR\birch-path.ps1" -Remove -Scope Machine'
  Pop $0
  DeleteRegKey HKCR "Birch.Flowchart"
  DeleteRegKey HKCR ".birch"
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, p 0, p 0)'
]=])
