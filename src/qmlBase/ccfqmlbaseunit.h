#ifndef CCFQMLBASEUNIT_H
#define CCFQMLBASEUNIT_H

#include <QtQuick/QQuickItem>

/*!
 * \brief The CcfQmlBaseUnit class is an experiment to see if moving most functionality
 * from Unit.qml to here is sane, doable, and does not require too much effort
 * and nasty workarounds.
 */
class CcfQmlBaseUnit : public QQuickItem
{
    Q_OBJECT
public:
    explicit CcfQmlBaseUnit(QQuickItem *parent = 0);
};

#endif // CCFQMLBASEUNIT_H
