
add_library( CCdb SHARED )

add_subdirectory( include )
add_subdirectory( src )

target_link_libraries( CCdb
    CCCoreLib
    CCFbo
)

target_compile_definitions( CCdb PRIVATE QCC_DB_LIBRARY_BUILD QT_NO_KEYWORDS )

set_target_properties( CCdb PROPERTIES
	CXX_VISIBILITY_PRESET hidden
)

target_link_libraries( CCdb 
    ${QT_VERSION_BASE}::Core 
    ${QT_VERSION_BASE}::Gui 
    ${QT_VERSION_BASE}::Widgets
    ${QT_VERSION_BASE}::OpenGL)