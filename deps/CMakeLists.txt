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

execute_process(
    COMMAND git submodule update --init .
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps
)

mapbox_base_add_library(geojson-vt-cpp ${PROJECT_SOURCE_DIR}/deps/geojson-vt-cpp/include)
mapbox_base_add_library(geojson.hpp ${PROJECT_SOURCE_DIR}/deps/geojson.hpp/include)
mapbox_base_add_library(geometry.hpp ${PROJECT_SOURCE_DIR}/deps/geometry.hpp/include)
mapbox_base_add_library(jni.hpp ${PROJECT_SOURCE_DIR}/deps/jni.hpp/include)
mapbox_base_add_library(optional ${PROJECT_SOURCE_DIR}/deps/optional)
mapbox_base_add_library(pixelmatch-cpp ${PROJECT_SOURCE_DIR}/deps/pixelmatch-cpp/include)
mapbox_base_add_library(shelf-pack-cpp ${PROJECT_SOURCE_DIR}/deps/shelf-pack-cpp/include)
mapbox_base_add_library(supercluster.hpp ${PROJECT_SOURCE_DIR}/deps/supercluster.hpp/include)
mapbox_base_add_library(variant ${PROJECT_SOURCE_DIR}/deps/variant/include)
mapbox_base_add_library(cheap-ruler-cpp ${PROJECT_SOURCE_DIR}/deps/cheap-ruler-cpp/include)
