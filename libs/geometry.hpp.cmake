if(__MAPBOX_BASE_GEOMETRYHPP_CMAKE__)
    return()
endif()

set(__MAPBOX_BASE_GEOMETRYHPP_CMAKE__ TRUE)

add_library(mapbox-base-geometry-hpp INTERFACE)

target_include_directories(mapbox-base-geometry-hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/geometry.hpp/include
)
