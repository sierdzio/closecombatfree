#ifndef CCFQMLBASESCENARIO_H
#define CCFQMLBASESCENARIO_H

#include <QtQuick/QQuickItem>

/*!
 * \brief The CcfQmlBaseScenario class is an experiment to see if moving most functionality
 * from Scenario.qml to here is sane, doable, and does not require too much effort
 * and nasty workarounds.
 */
class CcfQmlBaseScenario : public QQuickItem
{
    Q_OBJECT
public:
    explicit CcfQmlBaseScenario(QQuickItem *parent = 0);
};

#endif // CCFQMLBASESCENARIO_H
