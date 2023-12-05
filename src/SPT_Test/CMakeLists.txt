# Value Define
set(PROJECT_NAME SPT_Test)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(GSD)


# Current Project
project("${PROJECT_NAME}")
add_executable(${PROJECT_NAME} "main.cpp")
