# Установка (cmake --install) и сборка .dmg (cpack).
#
# При установке macdeployqt кладёт Qt и все библиотеки (включая Graphviz) внутрь
# Birch.app, затем packaging/macos/prune-bundle.cmake убирает лишнее, докопирует
# пропущенное и проверяет, что бандл самодостаточен. В конце — ad-hoc подпись.

option(BIRCH_DEPLOY_QT "При установке копировать Qt и другие библиотеки внутрь Birch.app" ON)

install(TARGETS Birch BUNDLE DESTINATION .)

if(BIRCH_DEPLOY_QT)
    # macdeployqt копирует только зависимости, которые он смог разрешить, а
    # @rpath-зависимости второго уровня (QtDBus у QtGui в Qt из Homebrew)
    # пропускает. Явная линковка делает QtDBus прямой зависимостью Birch.
    find_package(Qt6 QUIET COMPONENTS DBus)
    if(TARGET Qt6::DBus)
        target_link_libraries(Birch PRIVATE Qt6::DBus)
    endif()

    # macdeployqt ищет зависимости фреймворков Qt, подключённые через @rpath,
    # по rpath самого бинарника, а CMake при установке его стирает. Оставляем путь
    # к Qt; после деплоя prune-bundle.cmake удалит все абсолютные rpath из бандла.
    set_target_properties(Birch PROPERTIES INSTALL_RPATH "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS}")

    qt_generate_deploy_app_script(
        TARGET Birch
        OUTPUT_SCRIPT _birch_deploy_script)
    install(SCRIPT ${_birch_deploy_script})

    # Где искать библиотеки, которые macdeployqt пропустил
    get_filename_component(_birch_gv_libdir "${Graphviz_gvc_LIBRARY}" DIRECTORY)
    set(_birch_lib_dirs "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS}" "${_birch_gv_libdir}")
    foreach(_p IN LISTS CMAKE_PREFIX_PATH)
        list(APPEND _birch_lib_dirs "${_p}/lib")
    endforeach()
    install(CODE "set(_birch_prune_script \"${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/prune-bundle.cmake\")
                  set(BIRCH_LIB_DIRS \"${_birch_lib_dirs}\")")

    # macdeployqt переписывает пути в бинарниках, и подпись становится недействительной.
    # На Apple Silicon неподписанный код не запускается — подписываем ad-hoc.
    # Для распространения замените "-" на "Developer ID Application: …" и нотаризуйте.
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

# ---------------------------------------------------------------------------
# CPack: .dmg с Birch.app и ярлыком на «Программы»
# ---------------------------------------------------------------------------
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _birch_arch)
set(CPACK_GENERATOR "DragNDrop")
set(CPACK_PACKAGE_NAME "Birch")
set(CPACK_PACKAGE_VENDOR "Birch")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_FILE_NAME "Birch-${PROJECT_VERSION}-macos-${_birch_arch}")
set(CPACK_DMG_VOLUME_NAME "Birch")
set(CPACK_STRIP_FILES ON)

include(CPack)
