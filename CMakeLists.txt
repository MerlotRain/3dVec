cmake_minimum_required(VERSION 3.10)
project(GLCAD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

include(GenerateExportHeader)

find_package(PkgConfig REQUIRED)

# Check for Boost
find_package(Boost REQUIRED COMPONENTS serialization)
# Check for CGAL
find_package(CGAL REQUIRED)
# Check for Eigen
find_package(Eigen3 REQUIRED)
# Check for VTK
find_package(VTK REQUIRED)


add_subdirectory(math)