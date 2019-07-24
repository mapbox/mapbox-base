if(__MAPBOX_BASE_VARIANT_CMAKE__)
    return()
endif()

set(__MAPBOX_BASE_VARIANT_CMAKE__ TRUE)

add_library(mapbox-base-variant INTERFACE)

target_include_directories(mapbox-base-variant SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/variant/include
)
