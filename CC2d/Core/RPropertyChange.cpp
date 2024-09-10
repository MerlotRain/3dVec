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
#include "RPropertyChange.h"
#include <QVariant>

RPropertyChange::RPropertyChange(RPropertyTypeId propertyTypeId,
                                 QVariant oldValue, QVariant newValue)
    : propertyTypeId(propertyTypeId)
{
#if QT_MAJOR_VERSION > 6
    if (oldValue.typeId() != newValue.typeId() && oldValue.isValid() &&
        newValue.isValid())
    {
        qWarning() << "old and new property values for property "
                   << propertyTypeId.getPropertyTitle()
                   << " have different types:" << oldValue.typeName()
                   << " vs. " << newValue.typeName();
    }
    this->oldValue = oldValue;
    this->newValue = newValue;
#else
    if (oldValue.type() != newValue.type() && oldValue.isValid() &&
        newValue.isValid())
    {
        qWarning() << "old and new property values for property "
                   << propertyTypeId.getPropertyTitle()
                   << " have different types:" << oldValue.typeName()
                   << " vs. " << newValue.typeName();
    }
    this->oldValue = oldValue;
    this->newValue = newValue;
#endif


}
