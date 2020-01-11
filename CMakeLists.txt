add_subdirectory(filesystem)
add_subdirectory(indexing)
add_subdirectory(parser)
add_subdirectory(recordmanager)
add_subdirectory(systemmanager)

add_definitions(-DDEBUG)
add_definitions("-Wall -g")

project(MyDB)

set(CMAKE_CXX_STANDARD 11)

add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE} ${HEADERS})
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} SqlParser ${CMAKE_PROJECT_NAME}_lib)
