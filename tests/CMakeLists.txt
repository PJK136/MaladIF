include_directories("${PROJECT_SOURCE_DIR}")

# Now simply link against gtest or gtest_main as needed. Eg

# First Simple Google Test
add_executable(simple simple.cpp)
target_link_libraries(simple gtest_main)
add_test(NAME simple_test COMMAND simple)

add_subdirectory(TU1)
add_subdirectory(TU2)
