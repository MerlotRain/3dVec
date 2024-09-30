add_library(3dvec_math SHARED)

target_sources(3dvec_math PRIVATE
    shape.cc
    shape.h
)

target_link_libraries(3dvec_math ${CGAL_LIBRARIES})