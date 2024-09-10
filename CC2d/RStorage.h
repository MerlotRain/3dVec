#ifndef RSTORAGE_H
#define RSTORAGE_H

#include "RNonCopyable.h"

class RStorage : public RNonCopyable
{
    RDocument* document;
    int maxDrawOrder;
    RColor currentColor;
    RLineweight::Lineweight currentLineweight;
    RLinetype::Id currentLinetypeId;
    RLayer::Id layer0Id;

    QHash<RObject::Id, QSharedPointer<REntity>> entityMap;
    mutable QHash<REntity::Id, QSharedPointer<REntity>> selectedEntityMap;
    mutable bool selectedEntityMapDirty;
    mutable QHash<REntity::Id, QSharedPointer<REntity>> visibleEntityMap;
    mutable bool visibleEntityMapDirty;
    mutable QHash<RLayer::Id, QSharedPointer<RLayer>> selectedLayerMap;
    mutable bool selectedLayerMapDirty;

    QHash<RLayer::Id, QSharedPointer<RLayer>> layerMap;
    QHash<QString, QSharedPointer<RLayer>> layerNameMap;

    QHash<RLinetype::Id, QSharedPointer<RLinetype> > linetypeMap;
    
public:
    

};

#endif