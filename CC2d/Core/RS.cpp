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

#include "RGlobal.h"

const double RS::PointTolerance = 1.0e-9;
const double RS::AngleTolerance = 1.0e-9;


bool RS::exactMatch(const QRegularExpression &rx, const QString &string)
{
#if QT_VERSION >= 0x050000
    QString pat = rx.pattern();
    pat = "^" + pat + "$";
    QRegularExpression rxExact(pat);
    return rxExact.match(string).hasMatch();
#else
    return rx.exactMatch(string);
#endif
}

bool RS::exactMatch(const QRegularExpression &rx,
                    QRegularExpressionMatch &match, const QString &string)
{
#if QT_VERSION >= 0x050000
    QString pat = rx.pattern();
    pat = "^" + pat + "$";
    QRegularExpression rxExact(pat);
    match = rxExact.match(string);
    return match.hasMatch();
#else
    return rx.exactMatch(string);
#endif
}

bool RS::exactMatch(const QString &rxStr, const QString &string)
{
    QRegularExpression rx("^" + rxStr + "$");
#if QT_VERSION >= 0x050000
    return rx.match(string).hasMatch();
#else
    return rx.exactMatch(string);
#endif
}


/**
 * Alphanumerical sorting.
 */
QStringList RS::sortAlphanumerical(const QStringList &list)
{
    QStringList ret = list;

    std::sort(ret.begin(), ret.end(), RS::lessThanAlphanumerical);

    return ret;
}

/**
 * \return List of strings containing either only numbers or non-numbers.
 * E.g. "abc123" -> "abc", "123"
 */
QStringList RS::compareChunkify(const QString &s)
{
    static QRegularExpression rx("0x[0-9a-fA-F]*");

    QStringList tz;
    bool nummerical = false;
    QString sLocal;

    if (s.startsWith("0x"))
    {
        QRegularExpressionMatch match;
        if (RS::indexIn(rx, match, s) == 0)
        {
            int len = RS::matchedLength(rx, match);
            sLocal = s.mid(len);
            nummerical = true;
            tz.append(s.left(len));
        }
    }
    else { sLocal = s; }

    for (int i = 0; i < sLocal.length(); i++)
    {
        QChar c = sLocal.at(i);
        bool n = c.isDigit() || c == '.';
        if (n != nummerical || i == 0)
        {
            tz.append(c);
            nummerical = n;
        }
        else { tz.last().append(c); }
    }

    return tz;
}

/**
 * Alphanumerical, locale aware, case insensitive comparison.
 */
int RS::compareAlphanumerical(const QString &s1, const QString &s2)
{
    // note: we deliberately do not use QCollator here (buggy, crashes under Linux):
    QStringList aa = compareChunkify(s1.toLower());
    QStringList bb = compareChunkify(s2.toLower());

    for (int x = 0; x < aa.length() && x < bb.length(); x++)
    {
        if (aa[x] != bb[x])
        {
            bool ok1, ok2;
            float c, d;
            if (x == 0 && aa[0].startsWith("0x") && bb[0].startsWith("0x"))
            {
                c = aa[0].mid(2).toInt(&ok1, 16);
                d = bb[0].mid(2).toInt(&ok2, 16);
            }
            else
            {
                c = aa[x].toFloat(&ok1);
                d = bb[x].toFloat(&ok2);
            }
            if (ok1 && ok2)
            {
                if (c - d < 0.0) { return -1; }
                if (c - d > 0.0) { return 1; }
                return 0;
            }
            else { return aa[x].localeAwareCompare(bb[x]); }
        }
    }

    return aa.length() - bb.length();
}

bool RS::lessThanAlphanumerical(const QString &s1, const QString &s2)
{
    return RS::compareAlphanumerical(s1, s2) < 0;
}


int RS::indexIn(const QRegularExpression &rx, QRegularExpressionMatch &match,
                const QString &string, int from)
{
#if QT_VERSION >= 0x050000
    return (int) string.indexOf(rx, from, &match);
#else
    return rx.indexIn(string, from);
#endif
}

int RS::matchedLength(const QRegularExpression &rx,
                      const QRegularExpressionMatch &match)
{
#if QT_VERSION >= 0x050000
    return (int) match.capturedLength();
#else
    return rx.matchedLength();
#endif
}