#ifndef RRAY_H
#define RRAY_H

#include <CC2dCoreExport.h>

#include "RS.h"
#include "RShape.h"
#include "RVector.h"
#include "RXLine.h"

class RBox;
class RPolyline;

class CC2DCORE_EXPORT RRay : public RXLine
{
public:
    RRay();
    RRay(const RLine &line);
    RRay(const RVector &basePoint, const RVector &directionVector);
    RRay(const RVector &basePoint, double angle, double distance);
    virtual ~RRay();

    virtual RShape::Type getShapeType() const { return Ray; }

    virtual RRay *clone() const { return new RRay(*this); }

    virtual QList<RVector> getVectorProperties() const;

    virtual bool trimEndPoint(const RVector &trimPoint,
                              const RVector &clickPoint = RVector::invalid,
                              bool extend = false);
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance,
                                                      int from) const;

    virtual bool reverse();
    virtual RLine getClippedLine(const RBox &box) const;
    virtual RVector getVectorTo(const RVector &point, bool limited = true,
                                double strictRange = RMAXDOUBLE) const;

    virtual bool stretch(const RPolyline &area, const RVector &offset);

    virtual QList<QSharedPointer<RShape>>
    splitAt(const QList<RVector> &points) const;

#if QT_VERSION >= 0x060000
    /**
     * copy function for Qt 6 scripts:
     * \nonscriptable
     */
    RRay copy() const { return *this; }
#endif
};
Q_DECLARE_METATYPE(const RRay*)
Q_DECLARE_METATYPE(RRay*)
Q_DECLARE_METATYPE(RRay)
Q_DECLARE_METATYPE(QSharedPointer<RRay>)
Q_DECLARE_METATYPE(QSharedPointer<RRay>*)

#endif
