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

#ifndef RSPATIALINDEXNAVEL_H
#define RSPATIALINDEXNAVEL_H

#include "CC2dCoreExport.h"

#include <QList>
#include <QSet>
#include <QtGlobal>
#include "RSpatialIndex.h"
#include <spatialindex/SpatialIndex.h>

/**
 * \brief Spatial index class. 
 * Each instance of this class offers a spatial index.
 * This class wraps the API of the "spatial index" library.
 *
 * \ingroup spatialindex
 * \scriptable
 */
class QCADCORE_EXPORT RSpatialIndexNavel : public RSpatialIndex
{
protected:
    /**
     * Internal representation of a rectangular region.
     * Depends on the external spatial index library.
     *
     * \ingroup spatialindex
     */
    class RSiRegion : public SpatialIndex::Region
    {
    public:
        RSiRegion(double x1, double y1, double z1, double x2, double y2,
                  double z2);
    };

    /**
     * Internal representation of a point.
     * Depends on the external spatial index library.
     *
     * \ingroup spatialindex
     */
    class RSiPoint : public SpatialIndex::Point
    {
    public:
        RSiPoint(double x, double y, double z);
    };

    /**
     * \brief Internal node and data visitor.
     * Depends on the external spatial index library.
     * \see RSpatialIndexVisitor
     *
     * \ingroup spatialindex
     */
    class Visitor : public SpatialIndex::IVisitor
    {
    public:
        Visitor(QMap<int, QSet<int>> &ids,
                RSpatialIndexVisitor *dataVisitor = NULL)
            : ids(ids), dataVisitor(dataVisitor)
        {
        }

        void visitNode(const SpatialIndex::INode &n)
        {
            //printf("Visitor::visitNode: %Ld\n", n.getIdentifier());
            if (dataVisitor != NULL)
            {
                SpatialIndex::IShape *shape;
                n.getShape(&shape);
                if (shape == NULL)
                {
                    fprintf(stderr, "error: node has no shape\n");
                    return;
                }
                SpatialIndex::Region *region =
                        dynamic_cast<SpatialIndex::Region *>(shape);
                if (region == NULL)
                {
                    fprintf(stderr, "error: node shape in spacial index is not "
                                    "a SpatialIndex::Region\n");
                    return;
                }
                dataVisitor->visitNode(region->m_pLow[0], region->m_pLow[1],
                                       region->m_pLow[2], region->m_pHigh[0],
                                       region->m_pHigh[1], region->m_pHigh[2]);
            }
        }

        void visitData(const SpatialIndex::IData &d)
        {
            qint64 siid = d.getIdentifier();
            int id = RSpatialIndex::getId(siid);
            int pos = RSpatialIndex::getPos(siid);

            //if (ids.contains(id)) {
            ids[id].insert(pos);
            /*
                qFatal("Visitor.visitData: double entry found in spatial index, identifier: %d", 
                    (int)d.getIdentifier());
                Q_ASSERT(false);
                */
            //            }
            //            else {
            //                ids.insert(id, QSet<int>() << pos);
            //            }

            //ids.insert(d.getIdentifier());

            if (dataVisitor != NULL)
            {
                SpatialIndex::IShape *shape;
                d.getShape(&shape);
                if (shape == NULL)
                {
                    fprintf(stderr, "error: data node has no shape\n");
                    return;
                }
                SpatialIndex::Region *region =
                        dynamic_cast<SpatialIndex::Region *>(shape);
                if (region == NULL)
                {
                    fprintf(stderr, "error: shape in spacial index is not a "
                                    "SpatialIndex::Region\n");
                    return;
                }

                //size_t len;
                //uint8_t* data;
                //d.getData(len, &data);
                dataVisitor->visitData(id, pos, region->m_pLow[0],
                                       region->m_pLow[1], region->m_pLow[2],
                                       region->m_pHigh[0], region->m_pHigh[1],
                                       region->m_pHigh[2]);
                //region->getX1(), region->getY1(), region->getZ1(),
                //region->getX2(), region->getY2(), region->getZ2(),
                //len, data);
                //delete data;
                //delete region;
                delete shape;
            }
        }

        void visitData(std::vector<const SpatialIndex::IData *> &v)
        {
            std::vector<const SpatialIndex::IData *>::iterator it;
            for (it = v.begin(); it != v.end(); it++)
            {
                printf("Visitor::visitData[]: %lld\n",
                       (long long int) (*it)->getIdentifier());
            }
        }

    private:
        QMap<int, QSet<int>> &ids;
        RSpatialIndexVisitor *dataVisitor;
    };

public:
    RSpatialIndexNavel();
    ~RSpatialIndexNavel();

    //static int dataToInt(const uint8_t* data);
    //static void intToData(int i, uint8_t* data);

    virtual RSpatialIndex *create();
    virtual void clear();

    virtual void bulkLoad(const QList<int> &ids, const QList<QList<RBox>> &bbs);

    virtual void addToIndex(int id, int pos, double x1, double y1, double z1,
                            double x2, double y2, double z2);

    virtual void addToIndex(int id, int pos, const RBox &bb);

    //void removeFromIndex(int id);
    virtual bool removeFromIndex(int id, const QList<RBox> &bb);
    virtual bool removeFromIndex(int id, int pos, const RBox &bb);
    virtual bool removeFromIndex(int id, int pos, double x1, double y1,
                                 double z1, double x2, double y2, double z2);

    virtual QMap<int, QSet<int>>
    queryIntersected(double x1, double y1, double z1, double x2, double y2,
                     double z2, RSpatialIndexVisitor *dataVisitor = NULL);
    QList<int> queryIntersectedSimple(const RBox &b)
    {
        return RSpatialIndex::queryIntersected(b).keys();
    }
    virtual QMap<int, QSet<int>>
    queryContained(double x1, double y1, double z1, double x2, double y2,
                   double z2, RSpatialIndexVisitor *dataVisitor = NULL);

    virtual QMap<int, QSet<int>>
    queryNearestNeighbor(unsigned int k, double x, double y, double z,
                         RSpatialIndexVisitor *dataVisitor = NULL);

    virtual QPair<int, int> queryNearestNeighbor(double x, double y, double z);

protected:
    /**
     * \nonscriptable
     */
    void addToIndex(int id, int pos, const RSiRegion &region,
                    size_t dataLength = 0, const uint8_t *data = NULL);
    /**
     * \nonscriptable
     */
    QMap<int, QSet<int>>
    queryIntersected(const RSiRegion &region,
                     RSpatialIndexVisitor *dataVisitor = NULL);
    /**
     * \nonscriptable
     */
    QMap<int, QSet<int>>
    queryContained(const RSiRegion &region,
                   RSpatialIndexVisitor *dataVisitor = NULL);
    /**
     * \nonscriptable
     */
    QMap<int, QSet<int>>
    queryNearestNeighbor(unsigned int k, const RSiPoint &point,
                         RSpatialIndexVisitor *dataVisitor = NULL);

    //    QSet<int> sidsToIds(const QSet<int> sids);
    //    QSet<int> idToSids(int id);

protected:
    void init();
    void uninit();

protected:
    SpatialIndex::ISpatialIndex *tree;
    SpatialIndex::IStorageManager *buff;

    //    QMap<int, int> sidToId;
    //    QMap<int, QList<int> > idToSid;
    //    int sidCounter;
};

Q_DECLARE_METATYPE(RSpatialIndexNavel *)

#endif