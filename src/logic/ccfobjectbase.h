#ifndef CCFOBJECTBASE_H
#define CCFOBJECTBASE_H

#include <QString>
#include <QObject>
#include <QVariant>
#include <QQuickItem>
#include "ccfglobalobjectbase.h"

class CcfQmlBaseUnit;

/*!
  QQuickItem extension class, used as a base for many QML items in Ccf. Contains
  some convenience defines (set, get, getReal, getInt, getString, getBool) for
  easy property manipulation. Also sports some particularly useful convenience
  methods.
  */
class CcfObjectBase : public QQuickItem
{
    Q_OBJECT

public:
    explicit CcfObjectBase(QQuickItem *parent = 0);
    
    CcfQmlBaseUnit *ccfUnit(QObject *unitObject) const;
    QQuickItem *item(const QString &objectName) const;
    QQuickItem *item(QObject *object);
    QObject *child(const QString &objectName) const;
    bool invoke(QObject *obj, const char *member,
                    QGenericArgument val0 = QGenericArgument(0), QGenericArgument val1 = QGenericArgument(),
                    QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(),
                    QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(),
                    QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(),
                    QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument());
    QList<CcfQmlBaseUnit *> objectToUnitList(QObjectList list);
};

#endif // CCFOBJECTBASE_H
