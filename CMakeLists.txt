cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(MAPBOX_BASE LANGUAGES CXX C)

enable_testing()

# Required to avoid warnings when setting visibility
cmake_policy(SET CMP0063 NEW)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set (MAPBOX_BASE_EXTRAS ${CMAKE_CURRENT_LIST_DIR}/extras)

if(TARGET mapbox-base)
    return()
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extras)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps)

if (BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif(BUILD_TESTING)
