make_minimum_required(VERSION 3.14)

set(SOURCES
  main.cpp
  display.cpp
  buck_converter.cpp
  )

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(SOURCES ${SOURCES} PARENT_SCOPE)
