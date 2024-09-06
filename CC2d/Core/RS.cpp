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

#include <QtGlobal>

#include <QTextCodec>
#include <QDesktopWidget>
#include <QPageSize>

#include "RColor.h"
#include "RGlobal.h"
//#include "RLinetype.h"
//#include "RLineweight.h"
//#include "RPropertyAttributes.h"
#include "RVector.h"
#include <qmath.h>

const double RS::PointTolerance = 1.0e-9;
const double RS::AngleTolerance = 1.0e-9;
