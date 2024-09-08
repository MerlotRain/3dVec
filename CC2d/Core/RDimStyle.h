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

#ifndef RDIMSTYLE_H
#define RDIMSTYLE_H

#include "CC2dCoreExport.h"

#include "RDimStyleData.h"
#include "RObject.h"
#include "RPropertyTypeId.h"

class RDocument;
class REntityData;

/**
 * Dimension style class.
 * This is responsible for rendering dimensions depending on style settings.
 *
 * \ingroup entity
 * \scriptable
 * \sharedPointerSupport
 */
class QCADCORE_EXPORT RDimStyle : public RObject, public RDimStyleData
{

public:
    static RPropertyTypeId PropertyCustom;
    static RPropertyTypeId PropertyHandle;
    static RPropertyTypeId PropertyProtected;

    static RPropertyTypeId PropertyDimscale;
    static RPropertyTypeId PropertyDimlfac;
    static RPropertyTypeId PropertyDimtxt;
    static RPropertyTypeId PropertyDimgap;
    static RPropertyTypeId PropertyDimasz;
    static RPropertyTypeId PropertyDimdli;
    static RPropertyTypeId PropertyDimexe;
    static RPropertyTypeId PropertyDimexo;
    static RPropertyTypeId PropertyDimtad;
    static RPropertyTypeId PropertyDimtih;
    static RPropertyTypeId PropertyDimtsz;
    static RPropertyTypeId PropertyDimlunit;
    static RPropertyTypeId PropertyDimdec;
    static RPropertyTypeId PropertyDimdsep;
    static RPropertyTypeId PropertyDimzin;
    static RPropertyTypeId PropertyDimaunit;
    static RPropertyTypeId PropertyDimadec;
    static RPropertyTypeId PropertyDimazin;
    static RPropertyTypeId PropertyArchTick;
    static RPropertyTypeId PropertyDimclrt;
    static RPropertyTypeId PropertyDimblk;
    static RPropertyTypeId PropertyDimblk1;
    static RPropertyTypeId PropertyDimblk2;

public:
    RDimStyle();

    RDimStyle(RDocument *document);
    virtual ~RDimStyle();

    static void init();
    static void initDimX(const RPropertyTypeId &propertyTypeId,
                         RS::KnownVariable var, RS::KnownVariableType type);

    static RS::EntityType getRtti() { return RS::ObjectDimStyle; }

    virtual RS::EntityType getType() const { return RS::ObjectDimStyle; }

    virtual RDimStyle *clone() const { return new RDimStyle(*this); }

    void updateDocumentVariables();
    void updateFromDocumentVariables();

    QPair<QVariant, RPropertyAttributes>
    getProperty(RPropertyTypeId &propertyTypeId, bool humanReadable = false,
                bool noAttributes = false, bool showOnRequest = false);
    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant &value,
                     RTransaction *transaction);

    void render(const REntityData &entityData, bool preview, bool forceSelected)
    {
    }


public:
    static QList<QPair<RPropertyTypeId, RS::KnownVariable>> propertyVariables;
};

#endif
