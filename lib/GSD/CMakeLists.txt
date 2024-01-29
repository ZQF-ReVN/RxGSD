# Project
project(GSD)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND ${PROJECT_NAME}_SRC_FILE 
    "src/GSD/BMZ_Cryptor.cpp"
    "src/GSD/SPT_Global.cpp"
    "src/GSD/SPT_Cryptor.cpp"
    "src/GSD/SPT_Str.cpp"
    "src/GSD/SPT_HDR.cpp"
    "src/GSD/SPT_Arg.cpp"
    "src/GSD/SPT_Code.cpp"
    "src/GSD/SPT_File.cpp"
    "src/GSD/GSP_Editor.cpp" )

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    list(APPEND ${PROJECT_NAME}_SRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/GSD/VFS_Hook.cpp")
endif()

# Set Library
add_library("${PROJECT_NAME}" "${${PROJECT_NAME}_SRC_FILE}")
target_include_directories("${PROJECT_NAME}" INTERFACE "src")

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE Rut RxHook zlibstatic)

