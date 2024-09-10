/**
 * Copyright (c) 2011-2021 by Andrew Mustun. All rights reserved.
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

#include "RDimStyleData.h"

QMap<RS::KnownVariable, QVariant> RDimStyleData::mapDefaults;
//QList<RDimStyleData::RDimXVar> RDimStyleData::propertyVariables;
QMap<RS::KnownVariable, RS::KnownVariableType> RDimStyleData::dimXTypes;

RDimStyleData::RDimStyleData()
{
}

RDimStyleData::~RDimStyleData() {}

void RDimStyleData::init() {}

void RDimStyleData::initDefaults()
{
    mapDefaults[RS::DIMSCALE] = 1.0;
    mapDefaults[RS::DIMLFAC] = 1.0;
    mapDefaults[RS::DIMTXT] = 2.5;
    mapDefaults[RS::DIMGAP] = 0.625;
    mapDefaults[RS::DIMASZ] = 2.5;
    mapDefaults[RS::DIMEXE] = 1.25;
    mapDefaults[RS::DIMEXO] = 0.625;
    mapDefaults[RS::DIMTAD] = 1;
    mapDefaults[RS::DIMTIH] = false;
    mapDefaults[RS::DIMDLI] = 5.0;
    QVariant v;
    v.setValue<RColor>(RColor(RColor::ByLayer));
    mapDefaults[RS::DIMCLRT] = v;
    mapDefaults[RS::DIMTSZ] = 0.0;
    mapDefaults[RS::DIMLUNIT] = RS::Decimal;
    mapDefaults[RS::DIMDEC] = 4;
    mapDefaults[RS::DIMDSEP] = '.';
    mapDefaults[RS::DIMZIN] = 8;
    mapDefaults[RS::DIMAUNIT] = RS::DegreesDecimal;
    mapDefaults[RS::DIMADEC] = 0;
    mapDefaults[RS::DIMAZIN] = 2;

    mapDefaults[RS::DIMBLK] = "";
}

QVariant RDimStyleData::getVariantDefault(RS::KnownVariable key)
{
    if (mapDefaults.isEmpty()) { RDimStyleData::initDefaults(); }
    if (mapDefaults.contains(key)) { return mapDefaults[key]; }
    return QVariant();
}

double RDimStyleData::getDoubleDefault(RS::KnownVariable key)
{
    if (mapDefaults.isEmpty()) { RDimStyleData::initDefaults(); }
    if (mapDefaults.contains(key)) { return mapDefaults[key].toDouble(); }
    return RNANDOUBLE;
}

int RDimStyleData::getIntDefault(RS::KnownVariable key)
{
    if (mapDefaults.isEmpty()) { RDimStyleData::initDefaults(); }
    if (mapDefaults.contains(key)) { return mapDefaults[key].toInt(); }
    return RMININT;
}

bool RDimStyleData::getBoolDefault(RS::KnownVariable key)
{
    if (mapDefaults.isEmpty()) { RDimStyleData::initDefaults(); }
    if (mapDefaults.contains(key)) { return mapDefaults[key].toBool(); }
    return false;
}

RColor RDimStyleData::getColorDefault(RS::KnownVariable key)
{
    if (mapDefaults.isEmpty()) { RDimStyleData::initDefaults(); }
    if (mapDefaults.contains(key)) { return mapDefaults[key].value<RColor>(); }
    return RColor();
}