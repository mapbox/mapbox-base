add_executable(include-tests-bundle ${CMAKE_CURRENT_SOURCE_DIR}/include.cpp)

target_link_libraries(include-tests-bundle PRIVATE
    Mapbox::Base
    Mapbox::Base::Extras
)

add_executable(include-tests-targets ${CMAKE_CURRENT_SOURCE_DIR}/include.cpp)

target_link_libraries(include-tests-targets PRIVATE
    Mapbox::Base::Extras::args
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::filesystem
    Mapbox::Base::Extras::rapidjson
    Mapbox::Base::geometry.hpp
    Mapbox::Base::optional
    Mapbox::Base::pixelmatch-cpp
    Mapbox::Base::variant
)

add_test(NAME include-tests-bundle COMMAND include-tests-bundle)
add_test(NAME include-tests-targets COMMAND include-tests-targets)
