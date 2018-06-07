configure_file(../data/metadata1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/data1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(TU4 TU4.cpp $<TARGET_OBJECTS:MaladIF_core>)
target_link_libraries(TU4 gtest_main)
add_test(NAME TU4_test COMMAND TU4)
