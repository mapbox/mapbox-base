if(TARGET mapbox-base-rapidjson)
    return()
endif()

execute_process(
    COMMAND git submodule update --init rapidjson
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-rapidjson INTERFACE)

target_include_directories(mapbox-base-rapidjson SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/rapidjson/include
)
