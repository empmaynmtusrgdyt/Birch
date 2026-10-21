# Установка (cmake --install) и сборка пакетов (cpack) для текущей ОС.
#
#   macOS:   Birch.app + .dmg                         — cmake/PackagingMacOS.cmake
#   Linux:   /usr (бинарник, .desktop, иконка, MIME) + .deb и .tar.gz
#                                                     — cmake/PackagingLinux.cmake
#            Arch: packaging/arch/PKGBUILD; AppImage: packaging/linux/build-appimage.sh
#   Windows: папка с Birch.exe и DLL + .zip и установщик — cmake/PackagingWindows.cmake

include(GNUInstallDirs)

set(BIRCH_HOMEPAGE "https://github.com/empmaynmtusrgdyt/Birch")
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" BIRCH_ARCH)
if(BIRCH_ARCH STREQUAL "amd64")
    set(BIRCH_ARCH "x86_64")
endif()

set(CPACK_PACKAGE_NAME "Birch")
set(CPACK_PACKAGE_VENDOR "Birch")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${BIRCH_HOMEPAGE}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Birch")
set(CPACK_STRIP_FILES ON)
# Значения переменных доходят до генераторов как есть (без лишнего разбора
# кавычек и обратных слешей) — важно для команд NSIS и описания .deb.
set(CPACK_VERBATIM_VARIABLES ON)

if(APPLE)
    include(PackagingMacOS)
elseif(WIN32)
    include(PackagingWindows OPTIONAL)
else()
    include(PackagingLinux)
endif()

include(CPack)
