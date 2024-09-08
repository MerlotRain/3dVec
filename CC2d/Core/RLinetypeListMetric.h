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

#ifndef RLINETYPELISTMETRIC_H
#define RLINETYPELISTMETRIC_H

#include "CC2dCoreExport.h"

#include "RLinetypeList.h"

/**
 * The global list of metric linetypes.
 *
 * \ingroup core
 * \scriptable
 */
class QCADCORE_EXPORT RLinetypeListMetric : public RLinetypeList
{
public:
    static void init();
    static void uninit() {}

    static QStringList getNames() { return QStringList(); }

    static RLinetypePattern *get(const QString &name) { return nullptr; }

private:
    static QList<RLinetypePattern> res;
};

#endif
