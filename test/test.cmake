add_executable(include-tests ${CMAKE_SOURCE_DIR}/test/include.cpp)

target_link_libraries(include-tests PUBLIC
    mapbox-base
)

