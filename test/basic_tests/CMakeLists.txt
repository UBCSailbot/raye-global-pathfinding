include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
        example/ExampleTest.cpp)

add_executable(run_basic_tests ${TEST_FILES})

target_link_libraries(run_basic_tests gtest gtest_main)
target_link_libraries(run_basic_tests src_core)
