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
#include "RViewportEntity.h"
#include "RStorage.h"

RPropertyTypeId RViewportEntity::PropertyHandle;
RPropertyTypeId RViewportEntity::PropertyType;
RPropertyTypeId RViewportEntity::PropertyBlock;
RPropertyTypeId RViewportEntity::PropertyLayer;
RPropertyTypeId RViewportEntity::PropertyLinetype;
RPropertyTypeId RViewportEntity::PropertyLinetypeScale;
RPropertyTypeId RViewportEntity::PropertyLineweight;
RPropertyTypeId RViewportEntity::PropertyColor;
RPropertyTypeId RViewportEntity::PropertyDisplayedColor;
RPropertyTypeId RViewportEntity::PropertyDrawOrder;

RPropertyTypeId RViewportEntity::PropertyCenterX;
RPropertyTypeId RViewportEntity::PropertyCenterY;
RPropertyTypeId RViewportEntity::PropertyCenterZ;
RPropertyTypeId RViewportEntity::PropertyWidth;
RPropertyTypeId RViewportEntity::PropertyHeight;
RPropertyTypeId RViewportEntity::PropertyScale;
RPropertyTypeId RViewportEntity::PropertyRotation;
RPropertyTypeId RViewportEntity::PropertyOn;
RPropertyTypeId RViewportEntity::PropertyViewCenterX;
RPropertyTypeId RViewportEntity::PropertyViewCenterY;
RPropertyTypeId RViewportEntity::PropertyViewTargetX;
RPropertyTypeId RViewportEntity::PropertyViewTargetY;
RPropertyTypeId RViewportEntity::PropertyViewTargetZ;

RPropertyTypeId RViewportEntity::PropertyOverall;


RViewportEntity::RViewportEntity(RDocument *document, const RViewportData &data)
    : REntity(document), data(document, data)
{
}

RViewportEntity::~RViewportEntity() {}

void RViewportEntity::init()
{
    RViewportEntity::PropertyHandle.generateId(RViewportEntity::getRtti(),
                                               RObject::PropertyHandle);
    RViewportEntity::PropertyType.generateId(RViewportEntity::getRtti(),
                                             REntity::PropertyType);
    RViewportEntity::PropertyBlock.generateId(RViewportEntity::getRtti(),
                                              REntity::PropertyBlock);
    RViewportEntity::PropertyLayer.generateId(RViewportEntity::getRtti(),
                                              REntity::PropertyLayer);
    RViewportEntity::PropertyLinetype.generateId(RViewportEntity::getRtti(),
                                                 REntity::PropertyLinetype);
    RViewportEntity::PropertyLinetypeScale.generateId(
            RViewportEntity::getRtti(), REntity::PropertyLinetypeScale);
    RViewportEntity::PropertyLineweight.generateId(RViewportEntity::getRtti(),
                                                   REntity::PropertyLineweight);
    RViewportEntity::PropertyColor.generateId(RViewportEntity::getRtti(),
                                              REntity::PropertyColor);
    RViewportEntity::PropertyDisplayedColor.generateId(
            RViewportEntity::getRtti(), REntity::PropertyDisplayedColor);
    RViewportEntity::PropertyDrawOrder.generateId(RViewportEntity::getRtti(),
                                                  REntity::PropertyDrawOrder);

    RViewportEntity::PropertyCenterX.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyCenterY.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyCenterZ.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Position"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyWidth.generateId(
            RViewportEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Width"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyHeight.generateId(
            RViewportEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Height"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyScale.generateId(
            RViewportEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Scale"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyRotation.generateId(
            RViewportEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Rotation"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyOn.generateId(RViewportEntity::getRtti(), "",
                                           QT_TRANSLATE_NOOP("REntity", "On"));
    RViewportEntity::PropertyViewCenterX.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "View Center"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyViewCenterY.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "View Center"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyViewTargetX.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "View Target"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyViewTargetY.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "View Target"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RViewportEntity::PropertyViewTargetZ.generateId(
            RViewportEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "View Target"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);

    RViewportEntity::PropertyOverall.generateId(
            RViewportEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Overall"));
}

bool RViewportEntity::setProperty(RPropertyTypeId propertyTypeId,
                                  const QVariant &value,
                                  RTransaction *transaction)
{
    bool ret = REntity::setProperty(propertyTypeId, value, transaction);
    ret = ret || RObject::setMember(data.position.x, value,
                                    PropertyCenterX == propertyTypeId);
    ret = ret || RObject::setMember(data.position.y, value,
                                    PropertyCenterY == propertyTypeId);
    ret = ret || RObject::setMember(data.position.z, value,
                                    PropertyCenterZ == propertyTypeId);
    ret = ret || RObject::setMember(data.width, value,
                                    PropertyWidth == propertyTypeId);
    ret = ret || RObject::setMember(data.height, value,
                                    PropertyHeight == propertyTypeId);
    ret = ret || RObject::setMember(data.scaleFactor, value,
                                    PropertyScale == propertyTypeId);
    ret = ret || RObject::setMember(data.rotation, value,
                                    PropertyRotation == propertyTypeId);
    ret = ret || RObject::setMember(data.viewCenter.x, value,
                                    PropertyViewCenterX == propertyTypeId);
    ret = ret || RObject::setMember(data.viewCenter.y, value,
                                    PropertyViewCenterY == propertyTypeId);
    ret = ret || RObject::setMember(data.viewTarget.x, value,
                                    PropertyViewTargetX == propertyTypeId);
    ret = ret || RObject::setMember(data.viewTarget.y, value,
                                    PropertyViewTargetY == propertyTypeId);
    ret = ret || RObject::setMember(data.viewTarget.z, value,
                                    PropertyViewTargetZ == propertyTypeId);
    ret = ret || RObject::setMember(data.overall, value,
                                    PropertyOverall == propertyTypeId);

    if (PropertyOn == propertyTypeId)
    {
        bool on = !data.isOff();
        ret = ret || RObject::setMember(on, value);
        data.setOff(!on);
    }

    return ret;
}

QPair<QVariant, RPropertyAttributes>
RViewportEntity::getProperty(RPropertyTypeId &propertyTypeId,
                             bool humanReadable, bool noAttributes,
                             bool showOnRequest)
{

    if (propertyTypeId == PropertyCenterX)
    {
        return qMakePair(QVariant(data.position.x), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyCenterY)
    {
        return qMakePair(QVariant(data.position.y), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyCenterZ)
    {
        return qMakePair(QVariant(data.position.z), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyWidth)
    {
        return qMakePair(QVariant(data.width), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyHeight)
    {
        return qMakePair(QVariant(data.height), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyScale)
    {
        return qMakePair(QVariant(data.scaleFactor),
                         RPropertyAttributes(RPropertyAttributes::Scale));
    }
    else if (propertyTypeId == PropertyRotation)
    {
        return qMakePair(QVariant(data.rotation),
                         RPropertyAttributes(RPropertyAttributes::Angle));
    }
    else if (propertyTypeId == PropertyOn)
    {
        return qMakePair(QVariant(!data.isOff()), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyViewCenterX)
    {
        return qMakePair(QVariant(data.viewCenter.x), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyViewCenterY)
    {
        return qMakePair(QVariant(data.viewCenter.y), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyViewTargetX)
    {
        return qMakePair(QVariant(data.viewTarget.x), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyViewTargetY)
    {
        return qMakePair(QVariant(data.viewTarget.y), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyViewTargetZ)
    {
        return qMakePair(QVariant(data.viewTarget.z), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyOverall)
    {
        return qMakePair(QVariant(data.overall),
                         RPropertyAttributes(RPropertyAttributes::ReadOnly |
                                             RPropertyAttributes::Invisible));
    }
    return REntity::getProperty(propertyTypeId, humanReadable, noAttributes,
                                showOnRequest);
}

void RViewportEntity::setData(RViewportData &d)
{
    data = d;
    data.setDocument(getDocument());
}
