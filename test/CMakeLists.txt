include_directories(${CMAKE_SOURCE_DIR}/libxrme)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(testserver server.cpp)
target_link_libraries(testserver xrme)


qt4_wrap_ui(CLIENTUIC clientmainwindow.ui)
qt4_wrap_cpp(CLIENTMOC clientmainwindow.h)
add_executable(testclient client.cpp clientmainwindow.cpp ${CLIENTMOC} ${CLIENTUIC})
target_link_libraries(testclient xrme)
