project(gtk_program)

cmake_minimum_required(VERSION 3.15)

set(OUTPUT_NAME fltk-program)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ ${FL_DIR}/include/)

#add_library(HelloLibrary hello.cpp)

add_executable(${OUTPUT_NAME} main.cpp )

#target_link_libraries(${OUTPUT_NAME} ${FL_DIR}/lib/libfltk.a)
