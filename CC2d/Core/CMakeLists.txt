add_library(CC2dCore SHARED)

set(core_SRCS 
    RColor.cpp
    # RBlock.cpp
    # RBlockReferenceData.cpp
    # RBlockReferenceEntity.cpp
    # RDimStyle.cpp
    # RDimStyleData.cpp
    # RDocument.cpp
    # RDocumentVariables.cpp
    # REntity.cpp
    # REntityData.cpp
    # RLayer.cpp
    # RLayerState.cpp
    # RLinetype.cpp
    # RLinetypeList.cpp
    # RLinetypeListImperial.cpp
    # RLinetypeListMetric.cpp
    # RLinetypePattern.cpp
    # RLineweight.cpp
    # RLinkedStorage.cpp
    # RMemoryStorage.cpp
    # RObject.cpp
    # RPattern.cpp
    # RPatternLine.cpp
    # RPatternList.cpp
    # RPatternListImperial.cpp
    # RPatternListMetric.cpp
    # RPropertyAttributes.cpp
    # RPropertyTypeId.cpp
    RS.cpp
    # RSpatialIndex.cpp
    # RSpatialIndexSimple.cpp
    # RStorage.cpp
    # RStorageBlockSort.cpp
    # RStorageLayerSort.cpp
    # RUnit.cpp
    # RView.cpp
    # RViewportData.cpp
    # RViewportEntity.cpp

    math/RArc.cpp
    math/RBox.cpp
    math/RCircle.cpp
    math/RDirected.cpp
    math/REllipse.cpp
    math/RLine.cpp
    math/RMath.cpp
    math/RMatrix.cpp
    math/RPoint.cpp
    math/RPolyline.cpp
    math/RRay.cpp
    math/RRefPoint.cpp
    math/RShape.cpp
    math/RTextLabel.cpp
    math/RTransform.cpp
    math/RTriangle.cpp
    math/RVector.cpp
    math/RXLine.cpp
)

set(core_HDRS
    RColor.h
    RColorCodes.h
    # RBlock.h
    # RBlockReferenceData.h
    # RBlockReferenceEntity.h
    # RDimStyle.h
    # RDimStyleData.h
    # RDocument.h
    # RDocumentVariables.h
    # REntity.h
    # REntityData.h
    RGlobal.h
    # RLayer.h
    # RLayerProxy.h
    # RLayerState.h
    # RLinetype.h
    # RLinetypeList.h
    # RLinetypeListImperial.h
    # RLinetypeListMetric.h
    # RLinetypePattern.h
    # RLineweight.h
    # RLinkedStorage.h
    # RMemoryStorage.h
    # RNonCopyable.h
    # RObject.h
    # RPattern.h
    # RPatternLine.h
    # RPatternList.h
    # RPatternListImperial.h
    # RPatternListMetric.h
    # RPropertyAttributes.h
    # RPropertyTypeId.h
    # RRequireHeap.h
    # RResourceList.h
    RS.h
    # RSpatialIndex.h
    # RSpatialIndexSimple.h
    # RSpatialIndexVisitor.h
    # RStorage.h
    # RStorageBlockSort.h
    # RStorageLayerSort.h
    # RView.h
    # RViewportData.h
    # RViewportEntity.h

    math/RArc.h
    math/RBox.h
    math/RCircle.h
    math/RDirected.h
    math/REllipse.h
    math/REllipseProxy.h
    math/RExplodable.h
    math/RLine.h
    math/RMath.h
    math/RMatrix.h
    math/RPoint.h
    math/RPolyline.h
    math/RPolylineProxy.h
    math/RRay.h
    math/RRefPoint.h
    math/RShape.h
    math/RShapeProxy.h
    math/RTextLabel.h
    math/RTransform.h
    math/RTransformOp.h
    math/RTriangle.h
    math/RVector.h
    math/RXLine.h
)

generate_export_header(CC2dCore
    BASE_NAME "QCADCORE"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/CC2dCoreExport.h"
)

target_sources(CC2dCore PRIVATE ${core_SRCS} ${core_HDRS})

target_link_libraries(CC2dCore PUBLIC 
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Network
    )

target_include_directories(CC2dCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/math
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(CC2dCore PRIVATE R_NO_OPENNURBS)