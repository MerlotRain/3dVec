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
#include "RStorage.h"
#include "RDocument.h"

RStorage::RStorage()
    : modified(false), handleCounter(0), document(NULL), maxDrawOrder(0),
      idCounter(0), currentColor(RColor::ByLayer),
      currentLineweight(RLineweight::WeightByLayer),
      currentLinetypeId(RLinetype::INVALID_ID),
      //currentLayerId(RLayer::INVALID_ID),
      currentViewId(RView::INVALID_ID),
      currentViewportId(RViewportEntity::INVALID_ID),
      layer0Id(RLayer::INVALID_ID),
      lastTransactionId(-1), lastTransactionGroup(1),
      notifyGlobalListeners(true)
{
}

void RStorage::clear()
{
    lastModified = QDateTime();
    modified = false;
    handleCounter = 0;
    maxDrawOrder = 0;
    idCounter = 0;
    currentColor = RColor::ByLayer;
    currentLineweight = RLineweight::WeightByLayer,
    currentLinetypeId = RLinetype::INVALID_ID;
    currentViewId = RView::INVALID_ID;
    currentViewportId = RViewportEntity::INVALID_ID;
    lastTransactionId = -1;
    lastTransactionGroup = 1;
    notifyGlobalListeners = true;
}

void RStorage::setObjectId(RObject &object, RObject::Id objectId) const
{
    object.setId(objectId);
}

void RStorage::setObjectHandle(RObject &object, RObject::Handle objectHandle)
{
    object.setHandle(objectHandle);
    if (objectHandle > handleCounter) { handleCounter = objectHandle + 1; }
}

void RStorage::setUndoStatus(RObject &object, bool status)
{
    object.setUndone(status);
}

RObject::Id RStorage::getNewObjectId() { return idCounter++; }

RObject::Id RStorage::getMaxObjectId() { return idCounter; }

RObject::Handle RStorage::getNewObjectHandle() { return handleCounter++; }

RObject::Handle RStorage::getMaxObjectHandle() { return handleCounter; }

//RTransaction RStorage::setCurrentLayer(RLayer::Id layerId) {
//    RTransaction transaction(*this, "Setting current layer", true);
//    setCurrentLayer(transaction, layerId);
//    transaction.end();
//    return transaction;
//}

//RTransaction RStorage::setCurrentLayer(const QString& layerName) {
//    RLayer::Id id = getLayerId(layerName);
//    if (id == RLayer::INVALID_ID) {
//        return RTransaction();
//    }
//    return setCurrentLayer(id);
//}

//void RStorage::setCurrentLayer(RTransaction& transaction, RLayer::Id layerId) {
//    QSharedPointer<RDocumentVariables> v = queryDocumentVariables();
//    v->setCurrentLayerId(layerId);
//    transaction.addObject(v);
//}

//void RStorage::setCurrentLayer(RTransaction& transaction, const QString& layerName) {
//    RLayer::Id id = getLayerId(layerName);
//    if (id == RLayer::INVALID_ID) {
//        return;
//    }
//    setCurrentLayer(transaction, id);
//}

void RStorage::setCurrentLayer(RLayer::Id layerId, RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    Q_ASSERT(!docVars.isNull());
    docVars->setCurrentLayerId(layerId);
    transaction->setType(RTransaction::CurrentLayerChange);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

void RStorage::setCurrentLayer(const QString &layerName,
                               RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    Q_ASSERT(!docVars.isNull());
    RLayer::Id layerId = getLayerId(layerName);
    if (layerId == RLayer::INVALID_ID) { return; }

    // old layer is affected object:
    transaction->addAffectedObject(docVars->getCurrentLayerId());
    docVars->setCurrentLayerId(layerId);
    transaction->setType(RTransaction::CurrentLayerChange);
    // new layer is affected object:
    transaction->addAffectedObject(layerId);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

RLayer::Id RStorage::getCurrentLayerId() const
{
    QSharedPointer<RDocumentVariables> v = queryDocumentVariablesDirect();
    if (v.isNull()) { return RLayer::INVALID_ID; }
    return v->getCurrentLayerId();
}

void RStorage::setCurrentColor(const RColor &color) { currentColor = color; }

RColor RStorage::getCurrentColor() const { return currentColor; }

void RStorage::setCurrentLineweight(RLineweight::Lineweight lw)
{
    currentLineweight = lw;
}

RLineweight::Lineweight RStorage::getCurrentLineweight() const
{
    return currentLineweight;
}

void RStorage::setCurrentLinetype(RLinetype::Id ltId)
{
    currentLinetypeId = ltId;
}

void RStorage::setCurrentLinetype(const QString &name)
{
    QSet<RLinetype::Id> ltIds = queryAllLinetypes();
    QSet<RLinetype::Id>::iterator it;
    for (it = ltIds.begin(); it != ltIds.end(); ++it)
    {
        QSharedPointer<RLinetype> lt = queryLinetypeDirect(*it);
        if (lt->getName().toUpper() == name.toUpper())
        {
            setCurrentLinetype(lt->getId());
            return;
        }
    }
}

void RStorage::setCurrentLinetypePattern(const RLinetypePattern &p)
{
    setCurrentLinetype(p.getName());
}

RLinetype::Id RStorage::getCurrentLinetypeId() const
{
    return currentLinetypeId;
}

RLinetypePattern RStorage::getCurrentLinetypePattern() const
{
    QSharedPointer<RLinetype> lt = queryCurrentLinetype();
    if (lt.isNull()) { return RLinetypePattern(); }
    return lt->getPattern();
}

bool RStorage::hasView(const QString &viewName) const
{
    QStringList sl = RS::toList<QString>(getViewNames());
    return sl.contains(viewName, Qt::CaseInsensitive);
}

bool RStorage::hasLayer(const QString &layerName) const
{
    QStringList sl = RS::toList<QString>(getLayerNames());
    return sl.contains(layerName, Qt::CaseInsensitive);
}

bool RStorage::hasLayerStates() const
{
    return !queryAllLayerStates().isEmpty();
}

bool RStorage::hasLayerState(const QString &layerStateName) const
{
    QStringList sl = RS::toList<QString>(getLayerStateNames());
    return sl.contains(layerStateName, Qt::CaseInsensitive);
}

bool RStorage::hasLinetype(const QString &linetypeName) const
{
    QStringList sl = RS::toList<QString>(getLinetypeNames());
    return sl.contains(linetypeName, Qt::CaseInsensitive);
}

QList<REntity::Id>
RStorage::orderBackToFront(const QSet<REntity::Id> &entityIds) const
{
    // new draw order definition:
    // based on draw order property and entity ID if draw order is identical

    // list of <ID, draw order> tuples:
    QList<QPair<REntity::Id, int>> res;
    QSet<REntity::Id>::const_iterator it;
    for (it = entityIds.begin(); it != entityIds.end(); ++it)
    {
        QSharedPointer<REntity> e = queryEntityDirect(*it);
        if (!e.isNull())
        {
            res.append(QPair<REntity::Id, int>(*it, e->getDrawOrder()));
        }
    }

    // sort by draw order and enitty ID:
    //qSort(res.begin(), res.end(), lessThan);
    std::sort(res.begin(), res.end(), lessThan);

    // copy into list of sorted IDs:
    QList<REntity::Id> ret;
    QList<QPair<REntity::Id, int>>::const_iterator it2;
    for (it2 = res.begin(); it2 != res.end(); it2++)
    {
        ret.append((*it2).first);
    }
    return ret;

    /*
    // old draw order definition:
    // only based on draw order property
    QMap<int, REntity::Id> res;
    QSet<REntity::Id>::const_iterator it;
    for (it = entityIds.begin(); it != entityIds.end(); ++it) {
        QSharedPointer<REntity> e = queryEntityDirect(*it);
        if (!e.isNull()) {
            res.insertMulti(e->getDrawOrder(), *it);
        }
    }
    return res.values();
    */
}

QList<RLayer::Id> RStorage::sortLayers(const QList<RLayer::Id> &layerIds) const
{
    QList<RLayer::Id> ret = layerIds;
    return ret;
}

bool RStorage::lessThan(const QPair<REntity::Id, int> &p1,
                        const QPair<REntity::Id, int> &p2)
{
    if (p1.second == p2.second) { return p1.first < p2.first; }
    return p1.second < p2.second;
}

int RStorage::getMinDrawOrder()
{
    QSet<REntity::Id> entityIds = queryAllEntities(false);
    QSet<REntity::Id>::const_iterator it;
    int minDrawOrder = maxDrawOrder;
    for (it = entityIds.begin(); it != entityIds.end(); ++it)
    {
        QSharedPointer<REntity> e = queryEntityDirect(*it);
        if (!e.isNull())
        {
            if (e->getDrawOrder() < minDrawOrder)
            {
                minDrawOrder = e->getDrawOrder();
            }
        }
    }
    return minDrawOrder - 1;
}

QSharedPointer<RDocumentVariables>
RStorage::startDocumentVariablesTransaction(RTransaction *&transaction,
                                            bool &useLocalTransaction)
{
    useLocalTransaction = (transaction == NULL);

    if (useLocalTransaction)
    {
        transaction = new RTransaction(*this, "Change document setting", true);
        transaction->setType(RTransaction::ChangeDocumentSetting);
    }

    return queryDocumentVariables();
}

void RStorage::endDocumentVariablesTransaction(
        RTransaction *transaction, bool useLocalTransaction,
        QSharedPointer<RDocumentVariables> docVars)
{
    transaction->addObject(docVars);


    if (useLocalTransaction)
    {
        transaction->end();
        delete transaction;
    }
}

void RStorage::setUnit(RS::Unit unit, RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    Q_ASSERT(!docVars.isNull());
    docVars->setUnit(unit);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

RS::Unit RStorage::getUnit() const
{
    QSharedPointer<RDocumentVariables> v = queryDocumentVariablesDirect();
    if (v.isNull()) { return RS::None; }
    return v->getUnit();
}

void RStorage::setMeasurement(RS::Measurement m, RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    Q_ASSERT(!docVars.isNull());
    docVars->setMeasurement(m);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

RS::Measurement RStorage::getMeasurement() const
{
    QSharedPointer<RDocumentVariables> v = queryDocumentVariablesDirect();
    if (v.isNull()) { return RS::UnknownMeasurement; }
    return v->getMeasurement();
}

void RStorage::setDimensionFont(const QString &f, RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    Q_ASSERT(!docVars.isNull());
    docVars->setDimensionFont(f);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

QString RStorage::getDimensionFont() const
{
    QSharedPointer<RDocumentVariables> v = queryDocumentVariablesDirect();
    if (v.isNull()) { return "Standard"; }
    return v->getDimensionFont();
}

/**
 * Sets the global linetype scale of this document to the given value.
 *
 * \param transaction Transaction to use for the modification or NULL to
 * create a local transaction (default).
 */
void RStorage::setLinetypeScale(double v, RTransaction *transaction)
{
    bool useLocalTransaction;
    QSharedPointer<RDocumentVariables> docVars =
            startDocumentVariablesTransaction(transaction, useLocalTransaction);
    docVars->setLinetypeScale(v);
    endDocumentVariablesTransaction(transaction, useLocalTransaction, docVars);
}

double RStorage::getLinetypeScale() const
{
    QSharedPointer<RDocumentVariables> v = queryDocumentVariablesDirect();
    if (v.isNull()) { return 1.0; }
    return v->getLinetypeScale();
}

/**
 * \return Name of the given known variable enum. Used for debugging.
 */
QString RStorage::getKnownVariableName(RS::KnownVariable n)
{
    switch (n)
    {
        case RS::ANGBASE:
            return "ANGBASE";
        case RS::ANGDIR:
            return "ANGDIR";
        case RS::ATTMODE:
            return "ATTMODE";
        case RS::AUNITS:
            return "AUNITS";
        case RS::AUPREC:
            return "AUPREC";
        case RS::CECOLOR:
            return "CECOLOR";
        case RS::CELTSCALE:
            return "CELTSCALE";
        case RS::CHAMFERA:
            return "CHAMFERA";
        case RS::CHAMFERB:
            return "CHAMFERB";
        case RS::CHAMFERC:
            return "CHAMFERC";
        case RS::CHAMFERD:
            return "CHAMFERD";
        case RS::CMLJUST:
            return "CMLJUST";
        case RS::CMLSCALE:
            return "CMLSCALE";
        case RS::DIMADEC:
            return "DIMADEC";
        case RS::DIMALT:
            return "DIMALT";
        case RS::DIMALTD:
            return "DIMALTD";
        case RS::DIMALTF:
            return "DIMALTF";
        case RS::DIMALTRND:
            return "DIMALTRND";
        case RS::DIMALTTD:
            return "DIMALTTD";
        case RS::DIMALTTZ:
            return "DIMALTTZ";
        case RS::DIMALTU:
            return "DIMALTU";
        case RS::DIMALTZ:
            return "DIMALTZ";
        case RS::DIMAPOST:
            return "DIMAPOST";
        case RS::DIMASZ:
            return "DIMASZ";
        case RS::DIMATFIT:
            return "DIMATFIT";
        case RS::DIMAUNIT:
            return "DIMAUNIT";
        case RS::DIMAZIN:
            return "DIMAZIN";
        case RS::DIMBLK:
            return "DIMBLK";
        case RS::DIMBLK1:
            return "DIMBLK1";
        case RS::DIMBLK2:
            return "DIMBLK2";
        case RS::DIMCEN:
            return "DIMCEN";
        case RS::DIMCLRD:
            return "DIMCLRD";
        case RS::DIMCLRE:
            return "DIMCLRE";
        case RS::DIMCLRT:
            return "DIMCLRT";
        case RS::DIMDEC:
            return "DIMDEC";
        case RS::DIMDLE:
            return "DIMDLE";
        case RS::DIMDLI:
            return "DIMDLI";
        case RS::DIMDSEP:
            return "DIMDSEP";
        case RS::DIMEXE:
            return "DIMEXE";
        case RS::DIMEXO:
            return "DIMEXO";
        case RS::DIMFRAC:
            return "DIMFRAC";
        case RS::DIMGAP:
            return "DIMGAP";
        case RS::DIMJUST:
            return "DIMJUST";
        case RS::DIMLDRBLK:
            return "DIMLDRBLK";
        case RS::DIMLFAC:
            return "DIMLFAC";
        case RS::DIMLIM:
            return "DIMLIM";
        case RS::DIMLUNIT:
            return "DIMLUNIT";
        case RS::DIMLWD:
            return "DIMLWD";
        case RS::DIMLWE:
            return "DIMLWE";
        case RS::DIMPOST:
            return "DIMPOST";
        case RS::DIMRND:
            return "DIMRND";
        case RS::DIMSAH:
            return "DIMSAH";
        case RS::DIMSCALE:
            return "DIMSCALE";
        case RS::DIMSD1:
            return "DIMSD1";
        case RS::DIMSD2:
            return "DIMSD2";
        case RS::DIMSE1:
            return "DIMSE1";
        case RS::DIMSE2:
            return "DIMSE2";
        case RS::DIMSOXD:
            return "DIMSOXD";
        case RS::DIMTAD:
            return "DIMTAD";
        case RS::DIMTDEC:
            return "DIMTDEC";
        case RS::DIMTFAC:
            return "DIMTFAC";
        case RS::DIMTIH:
            return "DIMTIH";
        case RS::DIMTIX:
            return "DIMTIX";
        case RS::DIMTM:
            return "DIMTM";
        case RS::DIMTOFL:
            return "DIMTOFL";
        case RS::DIMTOH:
            return "DIMTOH";
        case RS::DIMTOL:
            return "DIMTOL";
        case RS::DIMTOLJ:
            return "DIMTOLJ";
        case RS::DIMTP:
            return "DIMTP";
        case RS::DIMTSZ:
            return "DIMTSZ";
        case RS::DIMTVP:
            return "DIMTVP";
        case RS::DIMTXSTY:
            return "DIMTXSTY";
        case RS::DIMTXT:
            return "DIMTXT";
        case RS::DIMTZIN:
            return "DIMTZIN";
        case RS::DIMUPT:
            return "DIMUPT";
        case RS::DIMZIN:
            return "DIMZIN";
        case RS::DISPSILH:
            return "DISPSILH";
        case RS::DRAWORDERCTL:
            return "DRAWORDERCTL";
        case RS::ELEVATION:
            return "ELEVATION";
        case RS::EXTMAX:
            return "EXTMAX";
        case RS::EXTMIN:
            return "EXTMIN";
        case RS::FACETRES:
            return "FACETRES";
        case RS::FILLETRAD:
            return "FILLETRAD";
        case RS::FILLMODE:
            return "FILLMODE";
        case RS::INSBASE:
            return "INSBASE";
        case RS::INSUNITS:
            return "INSUNITS";
        case RS::ISOLINES:
            return "ISOLINES";
        case RS::LIMCHECK:
            return "LIMCHECK";
        case RS::LIMMAX:
            return "LIMMAX";
        case RS::LIMMIN:
            return "LIMMIN";
        case RS::LTSCALE:
            return "LTSCALE";
        case RS::LUNITS:
            return "LUNITS";
        case RS::LUPREC:
            return "LUPREC";
        case RS::MAXACTVP:
            return "MAXACTVP";
        case RS::MEASUREMENT:
            return "MEASUREMENT";
        case RS::MIRRTEXT:
            return "MIRRTEXT";
        case RS::ORTHOMODE:
            return "ORTHOMODE";
        case RS::PDMODE:
            return "PDMODE";
        case RS::PDSIZE:
            return "PDSIZE";
        case RS::PELEVATION:
            return "PELEVATION";
        case RS::PELLIPSE:
            return "PELLIPSE";
        case RS::PEXTMAX:
            return "PEXTMAX";
        case RS::PEXTMIN:
            return "PEXTMIN";
        case RS::PINSBASE:
            return "PINSBASE";
        case RS::PLIMCHECK:
            return "PLIMCHECK";
        case RS::PLIMMAX:
            return "PLIMMAX";
        case RS::PLIMMIN:
            return "PLIMMIN";
        case RS::PLINEGEN:
            return "PLINEGEN";
        case RS::PLINEWID:
            return "PLINEWID";
        case RS::PROXYGRAPHICS:
            return "PROXYGRAPHICS";
        case RS::PSLTSCALE:
            return "PSLTSCALE";
        case RS::PUCSNAME:
            return "PUCSNAME";
        case RS::PUCSORG:
            return "PUCSORG";
        case RS::PUCSXDIR:
            return "PUCSXDIR";
        case RS::PUCSYDIR:
            return "PUCSYDIR";
        case RS::QTEXTMODE:
            return "QTEXTMODE";
        case RS::REGENMODE:
            return "REGENMODE";
        case RS::SHADEDGE:
            return "SHADEDGE";
        case RS::SHADEDIF:
            return "SHADEDIF";
        case RS::SKETCHINC:
            return "SKETCHINC";
        case RS::SKPOLY:
            return "SKPOLY";
        case RS::SPLFRAME:
            return "SPLFRAME";
        case RS::SPLINESEGS:
            return "SPLINESEGS";
        case RS::SPLINETYPE:
            return "SPLINETYPE";
        case RS::SURFTAB1:
            return "SURFTAB1";
        case RS::SURFTAB2:
            return "SURFTAB2";
        case RS::SURFTYPE:
            return "SURFTYPE";
        case RS::SURFU:
            return "SURFU";
        case RS::SURFV:
            return "SURFV";
        case RS::TEXTQLTY:
            return "TEXTQLTY";
        case RS::TEXTSIZE:
            return "TEXTSIZE";
        case RS::THICKNESS:
            return "THICKNESS";
        case RS::TILEMODE:
            return "TILEMODE";
        case RS::TRACEWID:
            return "TRACEWID";
        case RS::TREEDEPTH:
            return "TREEDEPTH";
        case RS::UCSNAME:
            return "UCSNAME";
        case RS::UCSORG:
            return "UCSORG";
        case RS::UCSXDIR:
            return "UCSXDIR";
        case RS::UCSYDIR:
            return "UCSYDIR";
        case RS::UNITMODE:
            return "UNITMODE";
        case RS::USERI1:
            return "USERI1";
        case RS::USERI2:
            return "USERI2";
        case RS::USERI3:
            return "USERI3";
        case RS::USERI4:
            return "USERI4";
        case RS::USERI5:
            return "USERI5";
        case RS::USERR1:
            return "USERR1";
        case RS::USERR2:
            return "USERR2";
        case RS::USERR3:
            return "USERR3";
        case RS::USERR4:
            return "USERR4";
        case RS::USERR5:
            return "USERR5";
        case RS::USRTIMER:
            return "USRTIMER";
        case RS::VISRETAIN:
            return "VISRETAIN";
        case RS::WORLDVIEW:
            return "WORLDVIEW";
        default:
            return "Unknown";
    }
}

void RStorage::setModified(bool m)
{
    bool wasModified = isModified();

    if (m == true)
    {
        lastModified = QDateTime::currentDateTime();
        modified = true;

        // document was unmodified, is now modified:
        if (!wasModified) {}
    }
    else
    {
        modified = false;

        // document was modified, is now unmodified:
        if (wasModified) {}
    }
}

bool RStorage::isLayerLocked(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isLayerLocked(*l);
}

bool RStorage::isLayerLocked(const RLayer &layer) const
{
    if (layer.isLocked()) { return true; }
    return isParentLayerLocked(layer);
}

/**
 * \return True if a parent layer of the given layer is locked.
 */
bool RStorage::isParentLayerLocked(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isParentLayerLocked(*l);
}

bool RStorage::isParentLayerLocked(const RLayer &layer) const
{
    RLayer::Id parentLayerId = layer.getParentLayerId();
    if (parentLayerId == RLayer::INVALID_ID)
    {
        // no parent:
        return false;
    }
    QSharedPointer<RLayer> pl = queryLayerDirect(parentLayerId);
    if (pl->isLocked()) { return true; }
    return isParentLayerLocked(*pl);
}

/**
 * \return True if this layer is off.
 */
bool RStorage::isLayerOff(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isLayerOff(*l);
}

bool RStorage::isLayerOff(const RLayer &layer) const
{
    if (layer.isOff()) { return true; }
    return false;
}

/**
 * \return True if this layer is off or frozen.
 */
bool RStorage::isLayerOffOrFrozen(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }

    return isLayerOff(*l) || isLayerFrozen(*l);
}

bool RStorage::isLayerOffOrFrozen(const RLayer &layer) const
{
    return isLayerOff(layer) || isLayerFrozen(layer);
}

/**
 * \return True if this layer or one of its parent layers are frozen.
 */
bool RStorage::isLayerFrozen(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isLayerFrozen(*l);
}

/**
 * \return True if this layer or one of its parent layers are frozen.
 */
bool RStorage::isLayerFrozen(const RLayer &layer) const
{
    if (layer.isFrozen()) { return true; }
    return isParentLayerFrozen(layer);
}

/**
 * \return True if a parent layer of the given layer is frozen.
 */
bool RStorage::isParentLayerFrozen(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isParentLayerFrozen(*l);
}

bool RStorage::isParentLayerFrozen(const RLayer &layer) const
{
    RLayer::Id parentLayerId = layer.getParentLayerId();
    if (parentLayerId == RLayer::INVALID_ID)
    {
        // no parent:
        return false;
    }
    QSharedPointer<RLayer> pl = queryLayerDirect(parentLayerId);
    if (pl->isFrozen()) { return true; }
    return isParentLayerFrozen(*pl);
}

/**
 * \return True if a parent layer of the given layer is snappable.
 */
bool RStorage::isParentLayerSnappable(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return true; }
    return isParentLayerSnappable(*l);
}

bool RStorage::isParentLayerSnappable(const RLayer &layer) const
{
    RLayer::Id parentLayerId = layer.getParentLayerId();
    if (parentLayerId == RLayer::INVALID_ID)
    {
        // no parent:
        return true;
    }
    QSharedPointer<RLayer> pl = queryLayerDirect(parentLayerId);
    if (!pl->isSnappable()) { return false; }
    return isParentLayerSnappable(*pl);
}

/**
 * \return True if this layer and its parent layers are snappable.
 */
bool RStorage::isLayerSnappable(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isLayerSnappable(*l);
}

/**
 * \return True if this layer and its parent layers are snappable.
 */
bool RStorage::isLayerSnappable(const RLayer &layer) const
{
    if (!layer.isSnappable()) { return false; }
    return isParentLayerSnappable(layer);
}

/**
 * \return True if this layer and its parent layers are plottable.
 */
bool RStorage::isLayerPlottable(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isLayerPlottable(*l);
}

/**
 * \return True if this layer and its parent layers are plottable.
 */
bool RStorage::isLayerPlottable(const RLayer &layer) const
{
    if (!layer.isPlottable()) { return false; }
    return isParentLayerPlottable(layer);
}

/**
 * \return True if a parent layer of the given layer is plottable.
 */
bool RStorage::isParentLayerPlottable(RLayer::Id layerId) const
{
    QSharedPointer<RLayer> l = queryLayerDirect(layerId);
    if (l.isNull()) { return false; }
    return isParentLayerPlottable(*l);
}

bool RStorage::isParentLayerPlottable(const RLayer &layer) const
{
    RLayer::Id parentLayerId = layer.getParentLayerId();
    if (parentLayerId == RLayer::INVALID_ID)
    {
        // no parent:
        return true;
    }
    QSharedPointer<RLayer> pl = queryLayerDirect(parentLayerId);
    if (!pl->isPlottable()) { return false; }
    return isParentLayerPlottable(*pl);
}