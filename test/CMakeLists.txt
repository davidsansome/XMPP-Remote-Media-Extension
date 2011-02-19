include_directories(${CMAKE_SOURCE_DIR}/libxrme)

add_executable(testserver server.cpp)
target_link_libraries(testserver xrme)


qt4_wrap_cpp(CLIENTMOC client.h)
add_executable(testclient client.cpp ${CLIENTMOC})
target_link_libraries(testclient xrme)
