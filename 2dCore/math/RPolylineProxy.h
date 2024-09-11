#ifndef RPOLYLINEPROXY_H
#define RPOLYLINEPROXY_H

#include <CC2dCoreExport.h>

#include <RMath.h>
#include <RS.h>

class RArc;
class RLine;
class RPolyline;
class RVector;

class CC2DCORE_EXPORT RPolylineProxy
{
public:
    virtual ~RPolylineProxy() {}
    virtual RS::Ending getTrimEnd(RPolyline &polyline, const RVector &trimPoint,
                                  const RVector &clickPoint) = 0;
    virtual bool trimStartPoint(RPolyline &polyline, const RVector &trimPoint,
                                const RVector &clickPoint,
                                bool extend = false) = 0;
    virtual bool trimEndPoint(RPolyline &polyline, const RVector &trimPoint,
                              const RVector &clickPoint,
                              bool extend = false) = 0;
    virtual bool trimStartPoint(RPolyline &polyline, double trimDist) = 0;
    virtual bool trimEndPoint(RPolyline &polyline, double trimDist) = 0;
    virtual bool relocateStartPoint(RPolyline &polyline, const RVector &p) = 0;
    virtual bool relocateStartPoint(RPolyline &polyline, double dist) = 0;
    virtual int getSegmentAtDist(const RPolyline &polyline, double dist) = 0;

    virtual QList<RPolyline> renderThickPolyline(const RPolyline &polyline) = 0;
    virtual QList<QPair<RPolyline, RPolyline>>
    getLeftRightOutline(const RPolyline &polyline) = 0;
    virtual QList<RVector>
    verifyTangency(const RPolyline &polyline,
                   double toleranceMin = RS::AngleTolerance,
                   double toleranceMax = M_PI_4) = 0;
    virtual double getArea(const RPolyline &polyline, double segmentLength) = 0;
    virtual RVector getPointInside(const RPolyline &polyline) = 0;

    virtual QList<RPolyline> splitAtDiscontinuities(const RPolyline &polyline,
                                                    double tolerance) = 0;

    virtual double getBaseAngle(const RPolyline &polyline) = 0;
    virtual double getWidth(const RPolyline &polyline) = 0;
    virtual bool setWidth(RPolyline &polyline, double v) = 0;
    virtual double getHeight(const RPolyline &polyline) = 0;
    virtual bool setHeight(RPolyline &polyline, double v) = 0;

    virtual QList<RPolyline> morph(const RPolyline &polyline,
                                   const RPolyline &target, int steps,
                                   RS::Easing easing = RS::Linear,
                                   bool zLinear = true,
                                   double customFactor = RNANDOUBLE) = 0;
    virtual RPolyline roundAllCorners(const RPolyline &polyline,
                                      double radius) = 0;
    virtual RPolyline getPolygon(const RPolyline &polyline,
                                 double segmentLength) = 0;
    virtual RPolyline getPolygonHull(const RPolyline &polyline, double angle,
                                     double tolerance, bool inner = false) = 0;
    virtual RVector insertVertexAtDistance(RPolyline &polyline,
                                           double dist) = 0;

    virtual bool simplify(RPolyline &polyline, double tolerance) = 0;
    virtual QList<RPolyline>
    splitAtSegmentTypeChange(const RPolyline &polyline) = 0;
};

#endif
