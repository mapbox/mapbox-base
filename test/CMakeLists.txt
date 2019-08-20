add_executable(include-test-bundle ${CMAKE_CURRENT_SOURCE_DIR}/include.cpp)

target_link_libraries(include-test-bundle PRIVATE
    Mapbox::Base
    Mapbox::Base::Extras
)

add_executable(include-test-targets ${CMAKE_CURRENT_SOURCE_DIR}/include.cpp)

target_link_libraries(include-test-targets PRIVATE
    Mapbox::Base::Extras::args
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
    Mapbox::Base::Extras::rapidjson
    Mapbox::Base::geometry.hpp
    Mapbox::Base::optional
    Mapbox::Base::pixelmatch-cpp
    Mapbox::Base::variant
)

add_test(NAME include-test-bundle COMMAND include-test-bundle)
add_test(NAME include-test-targets COMMAND include-test-targets)

add_executable(io-test ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp)

target_link_libraries(io-test PRIVATE
    Mapbox::Base::io
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
)

add_test(NAME io-test COMMAND io-test)

add_definitions(-DTEST_FIXTURES_PATH="${CMAKE_CURRENT_LIST_DIR}/fixtures/")
add_definitions(-DTEST_BINARY_PATH="${CMAKE_CURRENT_BINARY_DIR}/")
