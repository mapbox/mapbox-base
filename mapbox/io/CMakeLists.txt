if(TARGET mapbox-base-io)
    return()
endif()

add_library(mapbox-base-io STATIC EXCLUDE_FROM_ALL ${CMAKE_CURRENT_LIST_DIR}/src/io.cpp)
add_library(Mapbox::Base::io ALIAS mapbox-base-io)

get_target_property(expected_include_dir Mapbox::Base::Extras::expected-lite INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(filesystem_include_dir Mapbox::Base::Extras::filesystem INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(mapbox-base-io SYSTEM PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${expected_include_dir}
    ${filesystem_include_dir}
)
