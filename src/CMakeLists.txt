link_directories(${CMAKE_BINARY_DIR}/lib)
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(PlatformDir "x86")
else()
    set(PlatformDir "x64")
endif()
link_directories(${CMAKE_BINARY_DIR}/${PlatformDir}/lib)

set(qdf_headers application.h)
set(qdf_sources application.cpp)

find_package(${QT_MAJOR_VERSION} COMPONENTS Core Widgets Gui)

add_library(qdesktopframework SHARED
    ${qdf_headers} 
    ${qdf_sources})

set_target_properties(${qdesktopframework} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    VERSION ${SARIBBON_VERSION}
    EXPORT_NAME ${qdesktopframework}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(qdesktopframework PUBLIC 
    ${QT_MAJOR_VERSION}::Core 
    ${QT_MAJOR_VERSION}::Widgets 
    ${QT_MAJOR_VERSION}::Gui)

target_link_libraries(qdesktopframework PUBLIC 
    debug SARibbonBard
    debug qt6advanceddockingd

    optimized SARibbonBar
    optimized qt6advanceddocking
)

