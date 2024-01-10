link_directories(${CMAKE_BINARY_DIR}/lib)
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(PlatformDir "x86")
else()
    set(PlatformDir "x64")
endif()
link_directories(${CMAKE_BINARY_DIR}/${PlatformDir}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Widgets Gui)

add_executable(demo01 WIN32 main.cpp)

target_link_libraries(demo01 PUBLIC Qt6::Core Qt6::Widgets Qt6::Gui)
target_link_libraries(demo01 PUBLIC 
    debug SARibbonBard
    debug qt6advanceddockingd

    optimized SARibbonBar
    optimized qt6advanceddocking
)

