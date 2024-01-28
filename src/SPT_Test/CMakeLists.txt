# Value Define
set(USER_PROJECT_NAME SPT_Test)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Current Project
project("${USER_PROJECT_NAME}")
add_executable(${USER_PROJECT_NAME} "main.cpp")

# Add Library
target_link_libraries("${USER_PROJECT_NAME}" PRIVATE GSD)