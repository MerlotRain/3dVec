#include <qmath.h>

#include "RBox.h"
#include "RLine.h"
#include "RTextLabel.h"

/**
 * Creates a point object with an invalid position
 */
RTextLabel::RTextLabel() {}

/**
 * Creates a point object with the given position.
 *
 * \param position the position
 *
 */
RTextLabel::RTextLabel(const RVector &position, const QString &text,
                       const QVariant &userData)
    : RPoint(position), text(text), userData(userData)
{
}

RTextLabel::~RTextLabel() {}
