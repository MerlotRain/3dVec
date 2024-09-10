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
#include "RBlock.h"
#include "RDocument.h"

const QString RBlock::modelSpaceName = "*Model_Space";
const QString RBlock::paperSpaceName = "*Paper_Space";

RPropertyTypeId RBlock::PropertyType;
RPropertyTypeId RBlock::PropertyHandle;
RPropertyTypeId RBlock::PropertyName;
RPropertyTypeId RBlock::PropertyFrozen;
RPropertyTypeId RBlock::PropertyPixelUnit;
RPropertyTypeId RBlock::PropertyOriginX;
RPropertyTypeId RBlock::PropertyOriginY;
RPropertyTypeId RBlock::PropertyOriginZ;

RBlock::RBlock()
    : RObject(), frozen(false), anonymous(false), pixelUnit(false),
      origin(RVector::invalid)
{
}

RBlock::RBlock(RDocument *document, const QString &name, const RVector &origin)
    : RObject(document), name(name.trimmed()), frozen(false), anonymous(false),
      pixelUnit(false), origin(origin)
{
}

RBlock::~RBlock() {}

void RBlock::init()
{
    RBlock::PropertyType.generateId(RBlock::getRtti(), RObject::PropertyType);
    RBlock::PropertyHandle.generateId(RBlock::getRtti(),
                                      RObject::PropertyHandle);
    RBlock::PropertyName.generateId(RBlock::getRtti(), "",
                                    QT_TRANSLATE_NOOP("REntity", "Name"));
    RBlock::PropertyFrozen.generateId(RBlock::getRtti(), "",
                                      QT_TRANSLATE_NOOP("REntity", "Hidden"));
    RBlock::PropertyPixelUnit.generateId(
            RBlock::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Pixel Unit"));
    RBlock::PropertyOriginX.generateId(RBlock::getRtti(),
                                       QT_TRANSLATE_NOOP("REntity", "Origin"),
                                       QT_TRANSLATE_NOOP("REntity", "X"));
    RBlock::PropertyOriginY.generateId(RBlock::getRtti(),
                                       QT_TRANSLATE_NOOP("REntity", "Origin"),
                                       QT_TRANSLATE_NOOP("REntity", "Y"));
    RBlock::PropertyOriginZ.generateId(RBlock::getRtti(),
                                       QT_TRANSLATE_NOOP("REntity", "Origin"),
                                       QT_TRANSLATE_NOOP("REntity", "Z"));
}

RBlock *RBlock::clone() const { return new RBlock(*this); }

void RBlock::setName(const QString &n) { name = n.trimmed(); }

bool RBlock::setProperty(RPropertyTypeId propertyTypeId, const QVariant &value,
                         RTransaction *transaction)
{
    bool ret = RObject::setProperty(propertyTypeId, value, transaction);

    if (PropertyName == propertyTypeId)
    {
        // never change name of blocks starting with * (model space, paper space, ...):
        if (name.startsWith("*")) { return false; }
        // never change block name to empty string:
        if (value.toString().isEmpty()) { return false; }
    }

    ret = ret || RObject::setMember(name, value.toString().trimmed(),
                                    PropertyName == propertyTypeId);
    ret = ret ||
          RObject::setMember(frozen, value, PropertyFrozen == propertyTypeId);
    ret = ret || RObject::setMember(pixelUnit, value,
                                    PropertyPixelUnit == propertyTypeId);
    ret = ret || RObject::setMember(origin.x, value,
                                    PropertyOriginX == propertyTypeId);
    ret = ret || RObject::setMember(origin.y, value,
                                    PropertyOriginY == propertyTypeId);
    ret = ret || RObject::setMember(origin.z, value,
                                    PropertyOriginZ == propertyTypeId);

    return ret;
}

QPair<QVariant, RPropertyAttributes>
RBlock::getProperty(RPropertyTypeId &propertyTypeId, bool humanReadable,
                    bool noAttributes, bool showOnRequest)
{
    if (propertyTypeId == PropertyName)
    {
        return qMakePair(QVariant(name), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyFrozen)
    {
        return qMakePair(QVariant(frozen), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyPixelUnit)
    {
        return qMakePair(QVariant(pixelUnit),
                         RPropertyAttributes(RPropertyAttributes::Invisible));
    }
    else if (propertyTypeId == PropertyOriginX)
    {
        return qMakePair(QVariant(origin.x), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyOriginY)
    {
        return qMakePair(QVariant(origin.y), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyOriginZ)
    {
        return qMakePair(QVariant(origin.z), RPropertyAttributes());
    }

    return RObject::getProperty(propertyTypeId, humanReadable, noAttributes,
                                showOnRequest);
}

void RBlock::setCustomProperty(const QString &title, const QString &key,
                               const QVariant &value)
{
    // set custom property PixelUnit as flag:
    if (title == "QCAD" && key == "PixelUnit")
    {
        if (value.toString() == "1")
        {
            pixelUnit = true;
            return;
        }
    }

    RObject::setCustomProperty(title, key, value);
}