include_directories(${CMAKE_SOURCE_DIR}/libxrme)

set(SOURCES
  main.cpp
)

add_executable(test ${SOURCES})
target_link_libraries(test xmre)
