include_directories(${GLOOX_INCLUDE_DIRS})

link_directories(${GLOOX_LIBRARY_DIRS})

set(SOURCES
  common.cpp
  connection.cpp
  handler.cpp
  mediaplayerhandler.cpp
  mediaplayerinterface.cpp
  remotecontrolhandler.cpp
  remotecontrolinterface.cpp
)

set(HEADERS
  connection.h
)

qt4_wrap_cpp(MOC ${HEADERS})

add_library(xrme ${SOURCES} ${MOC})
target_link_libraries(xrme
  ${QT_LIBRARIES}
  ${GLOOX_LIBRARIES}
)
