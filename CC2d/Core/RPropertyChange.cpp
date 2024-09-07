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

    if (oldValue.typeId() == QVariant::Type::UserType &&
        (oldValue.canConvert<QList<double>>() || !oldValue.isValid()) &&
        (newValue.canConvert<QList<double>>() || !newValue.isValid()))
    {

        QList<double> oldList;
        if (oldValue.isValid()) { oldList = oldValue.value<QList<double>>(); }
        QList<double> newList;
        if (newValue.isValid()) { newList = newValue.value<QList<double>>(); }

        QList<QPair<int, double>> oldCompact;
        QList<QPair<int, double>> newCompact;

        // entries were changed only:
        if (oldList.size() == newList.size())
        {
            for (int i = 0; i < oldList.size(); i++)
            {
                if (oldList.at(i) != newList.at(i))
                {
                    oldCompact.append(QPair<int, double>(i, oldList.at(i)));
                    newCompact.append(QPair<int, double>(i, newList.at(i)));
                }
            }
        }

        // entries were appended and possibly changed:
        else if (oldList.size() < newList.size())
        {
            for (int i = 0; i < newList.size(); i++)
            {
                if (i >= oldList.size())
                {
                    oldCompact.append(QPair<int, double>(i, RNANDOUBLE));
                    newCompact.append(QPair<int, double>(i, newList.at(i)));
                }
                else if (oldList.at(i) != newList.at(i))
                {
                    oldCompact.append(QPair<int, double>(i, oldList.at(i)));
                    newCompact.append(QPair<int, double>(i, newList.at(i)));
                }
            }
        }

        // entries were removed and possibly changed:
        else if (oldList.size() > newList.size())
        {
            for (int i = 0; i < oldList.size(); i++)
            {
                if (i >= newList.size())
                {
                    oldCompact.append(QPair<int, double>(i, oldList.at(i)));
                    newCompact.append(QPair<int, double>(i, RNANDOUBLE));
                }
                else if (oldList.at(i) != newList.at(i))
                {
                    oldCompact.append(QPair<int, double>(i, oldList.at(i)));
                    newCompact.append(QPair<int, double>(i, newList.at(i)));
                }
            }
        }

        this->oldValue.setValue(oldCompact);
        this->newValue.setValue(newCompact);

        return;
    }

    else
    {
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
        return;
    }
}
