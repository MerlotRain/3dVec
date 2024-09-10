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

#ifndef RPATTERNLINE_H
#define RPATTERNLINE_H

#include "CC2dCoreExport.h"

#include "RLine.h"
#include "RVector.h"


/**
 * Represents a hatch pattern line.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 * \hasStreamOperator
 */
class CC2DCORE_EXPORT RPatternLine
{
public:
    double angle;
    RVector offset;
    RVector basePoint;
    QList<double> dashes;

    bool hasDots() const;
    QList<RLine> getLines(bool includeDashes = false) const;
    double getLength() const;
    double getAngle() const { return angle; }
    RVector getOffset() const { return offset; }
    RVector getBasePoint() const { return basePoint; }
    QList<double> getDashes() const { return dashes; }
    void scale(double f);
    void rotate(double a);
};

#endif
