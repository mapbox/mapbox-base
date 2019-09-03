if(TARGET mapbox-base-extras)
    return()
endif()

function(mapbox_base_extras_add_library name include_path)
    if(TARGET mapbox-base-extras-${name})
        return()
    endif()

    add_library(mapbox-base-extras-${name} INTERFACE)
    add_library(Mapbox::Base::Extras::${name} ALIAS mapbox-base-extras-${name})

    target_include_directories(mapbox-base-extras-${name} SYSTEM INTERFACE
        ${include_path}
    )

    target_link_libraries(mapbox-base-extras INTERFACE mapbox-base-extras-${name})
endfunction()

add_library(mapbox-base-extras INTERFACE)
add_library(Mapbox::Base::Extras ALIAS mapbox-base-extras)

execute_process(
    COMMAND git submodule update --init
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

mapbox_base_extras_add_library(args ${CMAKE_CURRENT_LIST_DIR}/args)
mapbox_base_extras_add_library(expected-lite ${CMAKE_CURRENT_LIST_DIR}/expected-lite/include)
mapbox_base_extras_add_library(filesystem ${CMAKE_CURRENT_LIST_DIR}/filesystem/include)
mapbox_base_extras_add_library(kdbush.hpp ${CMAKE_CURRENT_LIST_DIR}/kdbush.hpp/include)

include(${CMAKE_CURRENT_LIST_DIR}/rapidjson.cmake)
