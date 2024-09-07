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
#include <QDir>
#include <QFileInfo>

const double RS::PointTolerance = 1.0e-9;
const double RS::AngleTolerance = 1.0e-9;


bool RS::exactMatch(const QRegularExpression &rx, const QString &string)
{
    QString pat = rx.pattern();
    pat = "^" + pat + "$";
    QRegularExpression rxExact(pat);
    return rxExact.match(string).hasMatch();
}

bool RS::exactMatch(const QRegularExpression &rx,
                    QRegularExpressionMatch &match, const QString &string)
{
    QString pat = rx.pattern();
    pat = "^" + pat + "$";
    QRegularExpression rxExact(pat);
    match = rxExact.match(string);
    return match.hasMatch();
}

bool RS::exactMatch(const QString &rxStr, const QString &string)
{
    QRegularExpression rx("^" + rxStr + "$");
    return rx.match(string).hasMatch();
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
    return (int) string.indexOf(rx, from, &match);
}

int RS::matchedLength(const QRegularExpression &rx,
                      const QRegularExpressionMatch &match)
{
    return (int) match.capturedLength();
}

/**
 * \return List of all directories in subdirectory 'subDirectory' in
 * all possible QCAD directories.
 */
QStringList RS::getDirectoryList(const QString &subDirectory)
{
    QStringList dirList;

    QString appDir = QCoreApplication::applicationDirPath();
    QFileInfo fi(appDir);
    if (fi.fileName() == "debug" || fi.fileName() == "release")
    {
        appDir = fi.absolutePath();
    }

#ifdef Q_OS_MAC
    // macOS app bundle:
    dirList.append(appDir + "/../Resources/" + subDirectory);
    dirList.append(appDir + "/../../../" + subDirectory);
    dirList.append(QDir::currentPath() + "/" + subDirectory);
#else
    dirList.append(appDir + "/" + subDirectory);
#endif

    /*
#ifdef Q_OS_MAC
    if (subDirectory!="library") {
#endif
        //local (application) directory has priority over other dirs:
        dirList.append(appDir + QDir::separator() + subDirectory);

 #ifdef Q_OS_LINUX
        QString appDirName = QSettings.applicationName();
        // Redhat style:
        dirList.append("/usr/share/" + appDirName + "/" + subDirectory);

        // SuSE style:
        dirList.append("/usr/X11R6/" + appDirName + "/" + subDirectory);

        dirList.append("/usr/X11R6/share/" + appDirName + "/" + subDirectory);
        dirList.append(QDir::homePath() + "/." + appDirName + "/" + subDirectory);
#endif

#ifdef Q_OS_MAC
    }
#endif
    */

    // add a path to users home (config dir) to be used to extend pattern, linetypes, etc.
    dirList.append("/" + subDirectory);
    dirList.append("/" + subDirectory);

    QStringList ret;
    for (int i = 0; i < dirList.size(); i++)
    {
        QFileInfo fi(dirList.at(i));
        QString dir = fi.canonicalFilePath();
        if (fi.isDir() && !ret.contains(dir)) { ret.append(dir); }
    }

    return ret;
}

QStringList RS::getFileList(const QString &subDirectory,
                            const QString &fileExtension)
{

    QStringList dirList = getDirectoryList(subDirectory);

    QStringList fileList;
    QString path;
    QDir dir;

    for (int i = 0; i < dirList.size(); ++i)
    {
        path = dirList.at(i);
        dir = QDir(path);

        if (dir.exists() && dir.isReadable())
        {
            // this seems to be case insensitive by default:
            QStringList files = dir.entryList(QStringList("*." + fileExtension),
                                              QDir::Files | QDir::Readable);
            for (int k = 0; k < files.size(); ++k)
            {
                QString f = path + QDir::separator() + files.at(k);
                fileList.append(f);
            }
        }
    }

    return fileList;
}

/**
 * \return A list of absolute paths to all pattern files found.
 */
QStringList RS::getPatternList(bool metric)
{
    if (metric) { return getFileList("patterns/metric", "pat"); }
    else { return getFileList("patterns/imperial", "pat"); }
}

/**
 * \return A list of absolute paths to all pattern files found.
 */
QStringList RS::getLinetypeList(bool metric)
{
    if (metric) { return getFileList("linetypes/metric", "lin"); }
    else { return getFileList("linetypes/imperial", "lin"); }
}