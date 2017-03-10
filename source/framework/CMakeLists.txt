project(cg1framework)

file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/dependencies ${CMAKE_SOURCE_DIR}/dependencies/cg1framework)
add_library(cg1framework STATIC ${HEADERS} ${SOURCES})
