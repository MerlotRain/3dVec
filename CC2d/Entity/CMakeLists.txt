
add_library(CC2dEntity SHARED
    RArcData.cpp RArcData.h
    RArcEntity.cpp RArcEntity.h
    RAttributeData.cpp RAttributeData.h
    RAttributeDefinitionData.cpp RAttributeDefinitionData.h
    RAttributeDefinitionEntity.cpp RAttributeDefinitionEntity.h
    RAttributeEntity.cpp RAttributeEntity.h
    RCircleData.cpp RCircleData.h
    RCircleEntity.cpp RCircleEntity.h
    RDimAlignedData.cpp RDimAlignedData.h
    RDimAlignedEntity.cpp RDimAlignedEntity.h
    RDimAngular2LData.cpp RDimAngular2LData.h
    RDimAngular2LEntity.cpp RDimAngular2LEntity.h
    RDimAngular3PData.cpp RDimAngular3PData.h
    RDimAngular3PEntity.cpp RDimAngular3PEntity.h
    RDimAngularData.cpp RDimAngularData.h
    RDimAngularEntity.cpp RDimAngularEntity.h
    RDimArcLengthData.cpp RDimArcLengthData.h
    RDimArcLengthEntity.cpp RDimArcLengthEntity.h
    RDimDiametricData.cpp RDimDiametricData.h
    RDimDiametricEntity.cpp RDimDiametricEntity.h
    RDimLinearData.cpp RDimLinearData.h
    RDimLinearEntity.cpp RDimLinearEntity.h
    RDimOrdinateData.cpp RDimOrdinateData.h
    RDimOrdinateEntity.cpp RDimOrdinateEntity.h
    RDimRadialData.cpp RDimRadialData.h
    RDimRadialEntity.cpp RDimRadialEntity.h
    RDimRotatedData.cpp RDimRotatedData.h
    RDimRotatedEntity.cpp RDimRotatedEntity.h
    RDimStyleProxyBasic.cpp RDimStyleProxyBasic.h
    RDimensionData.cpp RDimensionData.h
    RDimensionEntity.cpp RDimensionEntity.h
    REllipseData.cpp REllipseData.h
    REllipseEntity.cpp REllipseEntity.h
    RFaceData.cpp RFaceData.h
    RFaceEntity.cpp RFaceEntity.h
    RHatchData.cpp RHatchData.h
    RHatchEntity.cpp RHatchEntity.h
    RHatchProxy.h
    RImageData.cpp RImageData.h
    RImageEntity.cpp RImageEntity.h
    RLeaderData.cpp RLeaderData.h
    RLeaderEntity.cpp RLeaderEntity.h
    RLineData.cpp RLineData.h
    RLineEntity.cpp RLineEntity.h
    RPointData.cpp RPointData.h
    RPointEntity.cpp RPointEntity.h
    RPolylineData.cpp RPolylineData.h
    RPolylineEntity.cpp RPolylineEntity.h
    RWipeoutData.cpp RWipeoutData.h
    RWipeoutEntity.cpp RWipeoutEntity.h
    RRayData.cpp RRayData.h
    RRayEntity.cpp RRayEntity.h
    RSolidData.cpp RSolidData.h
    RSolidEntity.cpp RSolidEntity.h
    RSplineData.cpp RSplineData.h
    RSplineEntity.cpp RSplineEntity.h
    RTextData.cpp RTextData.h
    RTextEntity.cpp RTextEntity.h
    RToleranceData.cpp RToleranceData.h
    RToleranceEntity.cpp RToleranceEntity.h
    RTraceData.cpp RTraceData.h
    RTraceEntity.cpp RTraceEntity.h
    RXLineData.cpp RXLineData.h
    RXLineEntity.cpp RXLineEntity.h
    ${RC}
)


generate_export_header(CC2dEntity
    BASE_NAME "QCADENTITY"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/CC2dEntityExport.h"
)

target_link_libraries(CC2dEntity CC2dCore)

target_include_directories(CC2dEntity PUBLIC ${CMAKE_CURRENT_BINARY_DIR})