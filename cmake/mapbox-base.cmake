include(${CMAKE_CURRENT_SOURCE_DIR}/libs/geometry.hpp.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/libs/variant.cmake)

execute_process(
    COMMAND git submodule update --init
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(mapbox-base INTERFACE)

target_link_libraries(mapbox-base INTERFACE
    mapbox-base-geometry-hpp
    mapbox-base-variant
)
