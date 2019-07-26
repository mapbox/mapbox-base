if(TARGET mapbox-base-variant)
    return()
endif()

execute_process(
    COMMAND git submodule update --init ${CMAKE_CURRENT_SOURCE_DIR}/variant
)

add_library(mapbox-base-variant INTERFACE)

target_include_directories(mapbox-base-variant SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/variant/include
)
