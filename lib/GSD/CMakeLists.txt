# Library Name
set(PROJECT_NAME GSD)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Rut RxHook)

# Add ZLIB
include_directories("${THIRD_DIR}/zlib/include/")
link_libraries("${THIRD_DIR}/zlib/lib.X86/zlibstatic.lib")

# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BMZ_Coder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Decoder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Text_Editor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GSP_Editor.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp")

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
