include_directories("${PROJECT_SOURCE_DIR}")

file(COPY metadata.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY metadataVIDE.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY metadata3.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY metadata4.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY data1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY data2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TU2 TU2.cpp ${PROJECT_SOURCE_DIR}/FileReader.cpp)
target_link_libraries(TU2 gtest_main)
add_test(NAME TU2_test COMMAND TU2)
