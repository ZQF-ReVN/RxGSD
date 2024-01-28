# Only x32
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
	return()
endif()

# Value Define
set(USER_PROJECT_NAME VFS_Hook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Current Project
project("${USER_PROJECT_NAME}")
add_library(${USER_PROJECT_NAME} MODULE "dllmain.cpp")

# Add Library
target_link_libraries("${USER_PROJECT_NAME}" PRIVATE GSD)