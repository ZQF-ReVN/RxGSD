# Project
project(GSD)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BMZ_Cryptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Global.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Cryptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Str.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_HDR.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Arg.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Code.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_File.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GSP_Editor.cpp" )

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    list(APPEND SRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp")
endif()

# Set Library
add_library("${PROJECT_NAME}" "${SRC_FILE}")

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE Rut RxHook zlibstatic)

