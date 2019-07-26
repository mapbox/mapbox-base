cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(MAPBOX_BASE LANGUAGES CXX C)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
