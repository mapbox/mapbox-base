if(TARGET mapbox-base-expected-lite)
    return()
endif()

execute_process(
    COMMAND git submodule update --init expected-lite
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-expected-lite INTERFACE)

target_include_directories(mapbox-base-expected-lite SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/expected-lite/include
)
