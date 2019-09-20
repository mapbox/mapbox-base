# Needed for obtaining path to jni.h header.
get_property(JNI_HPP_INCLUDE_PATH TARGET Mapbox::Base::jni.hpp PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_filename_component(JNI_BASE_DIRECTORY_PATH ${JNI_HPP_INCLUDE_PATH} DIRECTORY)


add_executable(include-test-bundle
    ${CMAKE_CURRENT_LIST_DIR}/include.cpp
)

target_link_libraries(include-test-bundle PRIVATE
    Mapbox::Base
    Mapbox::Base::Extras
)


target_include_directories(include-test-bundle SYSTEM PRIVATE
    ${JNI_BASE_DIRECTORY_PATH}/test/android
)

add_executable(include-test-targets
    ${CMAKE_CURRENT_LIST_DIR}/include.cpp
)

target_link_libraries(include-test-targets PRIVATE
    Mapbox::Base::Extras::args
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
    Mapbox::Base::Extras::rapidjson
    Mapbox::Base::Extras::kdbush.hpp
    Mapbox::Base::geojson.hpp
    Mapbox::Base::geometry.hpp
    Mapbox::Base::jni.hpp
    Mapbox::Base::optional
    Mapbox::Base::pixelmatch-cpp
    Mapbox::Base::supercluster.hpp
    Mapbox::Base::variant
    Mapbox::Base::weak
    Mapbox::Base::typewrapper
    Mapbox::Base::value
)

target_include_directories(include-test-targets SYSTEM PRIVATE
    ${JNI_BASE_DIRECTORY_PATH}/test/android
)

add_test(NAME include-test-bundle COMMAND include-test-bundle)
add_test(NAME include-test-targets COMMAND include-test-targets)

add_executable(io-test
    ${CMAKE_CURRENT_LIST_DIR}/io.cpp
    ${CMAKE_CURRENT_LIST_DIR}/io_delete.cpp
)
add_executable(weak-test ${CMAKE_CURRENT_LIST_DIR}/weak.cpp)
add_executable(typewrapper-test ${CMAKE_CURRENT_LIST_DIR}/type_wrapper.cpp)
add_executable(value-test ${CMAKE_CURRENT_LIST_DIR}/value.cpp)

target_link_libraries(io-test PRIVATE
    Mapbox::Base::io
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
)

target_link_libraries(weak-test PRIVATE
    Mapbox::Base::weak
    pthread
)

target_link_libraries(typewrapper-test PRIVATE
    Mapbox::Base::typewrapper
)

target_link_libraries(value-test PRIVATE
    Mapbox::Base::value
)

add_test(NAME io-test COMMAND io-test)
add_test(NAME weak-test COMMAND weak-test)
add_test(NAME typewrapper-test COMMAND typewrapper-test)
add_test(NAME value-test COMMAND value-test)

add_definitions(-DTEST_FIXTURES_PATH="${CMAKE_CURRENT_LIST_DIR}/fixtures/")
add_definitions(-DTEST_BINARY_PATH="${CMAKE_CURRENT_BINARY_DIR}/")
