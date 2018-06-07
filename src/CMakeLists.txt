add_library(MaladIF_core OBJECT CLI.cpp Database.cpp FileReader.cpp)

add_executable(MaladIF main.cpp $<TARGET_OBJECTS:MaladIF_core>)
