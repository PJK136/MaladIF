include_directories("${PROJECT_SOURCE_DIR}")

file(COPY metadata1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY metadata2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY bad_metadata1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY bad_metadata2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TU1 TU1.cpp ${PROJECT_SOURCE_DIR}/FileReader.cpp)
target_link_libraries(TU1 gtest_main)
add_test(NAME TU1_test COMMAND TU1)
