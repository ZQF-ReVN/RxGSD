# Project Name
project(Rut)

# Cpp Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source File Path
list(APPEND SRC_FILE 
    "RxSys/Win32/File.cpp" 
    "RxSys/Win32/Path.cpp" 
    "RxSys/Win32/String.cpp" 
    "RxSys/Win32/System.cpp" 
    "RxBench.cpp" 
    "RxCmd.cpp" 
    "RxPath.cpp" 
    "RxFile.cpp" 
    "RxMem.cpp" 
    "RxINI.cpp" 
    "RxJson.cpp" 
    "RxStr.cpp" )

# Set Library
add_library("${PROJECT_NAME}" "${SRC_FILE}")


