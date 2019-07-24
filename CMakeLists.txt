cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(MAPBOX_BASE LANGUAGES CXX C)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mapbox-base.cmake)

option(MBBASE_WITH_TESTS "Build tests" OFF)

if(MBBASE_WITH_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/test.cmake)
endif()
