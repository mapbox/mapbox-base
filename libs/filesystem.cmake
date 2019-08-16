if(TARGET mapbox-base-filesystem)
    return()
endif()

execute_process(
    COMMAND git submodule update --init filesystem
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-filesystem INTERFACE)

target_include_directories(mapbox-base-filesystem SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/filesystem/include
)
