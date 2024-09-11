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
