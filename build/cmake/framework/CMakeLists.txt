cmake_minimum_required(VERSION 2.8)

project(framework)

set(CMAKE_DEBUG_POSTFIX "D")

set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies")

add_definitions(-DZ_SOLO)
add_subdirectory(zlib zlib)
add_subdirectory(libpng libpng)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../source/framework")

set(Framework_INCLUDE_DIRS_internal
	${ZLIB_INCLUDE_DIR}
	${LPNG_INCLUDE_DIR}
	"${SOURCE_DIR}/"
	"${SOURCE_DIR}/../"
)

include_directories(${Framework_INCLUDE_DIRS_internal} ${LPNG_INCLUDE_DIR})

add_subdirectory_if_exists(utils)
add_subdirectory_if_exists(instrumentation)

file(GLOB FRAMEWORK_SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/math/*.cpp" "${SOURCE_DIR}/math/*.h")

add_library(framework ${FRAMEWORK_PLATFORM_SOURCES} ${FRAMEWORK_SOURCES})

if (WIN32)
	set(Framework_INCLUDE_DIRS ${Framework_INCLUDE_DIRS_internal} PARENT_SCOPE)
	set(Framework_LIBRARIES framework ${LPNG_LIBRARY} ${ZLIB_LIBRARY} PARENT_SCOPE)
else ()
	set(Framework_INCLUDE_DIRS ${Framework_INCLUDE_DIRS_internal} PARENT_SCOPE)
	set(Framework_LIBRARIES framework ${LPNG_LIBRARY} ${ZLIB_LIBRARY} ${CMAKE_DL_LIBS} PARENT_SCOPE)
endif ()
