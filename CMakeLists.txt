cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(MAPBOX_BASE LANGUAGES CXX C)

# Required to avoid warnings when setting visibility
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
