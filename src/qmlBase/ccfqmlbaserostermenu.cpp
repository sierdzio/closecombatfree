#include "ccfqmlbaserostermenu.h"
#include "qmlBase/ccfqmlbaseunit.h"

CcfQmlBaseRosterMenu::CcfQmlBaseRosterMenu(QQuickItem *parent) :
    CcfObjectBase(parent)
{
}

/*!
  Populates roster menu with units, calculates best fit and fixes the size.
  */
void CcfQmlBaseRosterMenu::populateUnits(QObjectList tmpUnitsList)
{
    units = tmpUnitsList;

    QObjectList model;
    for (int i = 0; i < units.length(); ++i) {
        model.append(units.at(i));
        connect(ccfUnit(units.at(i)), &CcfQmlBaseUnit::selectedChanged,
                this, &CcfQmlBaseRosterMenu::selectionChanged);
    }

    // Get widest and highest dimension from cells:
    qreal widestCell = 0;
    qreal heighestCell = 0;
    ccfUnit(units.at(0))->setUnitStatus("MOVING FAST");

    set("unitModel", QVariant::fromValue(model));

    QObject *unitsView = child("unitsView");
    unitsView->set("currentIndex", 0);

    QQuickItem *current = item(unitsView->property("currentItem").value<QObject *>());
    widestCell = current->width();
    heighestCell = current->height();
    ccfUnit(units.at(0))->setUnitStatus("READY");

    // Set all elements' dimentions
    for (int i = 0; i < units.length(); ++i) {
        unitsView->set("currentIndex", i);
        current = item(unitsView->property("currentItem").value<QObject *>());
        current->set("fixedHeight", heighestCell);
        current->set("fixedWidth", widestCell);
    }

    unitsView->set("currentIndex", -1);
    child("unitsView")->set("cellWidth", widestCell + 2);
    child("unitsView")->set("cellHeight", heighestCell + 2);
    set("entryWidth", widestCell);
    set("entryHeight", heighestCell);
}

/*!
  Returns CcfQmlBaseUnit pointer to a unit specified by \a x and \a y coords
  of the roster.

  \sa getUnitIndexAt
  */
CcfQmlBaseUnit *CcfQmlBaseRosterMenu::getUnitAt(qreal x, qreal y)
{
    int i;
    metaObject()->invokeMethod(child("unitsView"), "indexAt",
                               Q_RETURN_ARG(int, i),
                               Q_ARG(qreal, x), Q_ARG(qreal, y));

    if (i == -1)
        return 0;

    if (units.length() <= i)
        return 0;

    return ccfUnit(units.at(i));
}

/*!
  Same as getUnitAt, but returns unit's index.

  \sa getUnitAt
  */
int CcfQmlBaseRosterMenu::getUnitIndexAt(qreal x, qreal y)
{
    int i;
    metaObject()->invokeMethod(child("unitsView"), "indexAt",
                               Q_RETURN_ARG(int, i),
                               Q_ARG(qreal, x), Q_ARG(qreal, y));

    if (i == -1)
        return 0;

    if (units.length() <= i)
        return 0;

    return i;
}

/*!
  Returns a point denoting the center of roster's entry, for a given \a x and \a y.
  */
QPointF CcfQmlBaseRosterMenu::childCenterCoords(qreal x, qreal y)
{
    int index;
    QObject *unitsView = child("unitsView");
    metaObject()->invokeMethod(unitsView, "indexAt",
                               Q_RETURN_ARG(int, index),
                               Q_ARG(qreal, x), Q_ARG(qreal, y));

    int row;
    row = index % getInt("rows");
    int column;
    column = index / getInt("rows");

    int cellWidth = unitsView->getInt("cellWidth");
    int cellHeight = unitsView->getInt("cellHeight");

    QPointF result(((column * cellWidth) + (cellWidth/2)),
                   ((row * cellHeight) + (cellHeight/2)));

    return result;
}

/*!
  Updates unit's status to \a newStatus. Unit is identified by \a index.
  */
void CcfQmlBaseRosterMenu::changeStatus(const QString &newStatus, int index)
{
    ccfUnit(units.at(index))->setUnitStatus(newStatus);
}

/*!
  Selects or deselects a unit based on \a state. Unit is identified by \a index.
  */
void CcfQmlBaseRosterMenu::selectionChanged(bool state, int index)
{
    ccfUnit(units.at(index))->setSelected(state);
}
