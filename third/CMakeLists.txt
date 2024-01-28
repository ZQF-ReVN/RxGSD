# Zlib
if(NOT EXISTS "${ROOT_DIR}/third/zlib")
    message(FATAL_ERROR "***** Please add zlib to third folder *****")
else()
    option(ZLIB_BUILD_EXAMPLES "" OFF)
    add_subdirectory("zlib")
endif()
