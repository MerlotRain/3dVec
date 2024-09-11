#ifndef RTEXTLABEL_H
#define RTEXTLABEL_H

#include <CC2dCoreExport.h>

#include "RBox.h"
#include "RPoint.h"
#include "RVector.h"

#ifndef RDEFAULT_QVARIANT
#define RDEFAULT_QVARIANT QVariant()
#endif

class CC2DCORE_EXPORT RTextLabel : public RPoint
{
public:
    RTextLabel();
    RTextLabel(const RVector &position, const QString &text,
               const QVariant &userData = RDEFAULT_QVARIANT);
    virtual ~RTextLabel();

    virtual RTextLabel *clone() const { return new RTextLabel(*this); }

    QString getText() const { return text; }
    QVariant getUserData() const { return userData; }


public:
    QString text;
    QVariant userData;
};
Q_DECLARE_METATYPE(RTextLabel)
Q_DECLARE_METATYPE(RTextLabel*)
Q_DECLARE_METATYPE(QSharedPointer<RTextLabel>)
Q_DECLARE_METATYPE(QSharedPointer<RTextLabel>*)
Q_DECLARE_METATYPE(QList<RTextLabel>)

#endif
