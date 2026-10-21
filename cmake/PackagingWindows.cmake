# Windows: папка с Birch.exe и всеми DLL, .zip и установщик NSIS (cpack).
#
# Qt копирует windeployqt. Из Graphviz нужны только библиотеки ниже — это
# замыкание зависимостей gvc + плагинов core и dot_layout (проверено по таблицам
# импорта официальной сборки); остальные ~50 DLL (cairo, pango, Tcl…) не нужны.
# Среда выполнения MSVC кладётся рядом (app-local) через InstallRequiredSystemLibraries.

option(BIRCH_DEPLOY_QT "При установке копировать Qt и другие библиотеки рядом с Birch.exe" ON)

install(TARGETS Birch RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE DESTINATION . RENAME LICENSE.txt)
install(FILES LICENSE-EXCEPTION DESTINATION . RENAME LICENSE-EXCEPTION.txt)

if(BIRCH_DEPLOY_QT)
    qt_generate_deploy_app_script(
        TARGET Birch
        OUTPUT_SCRIPT _birch_deploy_script
        NO_TRANSLATIONS)
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
    include(InstallRequiredSystemLibraries)
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
set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-windows-x64")

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

# Файлы .birch открываются в Birch двойным щелчком
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS [=[
  WriteRegStr HKCR ".birch" "" "Birch.Flowchart"
  WriteRegStr HKCR "Birch.Flowchart" "" "Birch Flowchart"
  WriteRegStr HKCR "Birch.Flowchart\DefaultIcon" "" "$INSTDIR\bin\Birch.exe,0"
  WriteRegStr HKCR "Birch.Flowchart\shell\open\command" "" '"$INSTDIR\bin\Birch.exe" "%1"'
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, p 0, p 0)'
]=])
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS [=[
  DeleteRegKey HKCR "Birch.Flowchart"
  DeleteRegKey HKCR ".birch"
  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, p 0, p 0)'
]=])
