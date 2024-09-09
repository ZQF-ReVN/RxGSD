# RxGSD
cmake_minimum_required(VERSION 3.25)

# Project Name
project(ReVN_RxGSD)

# Export Symbols
if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Set Source File Path
list(APPEND ReVN_RxGSD_Core_Src_Files
    "src/RxGSD/Core/GSP.cpp"
    "src/RxGSD/Core/BMZ.cpp"
    "src/RxGSD/Core/SPT_Arg.cpp"
    "src/RxGSD/Core/SPT_Code.cpp"
    "src/RxGSD/Core/SPT_Cryptor.cpp"
    "src/RxGSD/Core/SPT_File.cpp"
    "src/RxGSD/Core/SPT_Global.cpp"
    "src/RxGSD/Core/SPT_HDR.cpp"
    "src/RxGSD/Core/SPT_Str.cpp")

# Set Library
add_library(ReVN_RxGSD_Core ${ReVN_RxGSD_Core_Src_Files})
add_library(ReVN::RxGSD::Core ALIAS ReVN_RxGSD_Core)
target_include_directories(ReVN_RxGSD_Core PUBLIC "src")
target_compile_features(ReVN_RxGSD_Core PRIVATE cxx_std_23)
target_compile_options(ReVN_RxGSD_Core PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_link_libraries(ReVN_RxGSD_Core PRIVATE Zut::ZxFile Zut::ZxFS ZLIB::ZLIB)
target_link_libraries(ReVN_RxGSD_Core PUBLIC Zut::ZxMem Zut::ZxJson Zut::ZxCvt)

# Warning
if(MSVC)
    target_compile_options(ReVN_RxGSD_Core PRIVATE /W4)
else()
    target_compile_options(ReVN_RxGSD_Core PRIVATE -Wall -Wextra)
endif()


# Hook
if((MSVC) AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    # Set Source File Path
    list(APPEND ReVN_RxGSD_Hook_Src_Files
        "src/RxGSD/Hook/VFS_Hook.cpp")

    # Set Library
    add_library(ReVN_RxGSD_Hook ${ReVN_RxGSD_Hook_Src_Files})
    add_library(ReVN::RxGSD::Hook ALIAS ReVN_RxGSD_Hook)
    target_include_directories(ReVN_RxGSD_Hook PUBLIC "src")
    target_compile_options(ReVN_RxGSD_Hook PRIVATE /W3)
    target_compile_features(ReVN_RxGSD_Hook PRIVATE cxx_std_23)
    target_compile_options(ReVN_RxGSD_Hook PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    target_link_libraries(ReVN_RxGSD_Hook PUBLIC Zut::ZxHook)
endif()
