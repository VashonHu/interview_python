cmake_minimum_required (VERSION 2.6)
project (nx)

file(GLOB SOURCES "src/*.c")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g -o2 -Wall -Werror -Wextra -pthread -DNDEBUG")
add_executable(nx ${SOURCES})
#add_subdirectory(tests)

project(conf)
set(conf_file ./src/util.c)
add_executable(conf ${conf_file})
