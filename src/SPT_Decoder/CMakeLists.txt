# Value Define
set(PROJECT_NAME SPT_Decoder)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(GSD Rut)

# Current Project
project("${PROJECT_NAME}")
add_executable(${PROJECT_NAME} "main.cpp")

