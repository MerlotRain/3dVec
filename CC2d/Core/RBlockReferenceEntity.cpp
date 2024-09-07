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
#include "RBlockReferenceEntity.h"
#include "RStorage.h"

RPropertyTypeId RBlockReferenceEntity::PropertyCustom;
RPropertyTypeId RBlockReferenceEntity::PropertyHandle;
RPropertyTypeId RBlockReferenceEntity::PropertyProtected;
RPropertyTypeId RBlockReferenceEntity::PropertyWorkingSet;
RPropertyTypeId RBlockReferenceEntity::PropertyType;
RPropertyTypeId RBlockReferenceEntity::PropertyBlock;
RPropertyTypeId RBlockReferenceEntity::PropertyLayer;
RPropertyTypeId RBlockReferenceEntity::PropertyLinetype;
RPropertyTypeId RBlockReferenceEntity::PropertyLinetypeScale;
RPropertyTypeId RBlockReferenceEntity::PropertyLineweight;
RPropertyTypeId RBlockReferenceEntity::PropertyColor;
RPropertyTypeId RBlockReferenceEntity::PropertyDisplayedColor;
RPropertyTypeId RBlockReferenceEntity::PropertyDrawOrder;

RPropertyTypeId RBlockReferenceEntity::PropertyReferencedBlock;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionX;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionY;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionZ;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleX;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleY;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleZ;
RPropertyTypeId RBlockReferenceEntity::PropertyRotation;
RPropertyTypeId RBlockReferenceEntity::PropertyColumnCount;
RPropertyTypeId RBlockReferenceEntity::PropertyRowCount;
RPropertyTypeId RBlockReferenceEntity::PropertyColumnSpacing;
RPropertyTypeId RBlockReferenceEntity::PropertyRowSpacing;


RBlockReferenceEntity::RBlockReferenceEntity(RDocument *document,
                                             const RBlockReferenceData &data)
    : REntity(document), data(document, data), recursionDepth(0)
{
    //  if (data.getDocument() == NULL) {
    //      return;
    //  }
    //  RBlock::Id id = &this->data.blockId;
    //  while (data.getDocument()->getBlockName(id) != "*Model_Space") {
    //      QSharedPointer<RBlock> block = data.getDocument()->queryBlock(id);
    //      if (block.isNull()) {
    //          break;Y
    //      }
    //      id = block->getId();
    //  }
}

RBlockReferenceEntity::RBlockReferenceEntity(const RBlockReferenceEntity &other)
    : REntity(other)
{
    data = other.data;
}

RBlockReferenceEntity::~RBlockReferenceEntity() {}

void RBlockReferenceEntity::init()
{
    RBlockReferenceEntity::PropertyCustom.generateId(
            RBlockReferenceEntity::getRtti(), RObject::PropertyCustom);
    RBlockReferenceEntity::PropertyHandle.generateId(
            RBlockReferenceEntity::getRtti(), RObject::PropertyHandle);
    RBlockReferenceEntity::PropertyProtected.generateId(
            RBlockReferenceEntity::getRtti(), RObject::PropertyProtected);
    RBlockReferenceEntity::PropertyWorkingSet.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyWorkingSet);
    RBlockReferenceEntity::PropertyType.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyType);
    RBlockReferenceEntity::PropertyBlock.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyBlock);
    RBlockReferenceEntity::PropertyLayer.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyLayer);
    RBlockReferenceEntity::PropertyLinetype.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyLinetype);
    RBlockReferenceEntity::PropertyLinetypeScale.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyLinetypeScale);
    RBlockReferenceEntity::PropertyLineweight.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyLineweight);
    RBlockReferenceEntity::PropertyColor.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyColor);
    RBlockReferenceEntity::PropertyDisplayedColor.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyDisplayedColor);
    RBlockReferenceEntity::PropertyDrawOrder.generateId(
            RBlockReferenceEntity::getRtti(), REntity::PropertyDrawOrder);

    RBlockReferenceEntity::PropertyReferencedBlock.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Referenced Block"));
    RBlockReferenceEntity::PropertyPositionX.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyPositionY.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyPositionZ.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyScaleX.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Scale"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyScaleY.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Scale"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyScaleZ.generateId(
            RBlockReferenceEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Scale"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyRotation.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Angle"), false,
            RPropertyAttributes::Geometry);
    RBlockReferenceEntity::PropertyColumnCount.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Columns"));
    RBlockReferenceEntity::PropertyRowCount.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Rows"));
    RBlockReferenceEntity::PropertyColumnSpacing.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Column Spacing"));
    RBlockReferenceEntity::PropertyRowSpacing.generateId(
            RBlockReferenceEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Row Spacing"));

    // make sure title of property attributes group is translated:
    QT_TRANSLATE_NOOP("REntity", "Attributes");
}

QSet<RPropertyTypeId> RBlockReferenceEntity::getPropertyTypeIds(
        RPropertyAttributes::Option option) const
{
    QSet<RPropertyTypeId> ret;

    // TODO: move to RObject?
    // add attribute tag / values as properties of the block reference:
    const RDocument *doc = getDocument();
    if (doc != NULL)
    {
        QSet<REntity::Id> childIds =
                doc->queryChildEntities(getId(), RS::EntityAttribute);
        QSet<REntity::Id>::iterator it;
        for (it = childIds.begin(); it != childIds.end(); it++)
        {
            REntity::Id childId = *it;
            QSharedPointer<REntity> child = doc->queryEntityDirect(childId);
            if (child.isNull()) { continue; }
            if (child->isUndone()) { continue; }

            // get block attribute properties:
            QSet<RPropertyTypeId> childProperties =
                    child->getPropertyTypeIds(option);
            QSet<RPropertyTypeId>::iterator it2;
            for (it2 = childProperties.begin(); it2 != childProperties.end();
                 it2++)
            {
                RPropertyTypeId pid = *it2;
                QPair<QVariant, RPropertyAttributes> p =
                        child->getProperty(pid);
                if (p.second.isVisibleToParent())
                {
                    pid.setId(RPropertyTypeId::INVALID_ID);
                    ret.insert(pid);
                }
            }
        }
    }
    else { qWarning() << "document is NULL"; }

    ret.unite(REntity::getPropertyTypeIds(option));

    return ret;
}

bool RBlockReferenceEntity::setProperty(RPropertyTypeId propertyTypeId,
                                        const QVariant &value,
                                        RTransaction *transaction)
{
    bool ret = REntity::setProperty(propertyTypeId, value, transaction);

    //    if (propertyTypeId==PropertyPositionX) {
    //        double d = value - data.position.x;
    //        RObject::setMember(data.position.x, value);

    //        const RDocument* doc = getDocument();
    //        if (doc!=NULL) {
    //            QSet<REntity::Id> childIds = doc->queryChildEntities(getId(), RS::EntityAttribute);
    //            QSet<REntity::Id>::iterator it;
    //            for (it=childIds.begin(); it!=childIds.end(); it++) {
    //                REntity::Id childId = *it;
    //                QSharedPointer<REntity> child = doc->queryEntity(childId);
    //                if (child.isNull()) {
    //                    continue;
    //                }

    //                QSet<RPropertyTypeId> childProperties = child->getPropertyTypeIds();
    //                QSet<RPropertyTypeId>::iterator it2;
    //                for (it2=childProperties.begin(); it2!=childProperties.end(); it2++) {
    //                    RPropertyTypeId pid = *it2;
    //                    QPair<QVariant, RPropertyAttributes> p = child->getProperty(pid);
    //                    if (p.second.isVisibleToParent() && pid.getCustomPropertyName()==tag) {
    //                        //ret.insert(RPropertyTypeId(QT_TRANSLATE_NOOP("REntity", "Attributes"), p.first.toString()));
    //                        //return qMakePair(QVariant(p.first), RPropertyAttributes());
    //                        child->setProperty(pid, value.toString(), transaction);
    //                        transaction->addObject(child);
    //                    }
    //                }
    //            }
    //        }
    //    }

    ret = ret || RObject::setMember(data.position.x, value,
                                    PropertyPositionX == propertyTypeId);
    ret = ret || RObject::setMember(data.position.y, value,
                                    PropertyPositionY == propertyTypeId);
    ret = ret || RObject::setMember(data.position.z, value,
                                    PropertyPositionZ == propertyTypeId);

    ret = ret || RObject::setMember(data.scaleFactors.x, value,
                                    PropertyScaleX == propertyTypeId &&
                                            fabs(value.toDouble()) >
                                                    RS::PointTolerance);
    ret = ret || RObject::setMember(data.scaleFactors.y, value,
                                    PropertyScaleY == propertyTypeId &&
                                            fabs(value.toDouble()) >
                                                    RS::PointTolerance);
    ret = ret || RObject::setMember(data.scaleFactors.z, value,
                                    PropertyScaleZ == propertyTypeId &&
                                            fabs(value.toDouble()) >
                                                    RS::PointTolerance);

    ret = ret || RObject::setMember(data.rotation, value,
                                    PropertyRotation == propertyTypeId);

    ret = ret || RObject::setMember(data.columnCount, value,
                                    PropertyColumnCount == propertyTypeId);
    ret = ret || RObject::setMember(data.rowCount, value,
                                    PropertyRowCount == propertyTypeId);

    ret = ret || RObject::setMember(data.columnSpacing, value,
                                    PropertyColumnSpacing == propertyTypeId);
    ret = ret || RObject::setMember(data.rowSpacing, value,
                                    PropertyRowSpacing == propertyTypeId);

    if (propertyTypeId == PropertyReferencedBlock)
    {
        if (RS::getMetaType(value) == QVariant::Int ||
            RS::getMetaType(value) == QVariant::LongLong)
        {

            ret = ret || RObject::setMember(getData().referencedBlockId,
                                            value.toInt(), true);
        }
        else if (RS::getMetaType(value) == QVariant::String)
        {
            RDocument *document = getData().getDocument();
            if (document != NULL)
            {
                ret = ret ||
                      RObject::setMember(getData().referencedBlockId,
                                         document->getBlockId(value.toString()),
                                         true);
            }
        }
    }

    // allow editing of attributes in the context of a block reference:
    if (transaction && propertyTypeId.isCustom())
    {
        if (propertyTypeId.getCustomPropertyTitle() == "Attributes")
        {
            QString tag = propertyTypeId.getCustomPropertyName();

            const RDocument *doc = getDocument();
            if (doc != NULL)
            {
                QSet<REntity::Id> childIds =
                        doc->queryChildEntities(getId(), RS::EntityAttribute);
                QSet<REntity::Id>::iterator it;
                for (it = childIds.begin(); it != childIds.end(); it++)
                {
                    REntity::Id childId = *it;
                    QSharedPointer<REntity> child = doc->queryEntity(childId);
                    if (child.isNull()) { continue; }

                    if (child->isUndone()) { continue; }

                    QSet<RPropertyTypeId> childProperties =
                            child->getPropertyTypeIds();
                    QSet<RPropertyTypeId>::iterator it2;
                    for (it2 = childProperties.begin();
                         it2 != childProperties.end(); it2++)
                    {
                        RPropertyTypeId pid = *it2;
                        QPair<QVariant, RPropertyAttributes> p =
                                child->getProperty(pid);
                        if (p.second.isVisibleToParent() &&
                            pid.getCustomPropertyName() == tag)
                        {
                            //ret.insert(RPropertyTypeId(QT_TRANSLATE_NOOP("REntity", "Attributes"), p.first.toString()));
                            //return qMakePair(QVariant(p.first), RPropertyAttributes());
                            child->setProperty(pid, value.toString(),
                                               transaction);
                            transaction->addObject(child, false);
                        }
                    }
                }
            }
        }
    }

    if (ret) { data.update(); }

    return ret;
}

QPair<QVariant, RPropertyAttributes>
RBlockReferenceEntity::getProperty(RPropertyTypeId &propertyTypeId,
                                   bool humanReadable, bool noAttributes,
                                   bool showOnRequest)
{

    if (propertyTypeId == PropertyPositionX)
    {
        return qMakePair(QVariant(data.position.x), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyPositionY)
    {
        return qMakePair(QVariant(data.position.y), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyPositionZ)
    {
        return qMakePair(QVariant(data.position.z), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyScaleX)
    {
        return qMakePair(QVariant(data.scaleFactors.x),
                         RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyScaleY)
    {
        return qMakePair(QVariant(data.scaleFactors.y),
                         RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyScaleZ)
    {
        return qMakePair(QVariant(data.scaleFactors.z),
                         RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyRotation)
    {
        return qMakePair(QVariant(data.rotation),
                         RPropertyAttributes(RPropertyAttributes::Angle));
    }
    else if (propertyTypeId == PropertyColumnCount)
    {
        return qMakePair(QVariant(data.columnCount),
                         RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyRowCount)
    {
        return qMakePair(QVariant(data.rowCount),
                         RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyColumnSpacing)
    {
        return qMakePair(QVariant(data.columnSpacing), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyRowSpacing)
    {
        return qMakePair(QVariant(data.rowSpacing), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyReferencedBlock)
    {
        if (humanReadable)
        {
            RDocument *document = getData().getDocument();
            if (document != NULL)
            {
                RPropertyAttributes attr;
                // TODO
                if (!noAttributes)
                {
                    QSet<QString> blockNames = document->getBlockNames();
                    QSet<QString> filtered;
                    QSet<QString>::iterator it;
                    for (it = blockNames.begin(); it != blockNames.end(); it++)
                    {
                        if (!(*it).startsWith("*")) { filtered.insert(*it); }
                    }
                    attr.setChoices(filtered);
                }
                return qMakePair(QVariant(document->getBlockName(
                                         getData().getReferencedBlockId())),
                                 attr);
            }
        }
        else
        {
            return qMakePair(QVariant(getData().getReferencedBlockId()),
                             RPropertyAttributes());
        }
    }
    else if (propertyTypeId.isCustom())
    {
        if (propertyTypeId.getCustomPropertyTitle() == "Attributes")
        {
            QString tag = propertyTypeId.getCustomPropertyName();

            const RDocument *doc = getDocument();
            if (doc != NULL)
            {
                QSet<REntity::Id> childIds =
                        doc->queryChildEntities(getId(), RS::EntityAttribute);
                QSet<REntity::Id>::iterator it;
                for (it = childIds.begin(); it != childIds.end(); it++)
                {
                    REntity::Id childId = *it;
                    QSharedPointer<REntity> child =
                            doc->queryEntityDirect(childId);
                    if (child.isNull()) { continue; }

                    QSet<RPropertyTypeId> childProperties =
                            child->getPropertyTypeIds();
                    QSet<RPropertyTypeId>::iterator it2;
                    for (it2 = childProperties.begin();
                         it2 != childProperties.end(); it2++)
                    {
                        RPropertyTypeId pid = *it2;
                        QPair<QVariant, RPropertyAttributes> p =
                                child->getProperty(pid);
                        if (p.second.isVisibleToParent() &&
                            pid.getCustomPropertyName() == tag)
                        {
                            //ret.insert(RPropertyTypeId(QT_TRANSLATE_NOOP("REntity", "Attributes"), p.first.toString()));
                            return qMakePair(QVariant(p.first),
                                             RPropertyAttributes());
                        }
                    }
                }
            }

            //document->queryChildEntities(getId());

            //return qMakePair(QVariant("Dummy"), RPropertyAttributes());
        }
    }

    return REntity::getProperty(propertyTypeId, humanReadable, noAttributes,
                                showOnRequest);
}

//void RBlockReferenceEntity::setSelected(bool on) {
//    RDocument* doc = getDocument();
//    if (doc==NULL) {
//        REntity::setSelected(on);
//        return;
//    }

//    QSet<REntity::Id> attributeIds = doc->queryAttributes(getId());
//    qDebug() << "RBlockReferenceEntity::setSelected: attribute IDs: " << attributeIds;
//    QSet<REntity::Id>::iterator it;
//    for (it=attributeIds.begin(); it!=attributeIds.end(); it++) {
//        QSharedPointer<REntity> attribute = doc->queryEntityDirect(*it);
//        if (attribute.isNull()) {
//            qDebug() << "attribute is NULL";
//            continue;
//        }
//        qDebug() << "selecting attribute";
//        attribute->REntity::setSelected(on);
//    }

//    REntity::setSelected(on);
//}
