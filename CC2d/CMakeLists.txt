add_library(CC2dCore SHARED)

file(GLOB_RECURSE CC2D_CORE_HDRS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE CC2D_CORE_SRCS ${CMAKE_CURRENT_LIST_DIR}/*.h)

generate_export_header(CC2dCore
    BASE_NAME "CC2DCORE"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/CC2dCoreExport.h"
)

target_sources(CC2dCore PRIVATE ${CC2D_CORE_SRCS} ${CC2D_CORE_HDRS})

target_include_directories(CC2dCore SYSTEM PUBLIC
    ${${QT_VERSION_BASE}Core_INCLUDE_DIRS}
    ${${QT_VERSION_BASE}Gui_INCLUDE_DIRS}
    ${${QT_VERSION_BASE}Widgets_INCLUDE_DIRS}
    ${${QT_VERSION_BASE}Xml_INCLUDE_DIRS}
    ${${QT_VERSION_BASE}Network_INCLUDE_DIRS}
)

target_link_libraries(CC2dCore PUBLIC 
    ${QT_VERSION_BASE}::Core
    ${QT_VERSION_BASE}::Gui
    ${QT_VERSION_BASE}::Widgets
    ${QT_VERSION_BASE}::Xml
    ${QT_VERSION_BASE}::Network

    spatialIndex
    )

target_include_directories(CC2dCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/math
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(CC2dCore PRIVATE R_NO_OPENNURBS)