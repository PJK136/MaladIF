include_directories("${PROJECT_SOURCE_DIR}")

configure_file(../data/metadata1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata_bad1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata_bad2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(TU1 TU1.cpp ${PROJECT_SOURCE_DIR}/FileReader.cpp)
target_link_libraries(TU1 gtest_main)
add_test(NAME TU1_test COMMAND TU1)
