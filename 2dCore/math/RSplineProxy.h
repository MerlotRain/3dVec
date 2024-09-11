#ifndef RSPLINEPROXY_H
#define RSPLINEPROXY_H

#include <CC2dCoreExport.h>

#include "RSpline.h"

class CC2DCORE_EXPORT RSplineProxy {
public:
    virtual ~RSplineProxy() {}
    virtual void init() = 0;
    virtual void uninit() = 0;
    virtual RSpline updateFromFitPoints(const RSpline& spline, bool rec = false) = 0;
    virtual QList<RSpline> split(const RSpline& spline, const QList<double>& params) = 0;
    virtual QList<RSpline> splitEqual(const RSpline& spline, int pieces) = 0;
    virtual double getTAtPoint(const RSpline& spline, const RVector& point) = 0;
    virtual double getTAtDistance(const RSpline& spline, double dist) = 0;
    virtual double getLength(const RSpline& spline) = 0;
    virtual double getDistanceAtT(const RSpline& spline, double t) = 0;
    virtual RVector getVectorTo(const RSpline& spline, const RVector& point, bool limited=true, double strictRange=RMAXDOUBLE) = 0;
    virtual RSpline simplify(const RSpline& spline, double tolerance) = 0;
    virtual RPolyline approximateWithArcs(const RSpline& spline, double tolerance=0.001, double radiusLimit=RDEFAULT_MIN1) = 0;
    virtual QList<RVector> getIntersectionPoints(const RSpline& spline, const RShape& other, bool limited=true, bool same=false, double tolerance=RS::PointTolerance) = 0;
};

#endif
