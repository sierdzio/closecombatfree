#ifndef CCFQMLBASEROSTERMENU_H
#define CCFQMLBASEROSTERMENU_H

#include <QtCore/QList>

#include "logic/ccfobjectbase.h"
#include "qmlBase/ccfqmlbaseunit.h"

class CcfQmlBaseRosterMenu : public CcfObjectBase
{
    Q_OBJECT
public:
    explicit CcfQmlBaseRosterMenu(QQuickItem *parent = 0);

    Q_INVOKABLE void populateUnits(QList<CcfQmlBaseUnit *> tmpUnitsList);
    Q_INVOKABLE CcfQmlBaseUnit *getUnitAt(qreal x, qreal y);
    Q_INVOKABLE int getUnitIndexAt(qreal x, qreal y);
    QPointF childCenterCoords(qreal x, qreal y);

public slots:
    void changeStatus(const QString &newStatus, int index);
    void selectionChanged(bool state, int index);

private:
    QList<CcfQmlBaseUnit *> units;
};

#endif // CCFQMLBASEROSTERMENU_H
