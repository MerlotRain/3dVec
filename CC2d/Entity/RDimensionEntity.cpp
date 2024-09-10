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
#include "RDimensionEntity.h"
#include "RStorage.h"
#include "RTextData.h"
#include "RTriangle.h"

RPropertyTypeId RDimensionEntity::PropertyHandle;
RPropertyTypeId RDimensionEntity::PropertyProtected;
RPropertyTypeId RDimensionEntity::PropertyWorkingSet;
RPropertyTypeId RDimensionEntity::PropertyType;
RPropertyTypeId RDimensionEntity::PropertyBlock;
RPropertyTypeId RDimensionEntity::PropertyLayer;
RPropertyTypeId RDimensionEntity::PropertyLinetype;
RPropertyTypeId RDimensionEntity::PropertyLinetypeScale;
RPropertyTypeId RDimensionEntity::PropertyLineweight;
RPropertyTypeId RDimensionEntity::PropertyColor;
RPropertyTypeId RDimensionEntity::PropertyDisplayedColor;
RPropertyTypeId RDimensionEntity::PropertyDrawOrder;

RPropertyTypeId RDimensionEntity::PropertyDefinitionPointX;
RPropertyTypeId RDimensionEntity::PropertyDefinitionPointY;
RPropertyTypeId RDimensionEntity::PropertyDefinitionPointZ;
RPropertyTypeId RDimensionEntity::PropertyMiddleOfTextX;
RPropertyTypeId RDimensionEntity::PropertyMiddleOfTextY;
RPropertyTypeId RDimensionEntity::PropertyMiddleOfTextZ;
RPropertyTypeId RDimensionEntity::PropertyText;
RPropertyTypeId RDimensionEntity::PropertyUpperTolerance;
RPropertyTypeId RDimensionEntity::PropertyLowerTolerance;
//RPropertyTypeId RDimensionEntity::PropertyLinearFactor;
//RPropertyTypeId RDimensionEntity::PropertyDimScale;
RPropertyTypeId RDimensionEntity::PropertyDimBlockName;
RPropertyTypeId RDimensionEntity::PropertyAutoTextPos;
RPropertyTypeId RDimensionEntity::PropertyFontName;
RPropertyTypeId RDimensionEntity::PropertyTextRotation;
//RPropertyTypeId RDimensionEntity::PropertyTextColor;
RPropertyTypeId RDimensionEntity::PropertyArrow1Flipped;
RPropertyTypeId RDimensionEntity::PropertyArrow2Flipped;

RPropertyTypeId RDimensionEntity::PropertyExtLineFix;
RPropertyTypeId RDimensionEntity::PropertyExtLineFixLength;

//RPropertyTypeId RDimensionEntity::PropertyHeight;
//RPropertyTypeId RDimensionEntity::PropertyLineSpacingFactor;
//RPropertyTypeId RDimensionEntity::PropertyHAlign;
//RPropertyTypeId RDimensionEntity::PropertyVAlign;

RPropertyTypeId RDimensionEntity::PropertyAutoLabel;
RPropertyTypeId RDimensionEntity::PropertyMeasuredValue;

RPropertyTypeId RDimensionEntity::PropertyDimscale;
RPropertyTypeId RDimensionEntity::PropertyDimlfac;
RPropertyTypeId RDimensionEntity::PropertyDimtxt;
RPropertyTypeId RDimensionEntity::PropertyDimgap;
RPropertyTypeId RDimensionEntity::PropertyDimasz;
//RPropertyTypeId RDimensionEntity::PropertyDimdli;
RPropertyTypeId RDimensionEntity::PropertyDimexe;
RPropertyTypeId RDimensionEntity::PropertyDimexo;
RPropertyTypeId RDimensionEntity::PropertyDimtad;
RPropertyTypeId RDimensionEntity::PropertyDimtih;
RPropertyTypeId RDimensionEntity::PropertyDimtsz;
RPropertyTypeId RDimensionEntity::PropertyDimlunit;
RPropertyTypeId RDimensionEntity::PropertyDimdec;
RPropertyTypeId RDimensionEntity::PropertyDimdsep;
RPropertyTypeId RDimensionEntity::PropertyDimzin;
RPropertyTypeId RDimensionEntity::PropertyDimaunit;
RPropertyTypeId RDimensionEntity::PropertyDimadec;
RPropertyTypeId RDimensionEntity::PropertyDimazin;
RPropertyTypeId RDimensionEntity::PropertyArchTick;
RPropertyTypeId RDimensionEntity::PropertyDimclrt;


RDimensionEntity::RDimensionEntity(RDocument *document) : REntity(document) {}

RDimensionEntity::~RDimensionEntity() {}

void RDimensionEntity::init()
{
    RDimensionEntity::PropertyHandle.generateId(RDimensionEntity::getRtti(),
                                                RObject::PropertyHandle);
    RDimensionEntity::PropertyProtected.generateId(RDimensionEntity::getRtti(),
                                                   RObject::PropertyProtected);
    RDimensionEntity::PropertyWorkingSet.generateId(
            RDimensionEntity::getRtti(), RObject::PropertyWorkingSet);
    RDimensionEntity::PropertyType.generateId(RDimensionEntity::getRtti(),
                                              REntity::PropertyType);
    RDimensionEntity::PropertyBlock.generateId(RDimensionEntity::getRtti(),
                                               REntity::PropertyBlock);
    RDimensionEntity::PropertyLayer.generateId(RDimensionEntity::getRtti(),
                                               REntity::PropertyLayer);
    RDimensionEntity::PropertyLinetype.generateId(RDimensionEntity::getRtti(),
                                                  REntity::PropertyLinetype);
    RDimensionEntity::PropertyLinetypeScale.generateId(
            RDimensionEntity::getRtti(), REntity::PropertyLinetypeScale);
    RDimensionEntity::PropertyLineweight.generateId(
            RDimensionEntity::getRtti(), REntity::PropertyLineweight);
    RDimensionEntity::PropertyColor.generateId(RDimensionEntity::getRtti(),
                                               REntity::PropertyColor);
    RDimensionEntity::PropertyDisplayedColor.generateId(
            RDimensionEntity::getRtti(), REntity::PropertyDisplayedColor);
    RDimensionEntity::PropertyDrawOrder.generateId(RDimensionEntity::getRtti(),
                                                   REntity::PropertyDrawOrder);

    RDimensionEntity::PropertyText.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Label"));
    RDimensionEntity::PropertyUpperTolerance.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Tolerance"),
            QT_TRANSLATE_NOOP("REntity", "Upper Limit"));
    RDimensionEntity::PropertyLowerTolerance.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Tolerance"),
            QT_TRANSLATE_NOOP("REntity", "Lower Limit"));
    RDimensionEntity::PropertyDefinitionPointX.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Definition Point"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RDimensionEntity::PropertyDefinitionPointY.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Definition Point"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RDimensionEntity::PropertyDefinitionPointZ.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Definition Point"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);
    RDimensionEntity::PropertyMiddleOfTextX.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Text Position"),
            QT_TRANSLATE_NOOP("REntity", "X"), false,
            RPropertyAttributes::Geometry);
    RDimensionEntity::PropertyMiddleOfTextY.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Text Position"),
            QT_TRANSLATE_NOOP("REntity", "Y"), false,
            RPropertyAttributes::Geometry);
    RDimensionEntity::PropertyMiddleOfTextZ.generateId(
            RDimensionEntity::getRtti(),
            QT_TRANSLATE_NOOP("REntity", "Text Position"),
            QT_TRANSLATE_NOOP("REntity", "Z"), false,
            RPropertyAttributes::Geometry);
    //    RDimensionEntity::PropertyFontName.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Font"));
    //    RDimensionEntity::PropertyTextColor.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Text Color"));
    RDimensionEntity::PropertyArrow1Flipped.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Flip First Arrow"));
    RDimensionEntity::PropertyArrow2Flipped.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Flip Second Arrow"));

    RDimensionEntity::PropertyExtLineFix.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Fixed Extension Line"));
    RDimensionEntity::PropertyExtLineFixLength.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Extension Line Length"));

    //    RDimensionEntity::PropertyHeight.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Height"));
    //    RDimensionEntity::PropertyLineSpacingFactor.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Line Spacing"));
    //    RDimensionEntity::PropertyHAlign.generateId(RDimensionEntity::getRtti(), QT_TRANSLATE_NOOP("REntity", "Alignment"), QT_TRANSLATE_NOOP("REntity", "Horizontal"));
    //    RDimensionEntity::PropertyVAlign.generateId(RDimensionEntity::getRtti(), QT_TRANSLATE_NOOP("REntity", "Alignment"), QT_TRANSLATE_NOOP("REntity", "Vertical"));

    RDimensionEntity::PropertyAutoLabel.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Auto Label"));
    RDimensionEntity::PropertyMeasuredValue.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Measured Value"));
    //    RDimensionEntity::PropertyLinearFactor.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Linear Factor"));
    //    RDimensionEntity::PropertyDimScale.generateId(RDimensionEntity::getRtti(), "", QT_TRANSLATE_NOOP("REntity", "Scale"));
    RDimensionEntity::PropertyDimBlockName.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Block Name"));
    RDimensionEntity::PropertyAutoTextPos.generateId(
            RDimensionEntity::getRtti(), "",
            QT_TRANSLATE_NOOP("REntity", "Auto Label Position"));
}

bool RDimensionEntity::setProperty(RPropertyTypeId propertyTypeId,
                                   const QVariant &value,
                                   RTransaction *transaction)
{
    bool ret = REntity::setProperty(propertyTypeId, value, transaction);

    ret = ret || RObject::setMember(getData().definitionPoint.x, value,
                                    PropertyDefinitionPointX == propertyTypeId);
    ret = ret || RObject::setMember(getData().definitionPoint.y, value,
                                    PropertyDefinitionPointY == propertyTypeId);
    ret = ret || RObject::setMember(getData().definitionPoint.z, value,
                                    PropertyDefinitionPointZ == propertyTypeId);
    if (RObject::setMember(getData().textPositionCenter.x, value,
                           PropertyMiddleOfTextX == propertyTypeId))
    {
        ret = true;
        getData().autoTextPos = false;
        getData().textPositionSide = RVector::invalid;
        //getData().updateFromTextPosition();
    }
    if (RObject::setMember(getData().textPositionCenter.y, value,
                           PropertyMiddleOfTextY == propertyTypeId))
    {
        ret = true;
        getData().autoTextPos = false;
        getData().textPositionSide = RVector::invalid;
        //getData().updateFromTextPosition();
    }
    if (RObject::setMember(getData().textPositionCenter.z, value,
                           PropertyMiddleOfTextZ == propertyTypeId))
    {
        ret = true;
        getData().autoTextPos = false;
        getData().textPositionSide = RVector::invalid;
        //getData().updateFromTextPosition();
    }
    ret = ret || RObject::setMember(getData().text, value,
                                    PropertyText == propertyTypeId);
    ret = ret || RObject::setMember(getData().upperTolerance, value,
                                    PropertyUpperTolerance == propertyTypeId);
    ret = ret || RObject::setMember(getData().lowerTolerance, value,
                                    PropertyLowerTolerance == propertyTypeId);

    if (PropertyDimlfac == propertyTypeId)
    {
        getData().setDimlfac(value.toDouble());
        ret = true;
    }

    if (PropertyDimscale == propertyTypeId)
    {
        getData().setDimscale(value.toDouble());
        ret = true;
    }

    ret = ret || RObject::setMember(getData().dimBlockName, value,
                                    PropertyDimBlockName == propertyTypeId);
    ret = ret || RObject::setMember(getData().autoTextPos, value,
                                    PropertyAutoTextPos == propertyTypeId);

    //    ret = ret || RObject::setMember(getData().textHeight, value, PropertyHeight == propertyTypeId);
    ret = ret || RObject::setMember(getData().textRotation, value,
                                    PropertyTextRotation == propertyTypeId);
    //    ret = ret || RObject::setMember(getData().lineSpacingFactor, value, PropertyLineSpacingFactor == propertyTypeId);
    //    ret = ret || RObject::setMember((int&)getData().horizontalAlignment, value.value<int>(), PropertyHAlign == propertyTypeId);
    //    ret = ret || RObject::setMember((int&)getData().verticalAlignment, value.value<int>(), PropertyVAlign == propertyTypeId);

    if (propertyTypeId == PropertyArchTick)
    {
        if (value.toBool() == true)
        {
            getData().setDimtsz(getData().getDimasz());
        }
        else { getData().setDimtsz(0.0); }
        ret = true;
    }

    for (int i = 0; i < RDimStyle::propertyVariables.length(); i++)
    {
        if (RDimStyle::propertyVariables[i].first == propertyTypeId)
        {

            if (propertyTypeId == RDimensionEntity::PropertyDimdsep)
            {
                if (value.type() == QVariant::String)
                {
                    QString str = value.toString();
                    if (str.length() >= 1)
                    {
                        getData().setDimXVariant(
                                RDimStyle::propertyVariables[i].second,
                                str.at(0).unicode());
                        ret = true;
                    }
                    break;
                }
            }

            getData().setDimXVariant(RDimStyle::propertyVariables[i].second,
                                     value);
            ret = true;
            break;
        }
    }

    if (ret) { getData().update(); }

    return ret;
}

QPair<QVariant, RPropertyAttributes>
RDimensionEntity::getProperty(RPropertyTypeId &propertyTypeId,
                              bool humanReadable, bool noAttributes,
                              bool showOnRequest)
{
    if (propertyTypeId == PropertyDefinitionPointX)
    {
        return qMakePair(QVariant(getData().definitionPoint.x),
                         RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyDefinitionPointY)
    {
        return qMakePair(QVariant(getData().definitionPoint.y),
                         RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyDefinitionPointZ)
    {
        return qMakePair(QVariant(getData().definitionPoint.z),
                         RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyMiddleOfTextX)
    {
        if (getData().textPositionSide.isValid())
        {
            return qMakePair(QVariant(getData().textPositionSide.x),
                             RPropertyAttributes());
        }
        else
        {
            return qMakePair(QVariant(getData().textPositionCenter.x),
                             RPropertyAttributes());
        }
    }
    else if (propertyTypeId == PropertyMiddleOfTextY)
    {
        if (getData().textPositionSide.isValid())
        {
            return qMakePair(QVariant(getData().textPositionSide.y),
                             RPropertyAttributes());
        }
        else
        {
            return qMakePair(QVariant(getData().textPositionCenter.y),
                             RPropertyAttributes());
        }
    }
    else if (propertyTypeId == PropertyMiddleOfTextZ)
    {
        if (getData().textPositionSide.isValid())
        {
            return qMakePair(QVariant(getData().textPositionSide.z),
                             RPropertyAttributes());
        }
        else
        {
            return qMakePair(QVariant(getData().textPositionCenter.z),
                             RPropertyAttributes());
        }
    }
    else if (propertyTypeId == PropertyText)
    {
        return qMakePair(
                QVariant(getData().text),
                RPropertyAttributes(RPropertyAttributes::DimensionLabel));
    }
    else if (propertyTypeId == PropertyUpperTolerance)
    {
        return qMakePair(QVariant(getData().upperTolerance),
                         RPropertyAttributes(RPropertyAttributes::Label));
    }
    else if (propertyTypeId == PropertyLowerTolerance)
    {
        return qMakePair(QVariant(getData().lowerTolerance),
                         RPropertyAttributes(RPropertyAttributes::Label));
    }
    else if (propertyTypeId == PropertyAutoLabel)
    {
        if (getType() == RS::EntityDimAngular2L ||
            getType() == RS::EntityDimAngular3P)
        {
            return qMakePair(QVariant(getData().getAutoLabel()),
                             RPropertyAttributes(RPropertyAttributes::ReadOnly |
                                                 RPropertyAttributes::Angle));
        }
        else
        {
            return qMakePair(
                    QVariant(getData().getAutoLabel()),
                    RPropertyAttributes(RPropertyAttributes::ReadOnly));
        }
    }
    else if (propertyTypeId == PropertyMeasuredValue)
    {
        if (getType() == RS::EntityDimAngular2L ||
            getType() == RS::EntityDimAngular3P)
        {
            return qMakePair(QVariant(getData().getMeasuredValue()),
                             RPropertyAttributes(RPropertyAttributes::ReadOnly |
                                                 RPropertyAttributes::Angle));
        }
        else
        {
            return qMakePair(
                    QVariant(getData().getMeasuredValue()),
                    RPropertyAttributes(RPropertyAttributes::ReadOnly));
        }
        //    } else if (propertyTypeId == PropertyDimlfac) {
        //        return qMakePair(QVariant(getData().getDimlfac()), RPropertyAttributes(RPropertyAttributes::UnitLess));
        //    } else if (propertyTypeId == PropertyDimscale) {
        //        return qMakePair(QVariant(getData().getDimscale()), RPropertyAttributes(RPropertyAttributes::UnitLess));
    }
    else if (propertyTypeId == PropertyDimBlockName)
    {
        return qMakePair(QVariant(getData().dimBlockName),
                         RPropertyAttributes(RPropertyAttributes::ReadOnly));
    }
    else if (propertyTypeId == PropertyAutoTextPos)
    {
        return qMakePair(QVariant(getData().autoTextPos),
                         RPropertyAttributes(RPropertyAttributes::Invisible));
    }

    //    else if (propertyTypeId == PropertyFontName) {
    //        return qMakePair(QVariant(getData().fontName),
    //            RPropertyAttributes(RPropertyAttributes::Style));
    //    }

    //    else if (propertyTypeId == PropertyTextColor) {
    //        return qMakePair(QVariant(getData().textColor), RPropertyAttributes());
    //    }

    else if (propertyTypeId == PropertyArrow1Flipped)
    {
        return qMakePair(QVariant(getData().arrow1Flipped),
                         RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyArrow2Flipped)
    {
        return qMakePair(QVariant(getData().arrow2Flipped),
                         RPropertyAttributes());
    }

    else if (propertyTypeId == PropertyExtLineFix)
    {
        return qMakePair(QVariant(getData().extLineFix), RPropertyAttributes());
    }
    else if (propertyTypeId == PropertyExtLineFixLength)
    {
        return qMakePair(QVariant(getData().extLineFixLength),
                         RPropertyAttributes());
    }

    /*else if (propertyTypeId == PropertyHeight) {
        return qMakePair(QVariant(getData().textHeight), RPropertyAttributes());
    */

    else if (propertyTypeId == PropertyTextRotation)
    {
        return qMakePair(QVariant(getData().textRotation),
                         RPropertyAttributes(RPropertyAttributes::Angle));
    }

    /*else if (propertyTypeId == PropertyLineSpacingFactor) {
        return qMakePair(QVariant(getData().lineSpacingFactor), RPropertyAttributes());
    } else if (propertyTypeId == PropertyHAlign) {
        return qMakePair(QVariant(getData().horizontalAlignment), RPropertyAttributes());
    } else if (propertyTypeId == PropertyVAlign) {
        return qMakePair(QVariant(getData().verticalAlignment), RPropertyAttributes());
    }*/

    if (propertyTypeId == PropertyArchTick)
    {
        return qMakePair(QVariant(getData().getDimtsz() > 0.0),
                         RPropertyAttributes());
    }

    for (int i = 0; i < RDimStyle::propertyVariables.length(); i++)
    {
        //RDimStyle::RDimXVar p = RDimStyle::propertyVariables[i];
        QPair<RPropertyTypeId, RS::KnownVariable> pv =
                RDimStyle::propertyVariables.value(i);
        if (pv.first == propertyTypeId)
        {
            RPropertyAttributes attr;
            if (propertyTypeId == RDimensionEntity::PropertyDimdsep)
            {
                // show DIMDESP as string
                QVariant v = getData().getDimXVariant(pv.second);
                QString str;
                str.append(QChar(v.toInt()));
                return qMakePair(QVariant(str), attr);
            }

            if (RDimStyleData::getVariableType(pv.second) == RS::VarTypeInt)
            {
                attr.setInteger(true);
            }

            // TODO: add advanced attribute to show / hide in PE
            return qMakePair(getData().getDimXVariant(pv.second), attr);
        }
    }

    return REntity::getProperty(propertyTypeId, humanReadable, noAttributes,
                                showOnRequest);
}

QSet<QString> RDimensionEntity::getDimensionBlockNames(RDocument *doc)
{
    if (doc == NULL) { return QSet<QString>(); }

    QSet<QString> ret;

    QList<RS::EntityType> dimTypes;
    dimTypes.append(RS::EntityDimLinear);
    dimTypes.append(RS::EntityDimAligned);
    dimTypes.append(RS::EntityDimRotated);
    dimTypes.append(RS::EntityDimRadial);
    dimTypes.append(RS::EntityDimDiametric);
    dimTypes.append(RS::EntityDimAngular2L);
    dimTypes.append(RS::EntityDimAngular3P);
    dimTypes.append(RS::EntityDimOrdinate);

    QSet<REntity::Id> ids = doc->queryAllEntities(false, true, dimTypes);
    QSet<REntity::Id>::iterator it;
    for (it = ids.begin(); it != ids.end(); it++)
    {
        int id = *it;
        QSharedPointer<REntity> e = doc->queryEntityDirect(id);
        QSharedPointer<RDimensionEntity> dimEntity =
                e.dynamicCast<RDimensionEntity>();
        if (dimEntity.isNull()) { continue; }

        ret.insert(dimEntity->getDimBlockName().toUpper());
    }

    return ret;
}
