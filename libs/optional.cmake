if(TARGET mapbox-base-optional)
    return()
endif()

execute_process(
    COMMAND git submodule update --init optional
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-optional INTERFACE)

target_include_directories(mapbox-base-optional SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/optional
)
