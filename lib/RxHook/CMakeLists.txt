# Project Name
project(RxHook)

# Cpp Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "src/API.cpp" 
    "src/Mem.cpp" 
    "src/Hook.cpp" 
    "src/ErrorMsg.cpp" 
)

# Set Current Library
add_library("${PROJECT_NAME}" "${SRC_FILE}")

# Add Library
# Detours
include_directories("${ROOT_DIR}/third/detours/include/")
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_link_libraries("${PROJECT_NAME}" PRIVATE "${ROOT_DIR}/third/detours/lib.X86/detours.lib")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries("${PROJECT_NAME}" PRIVATE "${ROOT_DIR}/third/detours/lib.X64/detours.lib")
endif()