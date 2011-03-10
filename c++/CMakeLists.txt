cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.5.0 REQUIRED QtCore QtGui)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLOOX REQUIRED gloox>=1.0)

include(${QT_USE_FILE})

add_subdirectory(libxrme)
add_subdirectory(test)


# Uninstall support
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
