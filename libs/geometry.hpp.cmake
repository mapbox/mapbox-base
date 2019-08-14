if(TARGET mapbox-base-geometry-hpp)
    return()
endif()

execute_process(
    COMMAND git submodule update --init geometry.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_library(mapbox-base-geometry-hpp INTERFACE)

target_include_directories(mapbox-base-geometry-hpp SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/geometry.hpp/include
)
