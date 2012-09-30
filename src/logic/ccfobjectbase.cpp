#include "ccfobjectbase.h"
#include "qmlBase/ccfqmlbaseunit.h"

#include <QtQml/QQmlListReference>

CcfObjectBase::CcfObjectBase(QQuickItem *parent) : QQuickItem(parent)
{
}

CcfQmlBaseUnit *CcfObjectBase::ccfUnit(QObject *unitObject) const
{
    return qobject_cast<CcfQmlBaseUnit *>(unitObject);
}

QQuickItem *CcfObjectBase::item(const QString &objectName) const
{
    return findChild<QQuickItem *>(objectName);
}

QQuickItem *CcfObjectBase::item(QObject *object)
{
    return qobject_cast<QQuickItem *>(object);
}

/*!
  Returns a children of this object, specified by \a objectName and cast to QObject.
  */
QObject *CcfObjectBase::child(const QString &objectName) const
{
    return findChild<QObject *>(objectName);
}

/*!
  Invokes a method. Shortened syntax of metaObject::invokeMehtod().
  */
bool CcfObjectBase::invoke(QObject *obj, const char *member, QGenericArgument val0, QGenericArgument val1,
                                QGenericArgument val2, QGenericArgument val3, QGenericArgument val4,
                                QGenericArgument val5, QGenericArgument val6, QGenericArgument val7,
                                QGenericArgument val8, QGenericArgument val9)
{
    return obj->metaObject()->invokeMethod(obj, member, val0, val1, val2, val3, val4, val5,
                                           val6, val7, val8, val9);
}

/*!
  Converts a QQmlListReference \a toConvert into a more robust QList<QObject *>.

  Warning: conversion includes traversing the whole reference list. Not necessarily an
  optimal way of doing things.
  */
QObjectList CcfObjectBase::listReferenceToQList(const QQmlListReference &toConvert)
{
    QObjectList result;
    for (int i = 0; i < toConvert.count(); ++i)
        result.append(toConvert.at(i));
    return result;
}

/*!
  Converts a QQmlListReference \a toConvert into a more robust QList<CcfQmlBaseUnit *>.

  Warning: conversion includes traversing the whole reference list. Not necessarily an
  optimal way of doing things.
  */
QList<CcfQmlBaseUnit *> CcfObjectBase::listReferenceToUnitList(const QQmlListReference &toConvert)
{
    QList<CcfQmlBaseUnit *> result;
    for (int i = 0; i < toConvert.count(); ++i)
        result.append(ccfUnit(toConvert.at(i)));
    return result;
}
