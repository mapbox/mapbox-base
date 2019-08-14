if(TARGET mapbox-base-variant)
    return()
endif()

execute_process(
    COMMAND git submodule update --init variant
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-variant INTERFACE)

target_include_directories(mapbox-base-variant SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/variant/include
)
