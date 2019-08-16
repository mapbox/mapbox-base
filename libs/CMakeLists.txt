include(${CMAKE_CURRENT_SOURCE_DIR}/args.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/expected-lite.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/filesystem.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/geometry.hpp.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/optional.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/variant.cmake)

add_library(mapbox-base INTERFACE)

target_link_libraries(mapbox-base INTERFACE
    mapbox-base-args
    mapbox-base-expected-lite
    mapbox-base-filesystem
    mapbox-base-geometry-hpp
    mapbox-base-optional
    mapbox-base-variant
)
