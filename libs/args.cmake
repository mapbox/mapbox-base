if(TARGET mapbox-base-args)
    return()
endif()

execute_process(
    COMMAND git submodule update --init args
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-args INTERFACE)

target_include_directories(mapbox-base-args SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/args
)
