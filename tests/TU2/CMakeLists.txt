configure_file(../data/metadata1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata3.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata4.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/data1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata_bad1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata_bad2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/metadata_bad3.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/empty.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/data_bad1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(../data/data_bad2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(TU2 TU2.cpp $<TARGET_OBJECTS:MaladIF_core>)
target_link_libraries(TU2 gtest_main)
add_test(NAME TU2_test COMMAND TU2)
