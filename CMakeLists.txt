# Main CMakeLists
cmake_minimum_required(VERSION 3.15)

# Project Name
project(GSD_Tools)

# Option
set(STATIC_MSVC_RUNTIME OFF)

# Value Define
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Out Path
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x32")
    set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x32")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/.bin/x64")
    set(LIBRARY_OUTPUT_PATH "${ROOT_DIR}/.bin/x64")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/debug")
    set(LIBRARY_OUTPUT_PATH "${LIBRARY_OUTPUT_PATH}/debug")
else()
    set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/release")
    set(LIBRARY_OUTPUT_PATH "${LIBRARY_OUTPUT_PATH}/release")
endif()

# MSVC Setting
if(MSVC)
    # Macro
    add_definitions(-DUNICODE -D_UNICODE)

    # Runtime
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if (STATIC_MSVC_RUNTIME)
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebugDLL)
        endif()
    else()
        if (STATIC_MSVC_RUNTIME)
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
        endif()
    endif()

    # Compiler
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        #set(CMAKE_CXX_FLAGS_DEBUG)
    else()
        add_link_options(/LTCG:INCREMENTAL /OPT:REF)
        set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /source-charset:utf-8 /GL /Gy /GS /Zc:inline /Zc:forScope")
        #set(CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        #set(CMAKE_CXX_FLAGS_MINSIZEREL)
    endif()
endif()

# Include
include_directories("${ROOT_DIR}")

# Library
add_subdirectory("third")
add_subdirectory("lib")
add_subdirectory("src")
