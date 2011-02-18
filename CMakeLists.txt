cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.5.0 REQUIRED QtCore)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLOOX REQUIRED gloox>=0.9)

include(${QT_USE_FILE})

add_subdirectory(libxrme)
add_subdirectory(test)

