#ifndef REXPLODABLE_H
#define REXPLODABLE_H

#include <CC2dCoreExport.h>

#include <QSharedPointer>

#include "RShape.h"

#ifndef RDEFAULT_MIN1
#define RDEFAULT_MIN1 -1
#endif

class CC2DCORE_EXPORT RExplodable
{
public:
    virtual ~RExplodable() {}
    virtual QList<QSharedPointer<RShape>>
    getExploded(int segments = RDEFAULT_MIN1) const = 0;
};


#endif
