cmake_minimum_required(VERSION 3.0..3.24)
project(QDesktopFramework)

set(qdf_include_files
    qdf_global.h
    qdf.h
    qdfmenubar.h)

set(qdf_private_files
    private/qdfmenubar_p.h
)

set(qdf_sources_files
    qdfmenubar.cpp
)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

add_library(QDesktopFramework SHARED ${qdf_include_files} ${qdf_private_files} ${qdf_sources_files})

target_link_libraries(QDesktopFramework
    PRIVATE
    Qt5::Core Qt5::Widgets Qt5::Gui
    user32
    gdi32
    shell32
    comctl32
)