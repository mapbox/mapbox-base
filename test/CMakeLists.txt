enable_testing()

set (TEST_BINARY_PATH ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_LIST_DIR}/test_defines.hpp.in ${CMAKE_CURRENT_LIST_DIR}/test_defines.hpp)

function(create_test folder_name)
    set (target_name "test_${folder_name}")
    set (curr_folder "${CMAKE_CURRENT_LIST_DIR}/${folder_name}")
    
    file (GLOB_RECURSE test_files "${curr_folder}/*.cpp" "${curr_folder}/*.hpp")
    message (STATUS "Adding test ${target_name}")

    add_executable(${target_name}  ${test_files})
    target_link_libraries(${target_name} 
        PRIVATE gtest_all 
        mapbox-base
        pthread
        )
    target_include_directories(${target_name} PRIVATE
            ${curr_folder}
            ${PROJECT_SOURCE_DIR}/mapbox/include
            ${CMAKE_CURRENT_LIST_DIR}
            )

    add_test(NAME ${target_name} COMMAND ${target_name} ${TEST_ARGUMENTS})
endfunction()

create_test("std")
create_test("io")
create_test("util")

# TODO: find out why this should be explicitly specified in order to run `make test `
add_custom_target(test-all COMMAND ${CMAKE_CTEST_COMMAND})
