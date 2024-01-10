cmake_minimum_required(VERSION 3.20)
project(desktopFramework)

# 提前编译第三方库，然后依赖
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(3rdparty/Qt-Advanced-Docking-System)
add_subdirectory(3rdparty/SARibbon)

include_directories(3rdparty/Qt-Advanced-Docking-System/src)
include_directories(3rdparty/SARibbon/src)

add_subdirectory(src)