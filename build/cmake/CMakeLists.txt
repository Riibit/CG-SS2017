cmake_minimum_required(VERSION 2.8)

include("add_configuration.cmake")
include("add_subdirectory_if_exists.cmake")

project(cg1)

add_configuration(Submission Release)

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
	add_definitions(-DGLCORE_STATIC)
else ()
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif ()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../dependencies")

add_subdirectory(framework)
add_subdirectory_if_exists(task1a)
add_subdirectory_if_exists(task1b)
