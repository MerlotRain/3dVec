#include <qmath.h>

#include "RBox.h"
#include "RLine.h"
#include "RPoint.h"

/**
 * Creates a point object with an invalid position
 */
RPoint::RPoint() {}

//RPoint::RPoint(const RPoint& other) : RShape() {
//    this->position = other.position;
//}

RPoint::RPoint(double x, double y) : position(x, y) {}

/**
 * Creates a point object with the given position.
 *
 * \param position the position
 *
 */
RPoint::RPoint(const RVector &position) : position(position) {}

RPoint::~RPoint() {}

void RPoint::setZ(double z) { position.z = z; }

QList<RVector> RPoint::getVectorProperties() const
{
    return QList<RVector>() << position;
}

RBox RPoint::getBoundingBox() const { return RBox(position, position); }

double RPoint::getLength() const { return 0.0; }

QList<RVector> RPoint::getEndPoints() const
{
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getMiddlePoints() const
{
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getCenterPoints() const
{
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getPointsWithDistanceToEnd(double distance,
                                                  int from) const
{
    Q_UNUSED(distance)
    Q_UNUSED(from)

    QList<RVector> ret;
    return ret;
}

double RPoint::getAngleAt(double distance, RS::From from) const
{
    Q_UNUSED(distance)
    Q_UNUSED(from)
    return RNANDOUBLE;
}

RVector RPoint::getVectorTo(const RVector &point, bool limited,
                            double strictRange) const
{
    Q_UNUSED(limited)
    Q_UNUSED(strictRange)

    return point - position;
}

bool RPoint::move(const RVector &offset)
{
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance)
    {
        return false;
    }
    position += offset;
    return true;
}

bool RPoint::rotate(double rotation, const RVector &center)
{
    if (fabs(rotation) < RS::AngleTolerance) { return false; }
    position.rotate(rotation, center);
    return true;
}

bool RPoint::scale(const RVector &scaleFactors, const RVector &center)
{
    position.scale(scaleFactors, center);
    return true;
}

bool RPoint::mirror(const RLine &axis)
{
    position.mirror(axis);
    return true;
}

bool RPoint::flipHorizontal()
{
    position.flipHorizontal();
    return true;
}

bool RPoint::flipVertical()
{
    position.flipVertical();
    return true;
}

QSharedPointer<RShape> RPoint::getTransformed(const QTransform &transform) const
{
    return QSharedPointer<RShape>(
            new RPoint(position.getTransformed2D(transform)));
}
