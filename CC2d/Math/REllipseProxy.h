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

#ifndef RELLIPSEPROXY_H
#define RELLIPSEPROXY_H

#include <CC2dCoreExport.h>

#include <RS.h>
#include <RVector.h>

class REllipse;
class RPolyline;
class RShape;


/**
 * Proxy for advanced ellipse functionality. May be implemented
 * by a plugin.
 *
 * \ingroup math
 */
class CC2DCORE_EXPORT REllipseProxy
{
public:
    virtual ~REllipseProxy() {}

    virtual RPolyline approximateWithArcs(const REllipse &ellipse,
                                          int segments) const = 0;

    virtual REllipse
    createInscribed(const RVector &pp1, const RVector &pp2, const RVector &pp3,
                    const RVector &pp4,
                    const RVector &centerHint = RVector::invalid) const = 0;
    virtual REllipse createFrom4Points(const RVector &pp1, const RVector &pp2,
                                       const RVector &pp3,
                                       const RVector &pp4) const = 0;
};

#endif
