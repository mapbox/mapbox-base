function(add_mapbox_base_library name include_path)
    if(TARGET mapbox-base-${name})
        return()
    endif()

    execute_process(
        COMMAND git submodule update --init ${name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )

    add_library(mapbox-base-${name} INTERFACE)

    target_include_directories(mapbox-base-${name} SYSTEM INTERFACE
        ${include_path}
    )

    list(APPEND mapbox_base_libraries mapbox-base-${name})
    set (mapbox_base_libraries ${mapbox_base_libraries} PARENT_SCOPE)
endfunction()

add_mapbox_base_library(args ${CMAKE_CURRENT_LIST_DIR}/args)
add_mapbox_base_library(expected-lite ${CMAKE_CURRENT_LIST_DIR}/expected-lite/include)
add_mapbox_base_library(filesystem ${CMAKE_CURRENT_LIST_DIR}/filesystem/include)
add_mapbox_base_library(geometry-hpp ${CMAKE_CURRENT_LIST_DIR}/geometry.hpp/include)
add_mapbox_base_library(optional ${CMAKE_CURRENT_LIST_DIR}/optional)
add_mapbox_base_library(pixelmatch-cpp ${CMAKE_CURRENT_LIST_DIR}/pixelmatch-cpp/include)
add_mapbox_base_library(rapidjson ${CMAKE_CURRENT_LIST_DIR}/rapidjson/include)
add_mapbox_base_library(variant ${CMAKE_CURRENT_LIST_DIR}/variant/include)

add_library(mapbox-base INTERFACE)

target_link_libraries(mapbox-base INTERFACE
    ${mapbox_base_libraries}
)
