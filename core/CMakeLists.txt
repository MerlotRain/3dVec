add_library(3dvec_core SHARED)

target_sources(3dvec_core PRIVATE
)

target_link_libraries(3dvec_core ${CGAL_LIBRARIES})