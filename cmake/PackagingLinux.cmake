# Linux: стандартная раскладка /usr и пакеты .deb / .tar.gz (cpack).
#
# Библиотеки берутся из системы. Для .deb зависимости вычисляет dpkg-shlibdeps,
# но он не видит то, что загружается динамически: платформенные плагины Qt
# (xcb, wayland) и плагины раскладки Graphviz с их реестром (пакет graphviz;
# в Ubuntu 26.04 без него «Layout type: "dot" not recognized»). Добавляем явно.
# Самодостаточный AppImage собирается отдельно: packaging/linux/build-appimage.sh.

install(TARGETS Birch RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES packaging/linux/birch.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES resources/icons/app.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
        RENAME birch.svg)
install(FILES packaging/linux/birch-mime.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/mime/packages
        RENAME birch.xml)

set(BIRCH_MAINTAINER "Birch Maintainers <birch@users.noreply.github.com>"
    CACHE STRING "Контакт для пакета .deb")

set(CPACK_GENERATOR "TGZ")
find_program(BIRCH_DPKG_SHLIBDEPS dpkg-shlibdeps)
if(BIRCH_DPKG_SHLIBDEPS)
    list(APPEND CPACK_GENERATOR "DEB")
endif()
set(CPACK_PACKAGE_FILE_NAME "birch-${PROJECT_VERSION}-linux-${BIRCH_ARCH}")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

set(CPACK_DEBIAN_PACKAGE_NAME "birch")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${BIRCH_MAINTAINER}")
set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
    "Блок-схемы из текста\n Строит блок-схемы по простому текстовому описанию с помощью Graphviz.\n Экспорт в PNG и SVG, копирование картинки в буфер обмена.")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${BIRCH_HOMEPAGE}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-qpa-plugins, graphviz")
