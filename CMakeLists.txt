cmake_minimum_required(VERSION 3.20)
project(desktopFramework)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(3rdparty/Qt-Advanced-Docking-System)
add_subdirectory(3rdparty/SARibbon)

include_directories(3rdparty/Qt-Advanced-Docking-System/src)
include_directories(3rdparty/SARibbon/src)

add_subdirectory(src)