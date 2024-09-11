#ifndef RPOINT_H
#define RPOINT_H

#include <CC2dCoreExport.h>

#include "RBox.h"
#include "RShape.h"
#include "RVector.h"

class RLine;

class CC2DCORE_EXPORT RPoint : public RShape
{
public:
    RPoint();
    RPoint(double x, double y);
    RPoint(const RVector &position);
    virtual ~RPoint();

    virtual RShape::Type getShapeType() const { return Point; }

    virtual RPoint *clone() const { return new RPoint(*this); }

    virtual void setZ(double z);

    virtual QList<RVector> getVectorProperties() const;

    RVector getPosition() const { return position; }

    void setPosition(const RVector &p) { position = p; }

    virtual RBox getBoundingBox() const;
    virtual double getLength() const;

    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector>
    getPointsWithDistanceToEnd(double distance, int from = RS::FromAny) const;

    virtual double getAngleAt(double distance,
                              RS::From from = RS::FromStart) const;

    virtual RVector getVectorTo(const RVector &point, bool limited = true,
                                double strictRange = RMAXDOUBLE) const;

    virtual bool move(const RVector &offset);
    virtual bool rotate(double rotation,
                        const RVector &center = RDEFAULT_RVECTOR);
    virtual bool scale(const RVector &scaleFactors,
                       const RVector &center = RDEFAULT_RVECTOR);
    virtual bool mirror(const RLine &axis);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

    virtual QSharedPointer<RShape>
    getTransformed(const QTransform &transform) const;

#if QT_VERSION >= 0x060000
    /**
     * copy function for Qt 6 scripts:
     * \nonscriptable
     */
    RPoint copy() const { return *this; }
#endif


public:
    /**
     * \getter{getPosition}
     * \setter{setPosition}
     */
    RVector position;
};
Q_DECLARE_METATYPE(RPoint)
Q_DECLARE_METATYPE(RPoint*)
Q_DECLARE_METATYPE(QSharedPointer<RPoint>)
Q_DECLARE_METATYPE(QSharedPointer<RPoint>*)

#endif
