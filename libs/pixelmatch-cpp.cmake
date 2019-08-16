if(TARGET mapbox-base-pixelmatch-cpp)
    return()
endif()

execute_process(
    COMMAND git submodule update --init pixelmatch-cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-pixelmatch-cpp INTERFACE)

target_include_directories(mapbox-base-pixelmatch-cpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/pixelmatch-cpp/include
)
