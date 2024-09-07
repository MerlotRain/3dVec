/**
 * Copyright (c) 2011-2018 by Andrew Mustun. All rights reserved.
 * 
 * This file is part of the QCAD project.
 *
 * QCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QCAD.
 */
#include <QElapsedTimer>
#include <QPen>

#include "RArc.h"
#include "RCircle.h"
#include "REllipse.h"
#include "RHatchData.h"
#include "RLine.h"
#include "RPatternListImperial.h"
#include "RPatternListMetric.h"
#include "RPointEntity.h"
#include "RPolyline.h"
#include "RUnit.h"

RHatchProxy *RHatchData::hatchProxy = NULL;

RHatchData::RHatchData()
    : solid(true), scaleFactor(1.0), angle(0.0), patternName("SOLID"),
      transparency(255), dirty(true), gotDraft(false), gotPixelSizeHint(0.0)
{
}

RHatchData::RHatchData(RDocument *document, const RHatchData &data)
    : REntityData(document)
{
    *this = data;
    this->document = document;
    if (document != NULL) { linetypeId = document->getLinetypeByLayerId(); }
}

RHatchData::RHatchData(const RHatchData &other) : REntityData(other)
{
    *this = other;
}

/**
 * \param solid True for a solid fill, false for a hatch pattern.
 * \param scaleFactor Scale factor of the hatch pattern.
 * \param angle Rotation angle of the hatch pattern.
 * \param patternName Name of the hatch pattern. "SOLID" for solid fills.
 */
RHatchData::RHatchData(bool solid, double scaleFactor, double angle,
                       const QString &patternName)
    : solid(solid), scaleFactor(scaleFactor), angle(angle),
      patternName(patternName), transparency(255), dirty(true), gotDraft(false)
{
}

RHatchData &RHatchData::operator=(const RHatchData &other)
{
    REntityData::operator=(other);

    solid = other.solid;
    scaleFactor = other.scaleFactor;
    angle = other.angle;
    transparency = other.transparency;
    patternName = other.patternName;
    originPoint = other.originPoint;
    gotDraft = other.gotDraft;
    pattern = other.pattern;
    gotPixelSizeHint = other.gotPixelSizeHint;

    boundary.clear();

    for (int i = 0; i < other.boundary.size(); ++i)
    {
        newLoop();
        QList<QSharedPointer<RShape>> loop = other.boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);

            QSharedPointer<RLine> line = shape.dynamicCast<RLine>();
            if (!line.isNull())
            {
                addBoundary(QSharedPointer<RShape>(new RLine(*line)));
                continue;
            }

            QSharedPointer<RArc> arc = shape.dynamicCast<RArc>();
            if (!arc.isNull())
            {
                addBoundary(QSharedPointer<RShape>(new RArc(*arc)));
                continue;
            }

            QSharedPointer<RCircle> circle = shape.dynamicCast<RCircle>();
            if (!circle.isNull())
            {
                addBoundary(QSharedPointer<RShape>(new RCircle(*circle)));
                continue;
            }

            QSharedPointer<REllipse> ellipseArc = shape.dynamicCast<REllipse>();
            if (!ellipseArc.isNull())
            {
                addBoundary(QSharedPointer<RShape>(new REllipse(*ellipseArc)));
                continue;
            }
        }
    }

    dirty = other.dirty;
    return *this;
}

void RHatchData::clearBoundary()
{
    boundary.clear();
    dirty = true;
}

RBox RHatchData::getBoundingBox(bool ignoreEmpty) const
{
    Q_UNUSED(ignoreEmpty)
    return RBox();
}

RVector RHatchData::getPointOnEntity() const { return RVector(); }

double RHatchData::getDistanceTo(const RVector &point, bool limited,
                                 double range, bool draft,
                                 double strictRange) const
{
    Q_UNUSED(limited)
    Q_UNUSED(range)

    if (!getBoundingBox().grow(strictRange).contains(point))
    {
        return RNANDOUBLE;
    }

    double ret = RNANDOUBLE;

    // path is filled (solid) or very dens / huge or we're in draft mode:
    int comp = getComplexity();

    return ret;
}

bool RHatchData::intersectsWith(const RShape &shape) const
{
    const RPolyline *polyline = dynamic_cast<const RPolyline *>(&shape);
    if (polyline == NULL) { return false; }

    // QPainterPath polylinePath = polyline->toPainterPath();

    // for (int i = 0; i < painterPaths.count(); i++)
    // {
    //     if (polylinePath.intersects(painterPaths[i]) &&
    //         !polylinePath.contains(painterPaths[i]))
    //     {

    //         return true;
    //     }
    // }

    return false;
}

QList<RRefPoint>
RHatchData::getReferencePoints(RS::ProjectionRenderingHint hint) const
{
    Q_UNUSED(hint)

    QList<RRefPoint> ret;

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);

            QSharedPointer<RLine> line = shape.dynamicCast<RLine>();
            if (!line.isNull())
            {
                ret.append(line->getStartPoint());
                continue;
            }

            QSharedPointer<RArc> arc = shape.dynamicCast<RArc>();
            if (!arc.isNull())
            {
                ret.append(arc->getStartPoint());
                continue;
            }

            QSharedPointer<RCircle> circle = shape.dynamicCast<RCircle>();
            if (!circle.isNull())
            {
                ret.append(circle->getCenter());
                continue;
            }

            QSharedPointer<REllipse> ellipseArc = shape.dynamicCast<REllipse>();
            if (!ellipseArc.isNull())
            {
                if (ellipseArc->isFullEllipse())
                {
                    ret.append(ellipseArc->getCenter());
                }
                else { ret.append(ellipseArc->getStartPoint()); }
                continue;
            }
        }
    }

    return ret;
}

bool RHatchData::moveReferencePoint(const RVector &referencePoint,
                                    const RVector &targetPoint,
                                    Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)

    bool ret = false;

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);

            QSharedPointer<RLine> line = shape.dynamicCast<RLine>();
            if (!line.isNull())
            {
                if (referencePoint.equalsFuzzy(line->getStartPoint()))
                {
                    line->setStartPoint(targetPoint);
                    ret = true;
                }
                if (referencePoint.equalsFuzzy(line->getEndPoint()))
                {
                    line->setEndPoint(targetPoint);
                    ret = true;
                }
            }

            QSharedPointer<RArc> arc = shape.dynamicCast<RArc>();
            if (!arc.isNull())
            {
                if (referencePoint.equalsFuzzy(arc->getStartPoint()))
                {
                    arc->moveStartPoint(targetPoint);
                    ret = true;
                }
                if (referencePoint.equalsFuzzy(arc->getEndPoint()))
                {
                    arc->moveEndPoint(targetPoint);
                    ret = true;
                }
            }

            QSharedPointer<RCircle> circle = shape.dynamicCast<RCircle>();
            if (!circle.isNull())
            {
                if (referencePoint.equalsFuzzy(circle->getCenter()))
                {
                    circle->setCenter(targetPoint);
                    ret = true;
                }
            }

            QSharedPointer<REllipse> ellipseArc = shape.dynamicCast<REllipse>();
            if (!ellipseArc.isNull())
            {
                if (ellipseArc->isFullEllipse())
                {
                    if (referencePoint.equalsFuzzy(ellipseArc->getCenter()))
                    {
                        ellipseArc->setCenter(targetPoint);
                        ret = true;
                    }
                }
                else
                {
                    if (referencePoint.equalsFuzzy(ellipseArc->getStartPoint()))
                    {
                        ellipseArc->moveStartPoint(targetPoint, false);
                        ret = true;
                    }
                    if (referencePoint.equalsFuzzy(ellipseArc->getEndPoint()))
                    {
                        ellipseArc->moveEndPoint(targetPoint, false);
                        ret = true;
                    }
                }
            }
        }
    }

    if (ret) { update(); }

    return ret;
}

bool RHatchData::move(const RVector &offset)
{
    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);
            shape->move(offset);
        }
    }

    update();
    return true;
}

bool RHatchData::rotate(double rotation, const RVector &center)
{
    angle = RMath::getNormalizedAngle(angle + rotation);

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);
            shape->rotate(rotation, center);
        }
    }

    update();
    return true;
}

bool RHatchData::scale(const RVector &scaleFactors, const RVector &center)
{
    scaleFactor *= scaleFactors.x;

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);
            shape->scale(scaleFactors, center);
        }
    }

    // scale custom entity specific pattern:
    pattern.scale(scaleFactors.x);

    update();
    return true;
}

bool RHatchData::mirror(const RLine &axis)
{
    double ang = axis.getAngle();
    angle = RMath::getNormalizedAngle(angle + ang * 2.0);

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);
            shape->mirror(axis);
        }
    }

    update();
    return true;
}

bool RHatchData::stretch(const RPolyline &area, const RVector &offset)
{
    bool ret = false;

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);
        for (int k = 0; k < loop.size(); ++k)
        {
            QSharedPointer<RShape> shape = loop.at(k);
            if (shape->stretch(area, offset)) { ret = true; }
        }
    }

    if (ret) { update(); }

    return ret;
}

QList<QSharedPointer<RShape>>
RHatchData::getShapes(const RBox &queryBox, bool ignoreComplex, bool segment,
                      QList<RObject::Id> *entityIds) const
{
    Q_UNUSED(queryBox)
    Q_UNUSED(segment)
    Q_UNUSED(entityIds)

    QList<QSharedPointer<RShape>> ret;

    if (ignoreComplex)
    {
        // complex shapes are ignored for example for snapping:
        for (int i = 0; i < boundary.size(); ++i)
        {
            QList<QSharedPointer<RShape>> loop = boundary.at(i);
            for (int k = 0; k < loop.size(); ++k)
            {
                QSharedPointer<RShape> shape = loop.at(k);
                if (queryBox.intersects(shape->getBoundingBox()))
                {
                    ret.append(shape);
                }
            }
        }
        return ret;
    }
    return ret;
}

QList<QSharedPointer<RShape>> RHatchData::getExploded() const
{
    return getShapes();
}

double RHatchData::getLength() const
{
    double ret = 0.0;
    //    QList<QSharedPointer<RShape> > shapes = getShapes();

    //    for (int i=0; i<shapes.length(); i++) {
    //        ret += shapes[i]->getLength();
    //    }

    //    return ret;

    QList<RPolyline> pls = getBoundaryAsPolylines();

    for (int i = 0; i < pls.length(); i++) { ret += pls[i].getLength(); }
    return ret;
}

double RHatchData::getArea() const
{
    QList<RPolyline> pls = getBoundaryAsPolylines();

    double ret = 0.0;
    for (int i = 0; i < pls.length(); i++)
    {
        RPolyline pl = pls[i];

        // check if polyline is inside odd number of other polylines (-)
        // or even number of other polylines (+) of this hatch:
        int counter = 0;
        for (int k = 0; k < pls.length(); k++)
        {
            // don't check against polyline self:
            if (i == k) { continue; }
            RPolyline other = pls[k];

            // define contains as 'must contain every vertex':
            bool contains = true;
            if (!other.getBoundingBox().contains(pl.getBoundingBox()))
            {
                // bounding box of other does not contain polyline:
                contains = false;
            }
            else
            {
                RVector pInside = pl.getPointInside();
                if (!pInside.isValid()) { pInside = pl.getStartPoint(); }
                if (!other.contains(pInside, true, 1.0e-3))
                {
                    // point inside polyline is not inside other:
                    contains = false;
                }
            }

            if (contains) { counter++; }
        }

        if (counter % 2 == 0)
        {
            // island:
            ret += pl.getArea();
        }
        else
        {
            // hole:
            ret -= pl.getArea();
        }
    }

    return ret;
}

void RHatchData::clearCustomPattern()
{
    pattern.clear();
    update();
}

void RHatchData::newLoop()
{
    QList<QSharedPointer<RShape>> loop;
    boundary.append(loop);
    update();
}

void RHatchData::cancelLoop()
{
    boundary.removeLast();
    update();
}

/**
 * Add a boundary shape to the current loop.
 * \param addAutoLoops True: create new loop if boundary does not connect.
 * Otherwise add line segment on the fly (DXF/DWG import).
 */
void RHatchData::addBoundary(QSharedPointer<RShape> shape, bool addAutoLoops)
{
    if (boundary.size() == 0)
    {
        qWarning() << "RHatchData::addBoundary: no loops found";
        return;
    }

    if (shape->getLength() < RS::PointTolerance)
    {
        // never append a zero length entity to a hatch boundary:
        return;
    }

    // if shape is a polyline, add segments as boundaries
    // TODO: add polyline as boundary
    QSharedPointer<RPolyline> polyline = shape.dynamicCast<RPolyline>();
    if (!polyline.isNull())
    {
        QList<QSharedPointer<RShape>> segments = polyline->getExploded();
        for (int i = 0; i < segments.length(); i++)
        {
            addBoundary(segments.at(i));
        }
    }
    else
    {
        // if the current loop is not empty, check if shape connects:
        if (!boundary.last().isEmpty())
        {
            QSharedPointer<RShape> prev = boundary.last().last();
            QSharedPointer<RShape> next = shape;
            if (!prev.isNull() && !next.isNull())
            {
                RVector ep = prev->getEndPoint();
                RVector sp = next->getStartPoint();

                // gap of more than 0.001:
                // new loop:
                if (!ep.equalsFuzzy(sp, 0.001))
                {
                    if (addAutoLoops)
                    {
                        // inserting loop on the fly:
                        newLoop();
                    }
                    else
                    {
                        // inserting line segment on the fly to closest match point,
                        // reverse shape if necessary:
                        if (ep.getDistanceTo(sp) <
                            ep.getDistanceTo(next->getEndPoint()))
                        {
                            boundary.last().append(
                                    QSharedPointer<RLine>(new RLine(ep, sp)));
                        }
                        else
                        {
                            boundary.last().append(QSharedPointer<RLine>(
                                    new RLine(ep, next->getEndPoint())));
                            shape->reverse();
                        }
                    }
                }

                // gap of more than 0.0001:
                else if (!ep.equalsFuzzy(sp, 0.0001))
                {
                    // enforce tolerance accepted by DXF/DWG format:
                    // insert bridge element on the fly:
                    boundary.last().append(
                            QSharedPointer<RLine>(new RLine(ep, sp)));
                }
            }
            else
            {
                qWarning()
                        << "RHatchData::addBoundary: unexpected boundary type";
            }
        }

        // 20160405: disabled, handled in getBoundaryPath at rendering time instead:
        // avoid arc boundaries with huge radius:
        //        QSharedPointer<RArc> arc = shape.dynamicCast<RArc>();
        //        if (!arc.isNull() && fabs(arc->getSweep())<RMath::deg2rad(2)) {
        //            boundary.last().append(QSharedPointer<RShape>(new RLine(arc->getStartPoint(), arc->getEndPoint())));
        //        }
        //        QSharedPointer<RArc> arc = shape.dynamicCast<RArc>();
        //        if (!arc.isNull() && fabs(arc->getSweep())>M_PI*2-RS::AngleTolerance) {
        //            boundary.last().append(QSharedPointer<RShape>(new RCircle(arc->getCenter(), arc->getRadius())));
        //        }
        //        else {
        boundary.last().append(shape);
        //        }
    }

    update();
}


QList<RLine> RHatchData::getSegments(const RLine &line) const
{
    QList<RLine> ret;

    // find all intersections of the given line with the boundary:
    // iterate through loops:
    QList<RVector> intersections;
    for (int loopIndex = 0; loopIndex < boundary.size(); loopIndex++)
    {
        QList<QSharedPointer<RShape>> loop = boundary[loopIndex];
        // iterate through boundary elements:
        for (int i = 0; i < loop.size(); i++)
        {
            QSharedPointer<RShape> boundary = loop[i];
            if (boundary.isNull()) { continue; }

            QList<RVector> ips =
                    boundary->getIntersectionPoints(line, true, false);
            if (ips.isEmpty()) { continue; }
            intersections.append(ips);
        }
    }

    if (intersections.isEmpty()) { return ret; }

    double angle = line.getAngle();
    if ((angle >= M_PI / 4.0 * 0.0 && angle <= M_PI / 4.0 * 1.0) ||
        (angle >= M_PI / 4.0 * 7.0 && angle <= M_PI / 4.0 * 8.0))
    {
        qSort(intersections.begin(), intersections.end(), RVector::lessThanX);
    }
    else if (angle >= M_PI / 4.0 * 3.0 && angle <= M_PI / 4.0 * 5.0)
    {
        qSort(intersections.begin(), intersections.end(),
              RVector::greaterThanX);
    }
    else if (angle >= M_PI / 4.0 * 1.0 && angle <= M_PI / 4.0 * 3.0)
    {
        qSort(intersections.begin(), intersections.end(), RVector::lessThanY);
    }
    else
    {
        qSort(intersections.begin(), intersections.end(),
              RVector::greaterThanY);
    }

    for (int i = 0; i < intersections.length() - 1; i++)
    {
        ret.append(RLine(intersections[i], intersections[i + 1]));
    }

    return ret;
}

/**
 * \return Pair of two shapes that connect to the vertex at the given index.
 * The first shape connects with its end point, the second with its start point.
 */
QPair<QSharedPointer<RShape>, QSharedPointer<RShape>>
RHatchData::getBoundaryElementsAt(int index, int &internalIndex) const
{
    QPair<QSharedPointer<RShape>, QSharedPointer<RShape>> ret;

    int indexCounter = 0;
    internalIndex = 0;

    for (int i = 0; i < boundary.size(); ++i)
    {
        QList<QSharedPointer<RShape>> loop = boundary.at(i);

        for (int k = 0; k < loop.size(); ++k)
        {
            if (index == indexCounter)
            {
                // spacial case: node is start point of a loop:
                if (k == 0)
                {
                    ret.first = loop.last();
                    if (loop.size() > 1) { ret.second = loop.first(); }
                    else
                    {
                        // loop with one entity (circe, ellipse, ..):
                        // set second result to NULL:
                        ret.second = QSharedPointer<RShape>();
                    }
                }
                else
                {
                    QSharedPointer<RShape> shape1 = loop.at(k - 1);
                    QSharedPointer<RShape> shape2 = loop.at(k);

                    ret.first = shape1;
                    ret.second = shape2;
                }
                return ret;
            }

            {
                indexCounter++;
            }
        }
    }

    return ret;
}

void RHatchData::update() const { dirty = true; }

bool RHatchData::order()
{
    update();
    return RShape::order(boundary);
}

QList<QSharedPointer<RShape>> RHatchData::getLoopBoundary(int index) const
{
    if (index < 0 || index >= boundary.count())
    {
        qWarning() << "RHatchData::getLoopBoundary: invalid loop index: "
                   << index;
        return QList<QSharedPointer<RShape>>();
    }
    return boundary[index];
}

QList<RPolyline> RHatchData::getBoundaryAsPolylines(double segmentLength) const
{
    if (hasProxy())
    {
        return RHatchData::getHatchProxy()->getBoundaryAsPolylines(
                *this, segmentLength);
    }
    else { return QList<RPolyline>(); }
}

int RHatchData::getComplexity() const
{

    int ret = 0;

    return ret;
}
