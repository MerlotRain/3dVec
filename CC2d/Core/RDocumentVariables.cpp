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
#include "RDocumentVariables.h"
#include "RDocument.h"

RPropertyTypeId RDocumentVariables::PropertyCustom;
RPropertyTypeId RDocumentVariables::PropertyHandle;
RPropertyTypeId RDocumentVariables::PropertyProtected;
RPropertyTypeId RDocumentVariables::PropertyCurrentLayerId;
RPropertyTypeId RDocumentVariables::PropertyUnit;
RPropertyTypeId RDocumentVariables::PropertyLinetypeScale;
RPropertyTypeId RDocumentVariables::PropertyDimensionFont;
RPropertyTypeId RDocumentVariables::PropertyWorkingSetBlockReferenceId;

RDocumentVariables::RDocumentVariables(RDocument *document)
    : RObject(document), currentLayerId(RLayer::INVALID_ID), unit(RS::None),
      measurement(RS::UnknownMeasurement), linetypeScale(1.0),
      workingSetBlockReferenceId(RObject::INVALID_ID)
{
}

RDocumentVariables::~RDocumentVariables() {}

void RDocumentVariables::init()
{
    RDocumentVariables::PropertyCustom.generateId(RDocumentVariables::getRtti(),
                                                  RObject::PropertyCustom);
    RDocumentVariables::PropertyHandle.generateId(RDocumentVariables::getRtti(),
                                                  RObject::PropertyHandle);
    RDocumentVariables::PropertyProtected.generateId(
            RDocumentVariables::getRtti(), RObject::PropertyProtected);
    RDocumentVariables::PropertyCurrentLayerId.generateId(
            RDocumentVariables::getRtti(), "",
            QT_TRANSLATE_NOOP("RDocumentVariables", "Current Layer ID"));
    RDocumentVariables::PropertyUnit.generateId(
            RDocumentVariables::getRtti(), "",
            QT_TRANSLATE_NOOP("RDocumentVariables", "Drawing Unit"));
    RDocumentVariables::PropertyLinetypeScale.generateId(
            RDocumentVariables::getRtti(), "",
            QT_TRANSLATE_NOOP("RDocumentVariables", "Linetype Scale"));
    RDocumentVariables::PropertyDimensionFont.generateId(
            RDocumentVariables::getRtti(), "",
            QT_TRANSLATE_NOOP("RDocumentVariables", "Dimension Font"));
    RDocumentVariables::PropertyWorkingSetBlockReferenceId.generateId(
            RDocumentVariables::getRtti(), "",
            "Working Set Block Reference Id");
}

QPair<QVariant, RPropertyAttributes>
RDocumentVariables::getProperty(RPropertyTypeId &propertyTypeId,
                                bool humanReadable, bool noAttributes,
                                bool showOnRequest)
{

    if (propertyTypeId == PropertyCurrentLayerId)
    {
        return qMakePair(QVariant(currentLayerId), RPropertyAttributes());
    }

    if (propertyTypeId == PropertyUnit)
    {
        return qMakePair(QVariant(unit), RPropertyAttributes());
    }

    if (propertyTypeId == PropertyLinetypeScale)
    {
        return qMakePair(QVariant(linetypeScale), RPropertyAttributes());
    }

    if (propertyTypeId == PropertyDimensionFont)
    {
        return qMakePair(QVariant(dimensionFont), RPropertyAttributes());
    }

    if (propertyTypeId == PropertyWorkingSetBlockReferenceId)
    {
        return qMakePair(QVariant(workingSetBlockReferenceId),
                         RPropertyAttributes());
    }

    return RObject::getProperty(propertyTypeId, humanReadable, noAttributes,
                                showOnRequest);
}

bool RDocumentVariables::setProperty(RPropertyTypeId propertyTypeId,
                                     const QVariant &value,
                                     RTransaction *transaction)
{

    bool ret = false;

    ret = ret || RObject::setMember(currentLayerId, value,
                                    PropertyCurrentLayerId == propertyTypeId);
    ret = ret || RObject::setMember((int &) unit, value,
                                    PropertyUnit == propertyTypeId);
    ret = ret || RObject::setMember(linetypeScale, value,
                                    PropertyLinetypeScale == propertyTypeId);
    ret = ret || RObject::setMember(dimensionFont, value,
                                    PropertyDimensionFont == propertyTypeId);
    ret = ret || RObject::setMember(workingSetBlockReferenceId, value,
                                    PropertyWorkingSetBlockReferenceId ==
                                            propertyTypeId);

    ret = ret || RObject::setProperty(propertyTypeId, value, transaction);

    return ret;
}

void RDocumentVariables::clear() { knownVariables.clear(); }

QString RDocumentVariables::addAutoVariable(double value)
{
    int c = getCustomIntProperty("QCAD", "AutoVariableCounter", 0);
    c++;

    QString key = QString("d%1").arg(c);

    setCustomProperty("QCAD", key, value);
    setCustomProperty("QCAD", "AutoVariableCounter", c);

    return key;
}

QStringList RDocumentVariables::getAutoVariables() const
{
    QStringList ret;
    int c = getCustomIntProperty("QCAD", "AutoVariableCounter", 0);
    QString key;
    for (int i = 1; i <= c; i++)
    {
        key = QString("d%1").arg(i);
        if (hasCustomProperty("QCAD", key)) { ret.append(key); }
    }
    return ret;
}
