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
#include <QFont>
#include <QTextBlock>
#include <QTextDocument>

#include "RColor.h"
#include "RPolyline.h"
#include "RTextBasedData.h"
#include "RUnit.h"


RTextBasedData::RTextBasedData(RDocument *document)
    : REntityData(document), textHeight(0.0), textWidth(0.0),
      verticalAlignment(RS::VAlignTop), horizontalAlignment(RS::HAlignLeft),
      drawingDirection(RS::LeftToRight), lineSpacingStyle(RS::Exact),
      lineSpacingFactor(1.0), fontName("standard"), angle(0.0), xScale(1.0),
      height(RNANDOUBLE), width(RNANDOUBLE), dirty(true), gotDraft(false)
{
}

RTextBasedData::RTextBasedData(RDocument *document, const RTextBasedData &data)
    : REntityData(document)
{
    *this = data;
    this->document = document;
    if (document != NULL) { linetypeId = document->getLinetypeByLayerId(); }
}

/**
 * \param position Insertion point (see DXF code 10 doc).
 * \param alignmentPoint Alignment point of text (see DXF code 11 doc).
 * \param height Nominal (initial) text height
 * \param textWidth Reference rectangle width for wrapping (not supported)
 * \param valign Vertical alignment
 * \param halign Horizontal alignment
 * \param drawingDirection Drawing direction
 * \param lineSpacingStyle Line spacing style
 * \param lineSpacingFactor Line spacing factor
 * \param text Text string
 * \param fontName Text font name
 * \param angle Rotation angle in rad
 */
RTextBasedData::RTextBasedData(const RVector &position,
                               const RVector &alignmentPoint, double height,
                               double textWidth, RS::VAlign valign,
                               RS::HAlign halign,
                               RS::TextDrawingDirection drawingDirection,
                               RS::TextLineSpacingStyle lineSpacingStyle,
                               double lineSpacingFactor, const QString &text,
                               const QString &fontName, bool bold, bool italic,
                               double angle, bool simple)
    : position(position), alignmentPoint(alignmentPoint), textHeight(height),
      textWidth(textWidth), verticalAlignment(valign),
      horizontalAlignment(halign), drawingDirection(drawingDirection),
      lineSpacingStyle(lineSpacingStyle), lineSpacingFactor(lineSpacingFactor),
      fontName(fontName), angle(angle), xScale(1.0), height(RNANDOUBLE),
      width(RNANDOUBLE), dirty(true), gotDraft(false)
{

    setText(text);
    setBold(bold);
    setItalic(italic);
    setSimple(simple);
}

bool RTextBasedData::isSane() const
{
    return position.isSane() && alignmentPoint.isSane();
}

RBox RTextBasedData::getBoundingBox(bool ignoreEmpty) const { return RBox(); }

/**
 * Width of the text (this is not equal to the bounding box width as it is
 * the width of the text at 0 angle).
 */
double RTextBasedData::getWidth() const { return width; }

/**
 * Height of the text (this is not equal to the bounding box height as it is
 * the height of the text at 0 angle).
 */
double RTextBasedData::getHeight() const { return height; }

void RTextBasedData::sync(const RTextBasedData &other) { *this = other; }

RVector RTextBasedData::getPointOnEntity() const { return RVector(); }

double RTextBasedData::getDistanceTo(const RVector &point, bool limited,
                                     double range, bool draft,
                                     double strictRange) const
{
    Q_UNUSED(limited)
    Q_UNUSED(draft)
    Q_UNUSED(range)


    return 0.0;
}

bool RTextBasedData::intersectsWith(const RShape &shape) const { return false; }

void RTextBasedData::setText(const QString &text)
{
    if (text.contains("\n"))
    {
        QString t = text;
        t.replace(QString("\n"), QString("\\P"));
        this->text = t;
    }
    else { this->text = text; }
    update();
}

QList<RRefPoint>
RTextBasedData::getReferencePoints(RS::ProjectionRenderingHint hint) const
{
    Q_UNUSED(hint)

    QList<RRefPoint> ret;

    ret.append(alignmentPoint);

    if (!isSimple())
    {
        //        if (height<1.0e-6 || dirty) {
        //            getPainterPaths();
        //        }

        double w = textWidth;
        switch (horizontalAlignment)
        {
            case RS::HAlignCenter:
            case RS::HAlignMid:
                w = w / 2;
                break;
            case RS::HAlignRight:
                w = -w;
                break;
            default:
                break;
        }
        RVector vWidth = RVector::createPolar(w, angle);
        ret.append(alignmentPoint + vWidth);
        //ret.append(position + vWidth + vHeight);
        //ret.append(position + vHeight);
    }

    return ret;
}

bool RTextBasedData::moveReferencePoint(const RVector &referencePoint,
                                        const RVector &targetPoint,
                                        Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)

    bool ret = false;
    if (referencePoint.equalsFuzzy(position))
    {
        position = targetPoint;
        ret = true;
        update(false);
    }
    if (referencePoint.equalsFuzzy(alignmentPoint))
    {
        alignmentPoint = targetPoint;
        ret = true;
        update(false);
    }

    double w = textWidth;
    switch (horizontalAlignment)
    {
        case RS::HAlignCenter:
        case RS::HAlignMid:
            w = w / 2;
            break;
        case RS::HAlignRight:
            w = -w;
            break;
    }

    RVector vWidth = RVector::createPolar(w, angle);
    RVector widthHandle = alignmentPoint + vWidth;

    if (referencePoint.equalsFuzzy(widthHandle))
    {
        // TODO: handle centered, right-aligned text:

        RLine alignmentAxis(
                alignmentPoint,
                alignmentPoint + RVector::createPolar(1.0, angle + M_PI / 2));
        double d = alignmentAxis.getDistanceTo(targetPoint, false);

        switch (horizontalAlignment)
        {
            case RS::HAlignCenter:
            case RS::HAlignMid:
                textWidth = d * 2;
                break;
            case RS::HAlignRight:
            default:
                textWidth = d;
                break;
        }

        qDebug() << "changed width to: " << textWidth;
        ret = true;
        update(false);
    }

    return ret;
}

bool RTextBasedData::move(const RVector &offset)
{
    position.move(offset);
    alignmentPoint.move(offset);
    update(false);
    return true;
}

bool RTextBasedData::rotate(double rotation, const RVector &center)
{
    position.rotate(rotation, center);
    alignmentPoint.rotate(rotation, center);
    angle = RMath::getNormalizedAngle(angle + rotation);
    update(false);
    return true;
}

bool RTextBasedData::scale(const RVector &scaleFactors, const RVector &center)
{
    position.scale(scaleFactors, center);
    alignmentPoint.scale(scaleFactors, center);
    textWidth *= scaleFactors.x;
    textHeight *= scaleFactors.x;
    return true;
}

bool RTextBasedData::mirror(const RLine &axis)
{
    position.mirror(axis);
    alignmentPoint.mirror(axis);
    bool readable = RMath::isAngleReadable(angle);

    RVector vec = RVector::createPolar(1.0, angle);
    vec.mirror(RVector(0.0, 0.0), axis.getEndPoint() - axis.getStartPoint());
    angle = vec.getAngle();

    if (isSimple() && document->getKnownVariable(RS::MIRRTEXT, 0) != 0)
    {
        setUpsideDown(!isUpsideDown());
    }
    else
    {
        bool corr;
        angle = RMath::makeAngleReadable(angle, readable, &corr);

        if (corr)
        {
            if (horizontalAlignment == RS::HAlignLeft)
            {
                horizontalAlignment = RS::HAlignRight;
            }
            else if (horizontalAlignment == RS::HAlignRight)
            {
                horizontalAlignment = RS::HAlignLeft;
            }
        }
        else
        {
            if (verticalAlignment == RS::VAlignTop)
            {
                verticalAlignment = RS::VAlignBase;
            }
            else if (verticalAlignment == RS::VAlignBase)
            {
                verticalAlignment = RS::VAlignTop;
            }
        }
    }

    update(false);
    return true;
}

bool RTextBasedData::flipHorizontal()
{
    return mirror(RLine(RVector(0, 0), RVector(0, 1)));
}

bool RTextBasedData::flipVertical()
{
    return mirror(RLine(RVector(0, 0), RVector(1, 0)));
}

QFont RTextBasedData::getMainFont() const
{
    QFont mainFont(fontName);
    mainFont.setPointSizeF(textHeight);
    return mainFont;
}

QString RTextBasedData::getPlainText() const
{
    QTextDocument td;
    td.setHtml(toRichText(text, getMainFont()));
    return td.toPlainText().replace("&nbsp;", " ");
}

QString RTextBasedData::getRenderedText(bool escUnicode) const
{
    if (escUnicode) { return escapeUnicode(text); }
    else { return text; }
}

QString RTextBasedData::getEscapedText(bool escUnicode) const
{
    if (escUnicode) { return escapeUnicode(text); }
    else { return text; }
}

QString RTextBasedData::escapeUnicode(const QString &str) { return QString(); }

RVector RTextBasedData::getClosestPointOnEntity(const RVector &point,
                                                double range,
                                                bool limited) const
{

    Q_UNUSED(point)
    Q_UNUSED(range)
    Q_UNUSED(limited)

    return RVector::invalid;
}

QList<QSharedPointer<RShape>>
RTextBasedData::getShapes(const RBox &queryBox, bool ignoreComplex,
                          bool segment, QList<RObject::Id> *entityIds) const
{
    Q_UNUSED(queryBox)
    Q_UNUSED(segment)
    Q_UNUSED(entityIds)

    QList<QSharedPointer<RShape>> ret;
    return ret;
}

QList<QSharedPointer<RShape>> RTextBasedData::getExploded() const
{
    return getShapes();
}

void RTextBasedData::setFontName(const QString &fontName)
{
    this->fontName = fontName;
    // reset fontFile if font name changed:
    this->fontFile = "";
    update();
}

void RTextBasedData::update(bool layout) const {}

bool RTextBasedData::isDirty() const { return dirty; }

QList<RTextLayout> RTextBasedData::getTextLayouts() const
{
    return textLayouts;
}

/**
 * \return List of text data objects representing simple texts.
 * Complex formatted texts are split up into smaller text blocks,
 * each with unique formatting, color, etc.
 */
QList<RTextBasedData> RTextBasedData::getSimpleTextBlocks()
{
    return QList<RTextBasedData>();
}

/**
 * Converts the given text document (typically from a QTextEdit) into
 * CAD escaped text.
 *
 * \param fontHeightFactor factor applied to all font heights. This allows
 *      the text edit to use a bigger / smaller font that needed in the end.
 */
QString RTextBasedData::toEscapedText(const QTextDocument &textDocument,
                                      const RColor &initialColor,
                                      double fontHeightFactor, bool simpleText)
{
    QString ret = "";
    return ret;
}

/**
 * \return HTML representing this text.
 */
QString RTextBasedData::toRichText(const QString &escapedText,
                                   const QFont &mainFont,
                                   double fontHeightFactor)
{


    return QString();
}

void RTextBasedData::to2D()
{
    position.z = 0.0;
    alignmentPoint.z = 0.0;
}