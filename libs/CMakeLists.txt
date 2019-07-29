include(${CMAKE_CURRENT_SOURCE_DIR}/geometry.hpp.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/optional.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/variant.cmake)

add_library(mapbox-base INTERFACE)

target_link_libraries(mapbox-base INTERFACE
    mapbox-base-geometry-hpp
    mapbox-base-optional
    mapbox-base-variant
)
