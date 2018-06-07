configure_file(../data/metadata1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/data1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/diagnosis1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/diagnosis2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/empty.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(TU5 TU5.cpp ${PROJECT_SOURCE_DIR}/src/Database.cpp ${PROJECT_SOURCE_DIR}/src/FileReader.cpp)
target_link_libraries(TU5 gtest_main)
add_test(NAME TU5_test COMMAND TU5)
