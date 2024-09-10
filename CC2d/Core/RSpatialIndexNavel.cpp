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
#include "RSpatialIndexNavel.h"


#ifdef _MSC_VER
#define RSIMAXDOUBLE 1e300
#define RSIMINDOUBLE -1e300
#else
#define RSIMAXDOUBLE std::numeric_limits<double>::max()
#define RSIMINDOUBLE -std::numeric_limits<double>::max()
#endif



class RSiDataStream : public SpatialIndex::IDataStream
{
public:
    RSiDataStream(const QList<int> &ids, const QList<QList<RBox>> &bbs)
        : ids(ids), bbs(bbs), index(0), pos(0), done(false)
    {

        Q_ASSERT(this->ids.length() == this->bbs.length());
    }

    virtual ~RSiDataStream() {}

    virtual SpatialIndex::IData *getNext()
    {
        Q_ASSERT(index < ids.length());
        Q_ASSERT(index < bbs.length());
        Q_ASSERT(pos < bbs[index].length());

        RBox bb = bbs[index][pos];
        double p1[] = {bb.getMinimum().x, bb.getMinimum().y, bb.getMinimum().z};
        double p2[] = {bb.getMaximum().x, bb.getMaximum().y, bb.getMaximum().z};
        SpatialIndex::Region r(p1, p2, 3);
        qint64 id = RSpatialIndex::getSIId(ids[index], pos);

        if (pos < bbs[index].length() - 1) { pos++; }
        else
        {
            index++;
            pos = 0;
            if (index >= ids.length() || index >= bbs.length()) { done = true; }
            else
            {
                // skip empty bbs lists:
                while (index < bbs.length() && bbs[index].isEmpty())
                {
                    index++;
                }
            }
        }

        return new SpatialIndex::RTree::Data(0, NULL, r, id);
    }

    virtual bool hasNext() { return !done; }

    virtual uint32_t size()
    {
        throw Tools::NotSupportedException("Operation not supported.");
    }

    virtual void rewind()
    {
        index = 0;
        pos = 0;
    }

private:
    const QList<int> &ids;
    const QList<QList<RBox>> &bbs;
    int index;
    int pos;
    bool done;
};


/**
 * \ingroup spatialindex
 */
class RSiGetRegionVisitor : public RSpatialIndexVisitor
{
public:
    RSiGetRegionVisitor(int id) : id(id), found(false) {}
    virtual ~RSiGetRegionVisitor() {}
    virtual void visitData(int id, int pos, double x1, double y1, double z1,
                           double x2, double y2, double z2)
    {

        Q_UNUSED(pos);

        if (this->id == id)
        {
            double p1[] = {x1, y1, z1};
            double p2[] = {x2, y2, z2};

            region = SpatialIndex::Region(p1, p2, 3);

            found = true;
        }
    }
    virtual void visitNode(double /*x1*/, double /*y1*/, double /*z1*/,
                           double /*x2*/, double /*y2*/, double /*z2*/)
    {
    }

public:
    int id;
    SpatialIndex::Region region;
    bool found;
};


/**
 * Internal.
 */
RSpatialIndexNavel::RSiRegion::RSiRegion(double x1, double y1, double z1,
                                         double x2, double y2, double z2)
{

    double p1[] = {std::min(x1, x2), std::min(y1, y2), std::min(z1, z2)};
    double p2[] = {std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)};

    (*(SpatialIndex::Region *) this) = SpatialIndex::Region(p1, p2, 3);
}


/**
 * Internal.
 */
RSpatialIndexNavel::RSiPoint::RSiPoint(double x, double y, double z)
{

    double p[] = {x, y, z};

    (*(SpatialIndex::Point *) this) = SpatialIndex::Point(p, 3);
}


/**
 * Creates a 2d spatial index in memory (r-tree, star shape).
 */
RSpatialIndexNavel::RSpatialIndexNavel()
{
    init();
}


RSpatialIndexNavel::~RSpatialIndexNavel()
{
    uninit();
}

void RSpatialIndexNavel::init()
{
    buff = SpatialIndex::StorageManager::createNewMemoryStorageManager();

    SpatialIndex::id_type indexIdentifier;
    int dimension = 3;

    // 0.7: results in much slower deletes
    // 0.1: crashes randomly with 'InvalidPageException: Unknown page id ...'
    // 0.2: seems to work reasonably well
    double minimumLoad = 0.2;
    // 100: slower for inserts
    size_t indexCapacity = 50;
    // 100: slower for inserts
    size_t leafCapacity = 50;
    tree = SpatialIndex::RTree::createNewRTree(
            *buff, minimumLoad, indexCapacity, leafCapacity, dimension,
            SpatialIndex::RTree::RV_RSTAR, indexIdentifier);
    //sidCounter = 0;
}


void RSpatialIndexNavel::uninit()
{
    delete tree;
    delete buff;
    tree = NULL;
    buff = NULL;
    //sidCounter = 0;
}

RSpatialIndex *RSpatialIndexNavel::create() { return new RSpatialIndexNavel(); }

void RSpatialIndexNavel::clear()
{
    uninit();
    init();
}

void RSpatialIndexNavel::bulkLoad(const QList<int> &ids,
                                  const QList<QList<RBox>> &bbs)
{
    if (ids.isEmpty() || bbs.isEmpty())
    {
        clear();
        return;
    }

    try
    {
        RSiDataStream stream(ids, bbs);
        uninit();

        buff = SpatialIndex::StorageManager::createNewMemoryStorageManager();
        SpatialIndex::id_type indexIdentifier;
        tree = SpatialIndex::RTree::createAndBulkLoadNewRTree(
                SpatialIndex::RTree::BLM_STR, stream, *buff, 0.2, 50, 50, 3,
                SpatialIndex::RTree::RV_RSTAR, indexIdentifier);
    }
    catch (Tools::IllegalArgumentException e)
    {
        qWarning() << "caught exception in spatial index: " << e.what().c_str();
    }
}

/**
 * Adds an item to the index.
 *
 * \param id ID of the item.
 * \param x1 lower value of the first dimension.
 * \param y1 lower value of the second dimension.
 * \param z1 lower value of the third dimension.
 * \param x2 higher value of the first dimension.
 * \param y2 higher value of the second dimension.
 * \param z2 higher value of the third dimension.
 * \param dataLength Length of the data in bytes or 0.
 * \param data Pointer to the data associated with the item. The
 *      index makes a deep copy of the data, the caller is responsible 
 *      for deleting the data. NULL indicates that the item data is stored
 *      externally (e.g. in a map or list).
 */
void RSpatialIndexNavel::addToIndex(int id, int pos, double x1, double y1,
                                    double z1, double x2, double y2, double z2)
{

    //qDebug() << "RSpatialIndexNavel::addToIndex: id: " << id << ", pos: " << pos << ", " << x1 << "," << y1 << "/" << x2 << "," << y2;

    //qDebug() << "\tbefore: " << *this;

    if (RMath::isNaN(x1) || RMath::isNaN(y1) || RMath::isNaN(z1) ||
        RMath::isNaN(x2) || RMath::isNaN(y2) || RMath::isNaN(z2))
    {

        qWarning() << "trying to add NaN values to spatial index";
        return;
    }

    addToIndex(id, pos,
               RSpatialIndexNavel::RSiRegion(qMin(x1, x2), qMin(y1, y2),
                                             qMin(z1, z2), qMax(x1, x2),
                                             qMax(y1, y2), qMax(z1, z2))
               //dataLength,
               //data
    );

    //qDebug() << "\tafter: " << *this << "\n\n";
}

void RSpatialIndexNavel::addToIndex(int id, int pos, const RBox &bb)
{
    RSpatialIndex::addToIndex(id, pos, bb);
}

/**
 * Internal.
 */
void RSpatialIndexNavel::addToIndex(int id, int pos, const RSiRegion &region,
                                    size_t dataLength, const uint8_t *data)
{
    tree->insertData(dataLength, data, region, RSpatialIndex::getSIId(id, pos));
}

bool RSpatialIndexNavel::removeFromIndex(int id, int pos, const RBox &bb)
{
    return RSpatialIndex::removeFromIndex(id, pos, bb);
}

bool RSpatialIndexNavel::removeFromIndex(int id, const QList<RBox> &bb)
{
    bool ok = true;
    for (int pos = 0; pos < bb.size(); ++pos)
    {
        ok = removeFromIndex(id, pos, bb[pos]) && ok;
    }
    return ok;
}

bool RSpatialIndexNavel::removeFromIndex(int id, int pos, double x1, double y1,
                                         double z1, double x2, double y2,
                                         double z2)
{

    double p1[] = {qMin(x1, x2), qMin(y1, y2), qMin(z1, z2)};
    double p2[] = {qMax(x1, x2), qMax(y1, y2), qMax(z1, z2)};
    SpatialIndex::Region r(p1, p2, 3);
    if (tree->deleteData(r, RSpatialIndex::getSIId(id, pos)))
    {
        return true;
    }
    return false;
}

QMap<int, QSet<int>>
RSpatialIndexNavel::queryIntersected(double x1, double y1, double z1, double x2,
                                     double y2, double z2,
                                     RSpatialIndexVisitor *dataVisitor)
{

    return queryIntersected(
            RSpatialIndexNavel::RSiRegion(x1, y1, z1, x2, y2, z2), dataVisitor);
}


/**
 * Internal.
 */
QMap<int, QSet<int>> RSpatialIndexNavel::queryIntersected(
        const RSpatialIndexNavel::RSiRegion &region,
        RSpatialIndexVisitor *dataVisitor)
{

    QMap<int, QSet<int>> result;
    RSpatialIndexNavel::Visitor visitor(result, dataVisitor);
    tree->intersectsWithQuery(region, visitor);

    return result;
}


QMap<int, QSet<int>>
RSpatialIndexNavel::queryContained(double x1, double y1, double z1, double x2,
                                   double y2, double z2,
                                   RSpatialIndexVisitor *dataVisitor)
{

    return queryContained(RSpatialIndexNavel::RSiRegion(x1, y1, z1, x2, y2, z2),
                          dataVisitor);
}


/**
 * Internal.
 */
QMap<int, QSet<int>>
RSpatialIndexNavel::queryContained(const RSpatialIndexNavel::RSiRegion &region,
                                   RSpatialIndexVisitor *dataVisitor)
{

    QMap<int, QSet<int>> result;
    RSpatialIndexNavel::Visitor visitor(result, dataVisitor);
    tree->containsWhatQuery(region, visitor);

    return result;
}


QMap<int, QSet<int>>
RSpatialIndexNavel::queryNearestNeighbor(unsigned int k, double x, double y,
                                         double z,
                                         RSpatialIndexVisitor *dataVisitor)
{

    return queryNearestNeighbor(k, RSpatialIndexNavel::RSiPoint(x, y, z),
                                dataVisitor);
}


/**
 * Internal.
 */
QMap<int, QSet<int>>
RSpatialIndexNavel::queryNearestNeighbor(unsigned int k, const RSiPoint &point,
                                         RSpatialIndexVisitor *dataVisitor)
{

    QMap<int, QSet<int>> result;
    RSpatialIndexNavel::Visitor visitor(result, dataVisitor);
    tree->nearestNeighborQuery(k, point, visitor);

    return result;
}

QPair<int, int> RSpatialIndexNavel::queryNearestNeighbor(double x, double y,
                                                         double z)
{
    return RSpatialIndex::queryNearestNeighbor(x, y, z);
}