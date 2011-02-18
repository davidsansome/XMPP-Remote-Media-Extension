include_directories(${GLOOX_INCLUDE_DIRS})

link_directories(${GLOOX_LIBRARY_DIRS})

set(SOURCES
  connection.cpp
)

set(HEADERS
  connection.h
)

qt4_wrap_cpp(MOC ${HEADERS})

add_library(xmre ${SOURCES} ${MOC})
target_link_libraries(xmre
  ${QT_LIBRARIES}
  ${GLOOX_LIBRARIES}
)
