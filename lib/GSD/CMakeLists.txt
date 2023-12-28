# Library Name
set(PROJECT_NAME GSD)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Library
link_libraries(Rut RxHook)

# Add ZLIB
include_directories("${THIRD_DIR}/zlib/include/")
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    link_libraries("${THIRD_DIR}/zlib/lib.X86/zlibstatic.lib")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    link_libraries("${THIRD_DIR}/zlib/lib.X64/zlibstatic.lib")
endif()


# Set Source File Path
list(APPEND SRC_FILE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BMZ_Cryptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Cryptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Str.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_HDR.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Arg.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Code.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SPT_Parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GSP_Editor.cpp" )

# Source File Only x32
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    list(APPEND SRC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/VFS_Hook.cpp")
endif()

# Set Library
add_library(${PROJECT_NAME} "${SRC_FILE}")
