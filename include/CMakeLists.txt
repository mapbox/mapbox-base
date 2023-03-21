add_library(mapbox-base INTERFACE)
add_library(Mapbox::Base ALIAS mapbox-base)

target_include_directories(mapbox-base SYSTEM INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/mapbox/polyfills"
)

target_link_libraries(mapbox-base INTERFACE
    Mapbox::Base::Extras::expected-lite
)
