if(TARGET mapbox-base-io)
    return()
endif()

add_library(mapbox-base-io STATIC ${CMAKE_CURRENT_LIST_DIR}/src/io.cpp)
add_library(Mapbox::Base::io ALIAS mapbox-base-io)

target_include_directories(mapbox-base-io SYSTEM PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(mapbox-base-io PRIVATE
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
)
