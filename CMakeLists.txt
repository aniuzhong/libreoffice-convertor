cmake_minimum_required(VERSION 3.5)

project(LibreOfficeConvertor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up LibreOffice SDK paths
set(SDK_INCLUDE_DIR  "${PROJECT_SOURCE_DIR}/instdir/sdk/include"  CACHE STRING  "OO_SDK_HOME/include")
set(SDK_LIBRARY_DIR  "${PROJECT_SOURCE_DIR}/instdir/sdk/lib"      CACHE STRING  "OO_SDK_HOME/lib")
set(SDK_BINARY_DIR   "${PROJECT_SOURCE_DIR}/instdir/sdk/bin"      CACHE STRING  "OO_SDK_HOME/bin")
set(SDK_PROGRAM_DIR  "${PROJECT_SOURCE_DIR}/instdir/program"      CACHE STRING  "OO_SDK_URE_LIB_DIR")
set(SDK_LIBRARIES    "isalhelper.lib" "isal.lib" "ipurpenvhelper.lib" "icppuhelper.lib" "icppu.lib")

add_definitions(-DWIN32 -DWNT -D_DLL -DCPPU_ENV=mscx)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${SDK_INCLUDE_DIR})
link_directories(${SDK_LIBRARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/cmdline)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/instdir/program)

add_executable(LOConvertor convertor.cpp)
target_link_directories(LOConvertor PRIVATE ${SDK_LIBRARY_DIR})
target_link_libraries(LOConvertor PRIVATE ${SDK_LIBRARIES})
