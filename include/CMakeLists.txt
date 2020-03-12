
file (GLOB_RECURSE SOURCES_BASE "${CMAKE_CURRENT_LIST_DIR}/mapbox/*.hpp")
add_library(mapbox-base INTERFACE)
add_library(Mapbox::Base ALIAS mapbox-base)
target_include_directories(mapbox-base
    INTERFACE "${MAPBOX_BASE_EXTRAS}/expected-lite/include"
    )
