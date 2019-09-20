if(TARGET mapbox-base)
    return()
endif()

function(mapbox_base_add_library name include_path)
    if(TARGET mapbox-base-${name})
        return()
    endif()

    add_library(mapbox-base-${name} INTERFACE)
    add_library(Mapbox::Base::${name} ALIAS mapbox-base-${name})

    target_include_directories(mapbox-base-${name} SYSTEM INTERFACE
        ${include_path}
    )

    target_link_libraries(mapbox-base INTERFACE mapbox-base-${name})
endfunction()

add_library(mapbox-base INTERFACE)
add_library(Mapbox::Base ALIAS mapbox-base)

execute_process(
    COMMAND git submodule update --init
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

mapbox_base_add_library(geojson.hpp ${CMAKE_CURRENT_LIST_DIR}/geojson.hpp/include)
mapbox_base_add_library(geometry.hpp ${CMAKE_CURRENT_LIST_DIR}/geometry.hpp/include)
mapbox_base_add_library(jni.hpp ${CMAKE_CURRENT_LIST_DIR}/jni.hpp/include)
mapbox_base_add_library(optional ${CMAKE_CURRENT_LIST_DIR}/optional)
mapbox_base_add_library(pixelmatch-cpp ${CMAKE_CURRENT_LIST_DIR}/pixelmatch-cpp/include)
mapbox_base_add_library(supercluster.hpp ${CMAKE_CURRENT_LIST_DIR}/supercluster.hpp/include)
mapbox_base_add_library(variant ${CMAKE_CURRENT_LIST_DIR}/variant/include)
mapbox_base_add_library(weak ${CMAKE_CURRENT_LIST_DIR}/weak/include)
mapbox_base_add_library(typewrapper ${CMAKE_CURRENT_LIST_DIR}/typewrapper/include)
mapbox_base_add_library(value ${CMAKE_CURRENT_LIST_DIR}/value/include)
target_link_libraries(mapbox-base-value INTERFACE mapbox-base-geometry.hpp)
target_link_libraries(mapbox-base-value INTERFACE mapbox-base-variant)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/io)
