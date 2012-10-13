#include "ccfqmlbasescenario.h"
#include "ccfmain.h"
#include "config/ccfconfig.h"
#include "qmlBase/ccfqmlbaseunit.h"
#include "qmlBase/ccfqmlbaserostermenu.h"

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlListProperty>
#include <QtQuick/QQuickItem>

/*!
  Standard constructor. Initialises properties to default values.
  */
CcfQmlBaseScenario::CcfQmlBaseScenario(QQuickItem *parent) : CcfObjectBase(parent)
{
    m_zoom = 1.0;
    m_paused = false;
    m_menuBackgroundColor = QColor("#7e8c24");
    m_aimLineRotation = 0;
    m_isCampaign = false;

    m_mainInstance = CcfMain::instance();
    m_effectsComponent = new QQmlComponent(m_mainInstance->engine(),
                                           QUrl::fromLocalFile("qml/effects/Effect.qml"));

    connect(this, &CcfQmlBaseScenario::togglePause, this, &CcfQmlBaseScenario::onTogglePause);
}

/*!
  Sets internal pointers to frequently used QML items, thus reducing the overhead
  of repeated searching for them at runtime.
  */
void CcfQmlBaseScenario::init()
{
    if (m_scenarioFile != "") {
        QStringList unitSideList;
        QObject *unitsLoader = child("unitsLoader");
        QObject *unitItem = unitsLoader->property("unitsItem").value<QObject *>();
        QObject *map = child("map");
        QObject *mapItem = NULL;

        if (unitItem->objectName() != "Campaign") {
            // This is a single scenario
            map->set("source", unitItem->getString("mapFile"));
            QObjectList tempList = unitItem->children();

            for (int i = 0; i < tempList.count(); ++i) {
                CcfQmlBaseUnit *unit = ccfUnit(tempList.at(i));
                if ((unit != 0) && (unit->property("unitIndex").isValid())) {
                    unit->setUnitIndex(i);
                    connect(this, &CcfQmlBaseScenario::togglePause,
                            unit, &CcfQmlBaseUnit::togglePause);
                    connect(unit, &CcfQmlBaseUnit::actionFinished,
                            this, &CcfQmlBaseScenario::actionFinished);
                    connect(unit, &CcfQmlBaseUnit::movementStateChange,
                            this, &CcfQmlBaseScenario::handleUnitMovement);
                    unitSideList.append(unit->getUnitSide());
                    m_units.append(unit);
                }
            }

            mapItem = map->property("mapItem").value<QObject *>();

            if(mapItem != 0)
                invoke(mapItem, "setUnits", Q_ARG(QVariant, QVariant::fromValue(m_units) ));
            else
                mmain->logger()->error("MapItem object was not properly initialised",
                                       "Robin Hood is a jerk");
        } else {
            // This is a campaign
            // TODO: add some clever code here ;)
        }

        mmain->scenarioState()->setAvailableSides(unitSideList);

        m_aimLine = item("aimLine");
        m_gameArea = item("gameArea");
        m_zoomArea = item("zoomArea");
        m_contextMenu = item("contextMenu");
        m_effectsTimer = child("effectsTimer");
        m_rotationTimer = child("rotationTimer");
        m_followingTimer = child("followingTimer");
        m_mouseAreaMain = child("mouseAreaMain");

        QObjectList mapItemKids = mapItem->children();
        for (int i = 0; i < mapItemKids.length() - 2; ++i) {
            m_props.append(mapItemKids.at(i + 2));
        }

        m_roster = findChild<CcfQmlBaseRosterMenu *>("roster");
        m_roster->populateUnits(m_units);

        connect(m_contextMenu, SIGNAL(menuEntryClicked(QString)), this, SLOT(scheduleContextAction(QString)));
        connect(m_rotationTimer, SIGNAL(triggered()), this, SLOT(updateAimLine()));
        connect(m_effectsTimer, SIGNAL(triggered()), this, SLOT(updateEffects()));
        connect(m_followingTimer, SIGNAL(triggered()), this, SLOT(updateFollowingUnit()));
        connect(child("sceneUpdateTimer"), SIGNAL(triggered()), this, SLOT(updateUnitVisibility()));
        connect(child("rubberBandTimer"), SIGNAL(triggered()), this, SLOT(updateRubberBand()));

        hideNonPlayerUnits();
        setSideMarks();
    }
}

/*!
  Zooms the game area in by 10%.
  */
void CcfQmlBaseScenario::zoomIn()
{
    m_zoom += 0.1;
}

/*!
  Zooms the game area out by 10%.
  */
void CcfQmlBaseScenario::zoomOut()
{
    m_zoom -= 0.1;
}

/*!
  Returns a QObjectList of units that are on the side of \a player.
  */
QObjectList CcfQmlBaseScenario::playerUnits(const QString &player)
{
    QList<QObject *> unitsArray;
    for (int i = 0; i < m_units.count(); ++i) {
        if (m_units.at(i)->getString("unitSide") == player) {
            unitsArray.append(m_units.at(i));
        }
    }
    return unitsArray;
}

/*!
  Returns a QObjectList of units that are not on the side of \a player.
  */
QObjectList CcfQmlBaseScenario::enemyUnits(const QString &player)
{
    QList<QObject *> unitsArray;
    for (int i = 0; i < m_units.count(); ++i) {
        if (m_units.at(i)->getString("unitSide") != player) {
            unitsArray.append(m_units.at(i));
        }
    }
    return unitsArray;
}

/*!
  Creates an unit group. Usually done with CTRL+number.
  */
void CcfQmlBaseScenario::groupUnits(int groupNumber)
{
    if ((groupNumber < 0) || (selectedUnitsCount() == 0)) {
        return;
    }

    // Remove old members.
    if (groupNumber < m_unitGroups.count()) {
        for (int i = 0; i < m_unitGroups.value(groupNumber).length(); ++i) {
            if (m_unitGroups.value(groupNumber).at(i)->getBool("selected") == false)
                m_unitGroups.value(groupNumber).at(i)->set("groupNumber", 0);
        }
    }

    QObjectList group = selectedUnits();
    m_unitGroups.insert(groupNumber, group);

    for (int i = 0; i < group.length(); ++i) {
        group.at(i)->set("groupNumber", groupNumber);
    }

    //    console.log("Group " + groupNumber + " created.");
    mmain->global()->statusMessage(tr("Group %1 created.").arg(groupNumber));
}

/*!
  Selects a whole unit group.

  It has to be created first, of course.
  */
void CcfQmlBaseScenario::selectGroup(int groupNumber)
{
    if (groupNumber < 0)
        return;

    QObjectList group = m_unitGroups.value(groupNumber);
    deselectAllUnits();

    for (int i = 0; i < group.length(); ++i) {
        group.at(i)->set("selected", true);
    }
}

/*!
  Cancels order if there are obstacles in LOS.
  */
bool CcfQmlBaseScenario::checkIfUnitCanFire(const QString &scheduledOperation)
{
    // Take obstacles into account
    if ((scheduledOperation == "Attack") || (scheduledOperation == "Smoke")) {
        if (m_aimLine->getReal("invisibleBeginning") < m_aimLine->height()) {
            cleanContextAction();
            return false;
        }
    }

    return true;
}

/*!
  Called when user "places order", that is, when they click left mouse button
  or tap on the screen.
  */
void CcfQmlBaseScenario::performContextAction(int index, qreal targetX, qreal targetY)
{
    QList<QObject *> selectedGroup = selectedUnits();
    CcfQmlBaseUnit *unit = ccfUnit(m_units.at(index));
    QString scheduledOperation = m_aimLine->getString("scheduledOperation");

    if ((scheduledOperation != "Ambush")
            && (scheduledOperation != "Defend")
            && (scheduledOperation != "Stop")
            && (scheduledOperation != "Follow")) {
        if (!checkIfUnitCanFire(scheduledOperation)) {
            return;
        }

        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueActionOrder(unit, targetX, targetY, scheduledOperation);

        for (int i = 0; i < selectedGroup.length(); ++i) {
            unit = ccfUnit(selectedGroup.at(i));

            // This unit's order is already issued.
            if (unit->getUnitIndex() == index)
                continue;

            qreal tempX = targetX + (unit->x() - m_units.at(index)->getReal("x"));
            qreal tempY = targetY + (unit->y() - m_units.at(index)->getReal("y"));

            issueActionOrder(unit, tempX, tempY, scheduledOperation);
        }
    }

    cleanContextAction();
}

/*!
  Puts a waypoint on the map, also adds order to unit's order queue.
  */
void CcfQmlBaseScenario::placeWaypoint(int index, qreal targetX, qreal targetY)
{
    QList<QObject *> selectedGroup = selectedUnits();
    CcfQmlBaseUnit *unit = ccfUnit(m_units.at(index));
    QString scheduledOperation = m_aimLine->getString("scheduledOperation");

    if ((scheduledOperation != "Ambush")
            && (scheduledOperation != "Defend")
            && (scheduledOperation != "Stop")
            && (scheduledOperation != "Follow")) {
        if (!checkIfUnitCanFire(scheduledOperation)) {
            return;
        }

        // Set up the unit to which the aimLine is anchored.
        // Others are set in the loop later, based on this "main"
        // object.
        issueWaypointOrder(unit, targetX, targetY, scheduledOperation);


        for (int i = 0; i < selectedGroup.length(); ++i) {
            unit = ccfUnit(selectedGroup.at(i));

            // This unit's order is already issued.
            if (unit->getUnitIndex() == index)
                continue;

            qreal tempX = targetX + (unit->x() - m_units.at(index)->getReal("x"));
            qreal tempY = targetY + (unit->y() - m_units.at(index)->getReal("y"));

            issueWaypointOrder(unit, tempX, tempY, scheduledOperation);
        }
    }
}

/*!
  Issues a waypoint order (adds order to unit's queue).
  */
void CcfQmlBaseScenario::issueWaypointOrder(CcfQmlBaseUnit *unit, qreal x, qreal y, const QString &operation)
{
    // Clear defence, if it is on.
    unit->setDefenceSphereColor("");
    unit->changeStatus("READY");

    unit->queueOrder(operation, x, y, m_zoomArea);
}

/*!
  Issues an order (fires queue execution).
  */
void CcfQmlBaseScenario::issueActionOrder(CcfQmlBaseUnit *unit, qreal x, qreal y, const QString &operation)
{
    // Clear defence, if it is on.
    unit->setDefenceSphereColor("");
    unit->changeStatus("READY");

    if (operation == "Move") {
        unit->moveTo(x, y, m_zoomArea);
    } else if (operation == "Move fast") {
        unit->moveFastTo(x, y, m_zoomArea);
    } else if (operation == "Sneak") {
        unit->sneakTo(x, y, m_zoomArea);
    } else if (operation == "Attack") {
        unit->fireTo(x, y, m_zoomArea);
    } else if (operation == "Smoke") {
        unit->smokeTo(x, y, m_zoomArea);
    }
}

/*!
  Slot invoked when an unit finishes an action.
  */
void CcfQmlBaseScenario::actionFinished(int index, qreal targetX, qreal targetY)
{
    CcfQmlBaseUnit *unit = ccfUnit(m_units.at(index));

    if (unit->getCurrentOrder() != -1) {
        QString scheduledOperation = unit->operation(unit->getCurrentOrder());
        if ((scheduledOperation != "Move")
                && (scheduledOperation != "Move fast")
                && (scheduledOperation != "Sneak")
                && (scheduledOperation != "Follow")) {
            firingActionFinished(index, targetX, targetY);
            checkScenarioFinished();
        }
    }
}

/*!
  Schedules action chosen in context menu (or through a keyboard shortcut).
  It is then used to add that order to queue.
  */
void CcfQmlBaseScenario::scheduleContextAction(const QString &operation)
{
    CcfUnitList units = objectToUnitList(selectedUnits());

    m_aimLine->set("unitIndex", units.at(0)->property("unitIndex"));
    m_aimLine->set("scheduledOperation", operation);
    m_contextMenu->setVisible(false);

    if (operation == "Stop") {
        // Iterate over every unit!
        for (int i = 0; i < units.length(); ++i) {
            units.at(i)->cancelOrder();
        }
        cleanContextAction();
    } else if (operation == "Follow") {
        if (m_followingTimer->getBool("running"))
            stopFollowingUnit();
        else
            startFollowingUnit(units.at(0)->getInt("unitIndex"));
    } else {
        // Draw aim line for all move/attack operations.
        if ((operation != "Ambush") && (operation != "Defend")) {
            m_aimLine->setX(units.at(0)->x() + units.at(0)->getCenterX());
            m_aimLine->setY(units.at(0)->y() + units.at(0)->getCenterY());

            m_aimLine->set("color", CcfEngineHelpers::colorForOrder(operation));
            invoke(m_rotationTimer, "start");
            m_aimLine->setVisible(true);
        } else { // Draw defense 'spheres'
            if (operation == "Ambush") {
                for (int i = 0; i < units.length(); ++i) {
                    units.at(i)->setDefenceSphereColor("green");
                    ccfUnit(units.at(i))->changeStatus("AMBUSHING");
                }
            }
            else if (operation == "Defend") {
                for (int i = 0; i < units.length(); ++i) {
                    units.at(i)->setDefenceSphereColor("blue");
                    ccfUnit(units.at(i))->changeStatus("AMBUSHING");
                }
            }
            invoke(m_rotationTimer, "start");
        }
    }
}

/*!
  Toggles the in-game pause.
  */
void CcfQmlBaseScenario::onTogglePause()
{
    if (m_paused == true) {
        m_paused = false;
    } else {
        m_paused = true;
    }

    emit pausedChanged();
}

/*!
  Creates an instance of Effect.qml component.
  */
QObject *CcfQmlBaseScenario::createEffect(QObject *parent)
{
    if (m_effectsComponent->isReady()) {
        QObject *object = m_effectsComponent->create();
        object->set("parent", QVariant::fromValue(parent));
        return object;
    } else {
        return 0;
    }
}

/*!
  Invoked when a firing action is finished.

  Turns hit effects on, checks whether someone was hit, etc.
  */
void CcfQmlBaseScenario::firingActionFinished(int index, qreal targetX, qreal targetY) {
    QObject *unit = m_units.at(index);

    if (unit->getInt("currentOrder") != -1) {
        // TODO: remove use of effect for smoke and attack - and optimise it away.
        QObject *effect = createEffect(m_zoomArea);
        if (effect == 0)
            return;

        m_effects.append(effect);
        QString scheduledOperation;
        unit->metaObject()->invokeMethod(unit, "operation", Qt::DirectConnection, Q_RETURN_ARG(QString, scheduledOperation));

        if (scheduledOperation == "Attack") {
            effect->set("animationString", "gun_fire");
            // Check, whether a unit was hit.
            QObject *hitee = 0;
            hitee = unitAt(targetX, targetY);

            if ((hitee != 0)
                    && (hitee->property("unitIndex").isValid() == true)
                    && (hitee->property("unitType").isValid() == true)) {
                invoke(hitee, "hit", Q_ARG(QObject *, unit), Q_ARG(qreal, targetX), Q_ARG(qreal, targetY));
            }
        }
        else if(scheduledOperation == "Smoke") {
            effect->set("animationString", "smoke_fire");
        }

        effect->set("x", targetX);
        effect->set("y", targetY);
        effect->set("running", true);

        invoke(m_effectsTimer, "start");
    }
}

/*!
  Cleans context action (hides context menu and aimLine, etc.).
  */
void CcfQmlBaseScenario::cleanContextAction()
{
    invoke(m_rotationTimer, "stop");
    m_aimLine->setVisible(false);
    m_aimLine->setHeight(5.0);
    m_contextMenu->setVisible(false);
}

/*!
  Cancels all orders for all selected units.
  */
void CcfQmlBaseScenario::cancelAllSelectedOrders()
{
    QObjectList selected = selectedUnits();
    for (int i = 0; i < selected.length(); ++i) {
        invoke(selected.at(i), "cancelOrder");
    }
}

/*!
  Returns an array of all selected units.
  */
QList<QObject *> CcfQmlBaseScenario::selectedUnits()
{
    QList<QObject *> result;
    for (int i = 0; i < m_units.count(); ++i) {
        if (m_units.at(i)->getBool("selected") == true)
            result.append(m_units.at(i));
    }

    return result;
}

/*!
  Returns number of units that are selected.
  */
int CcfQmlBaseScenario::selectedUnitsCount()
{
    return selectedUnits().length();
}

/*!
  Returns Array of all units with exception of one, specified by unitIndex.

  If unitIndex is -1, this function returns ALL units.
  */
QObjectList CcfQmlBaseScenario::getAllUnitsButOne(int unitToOmit)
{
    QList<QObject *> result;

    for (int i = 0; i < m_units.count(); ++i) {
        if (m_units.at(i)->getInt("unitIndex") != unitToOmit) {
            result.append(m_units.at(i));
        }
    }

    return result;
}

/*!
  Centers view on a given unit.
  */
void CcfQmlBaseScenario::centerViewOnUnit(QObject *unit)
{
    m_gameArea->set("contentX", (((unit->getReal("x") + unit->getReal("centerX")) * m_zoom)
                                 - m_gameArea->width()/2));
    m_gameArea->set("contentY", (((unit->getReal("y") + unit->getReal("centerY")) * m_zoom)
                                 - m_gameArea->height()/2));
}

/*!
  Centers view on given coordinates.
  */
void CcfQmlBaseScenario::centerViewOn(qreal x, qreal y)
{
    m_gameArea->set("contentX", (x - m_gameArea->width()/2));
    m_gameArea->set("contentY", (y - m_gameArea->height()/2));
}

/*!
  Starts following a unit specified by it's index.
  */
void CcfQmlBaseScenario::startFollowingUnit(int index)
{
    m_followingTimer->set("index", index);
    m_followingTimer->set("running", true);
    centerViewOnUnit(m_units.at(index));
    child("followingInfoBox")->set("bodyText", QString("Unit name: " + m_units.at(index)->getString("unitType")
                                                       + "\nDouble click to stop."));
    invoke(m_followingTimer, "start");
}

/*!
  Stops following a unit.
  */
void CcfQmlBaseScenario::stopFollowingUnit()
{
    m_followingTimer->set("index", -1);
    m_followingTimer->set("running", false);
    invoke(m_followingTimer, "stop");
}

/*!
  Returns true is any unit is being followed.
  */
bool CcfQmlBaseScenario::isFollowingOn()
{
    return m_followingTimer->getBool("running");
}

/*!
  Invoked on every followTimer update.

  Checks whether a unit is moving, centers view on the unit etc. If a unit is
  stationary, timer is turned off.
  */
void CcfQmlBaseScenario::updateFollowingUnit()
{
    QObject *unit = m_units.at(m_followingTimer->getInt("index"));
    if (unit->getBool("moving") == true) {
        centerViewOnUnit(unit);
    } else {
        invoke(m_followingTimer, "stop");
    }
}

/*!
  Restarts followTimer when a unit is on the move.
  */
void CcfQmlBaseScenario::handleUnitMovement(bool isMoving, int unitIndex)
{
    if (m_followingTimer->getInt("index") != unitIndex) {
        return;
    }

    if (isMoving == true) {
        invoke(m_followingTimer, "start");
    } else if (isMoving == false) {
        invoke(m_followingTimer, "stop");
    }
}

/*!
  Triggered by effectsTimer, switches all effects' frames to the next one.
  */
void CcfQmlBaseScenario::updateEffects() {
    bool haveAllEffectsFinished = true;
    for (int i = 0; i < m_effects.length(); i++) {
        if (m_effects.at(i)->getBool("running") == true) {
            invoke(m_effects.at(i), "nextFrame");
            haveAllEffectsFinished = false;
        }
    }

    // Clear list if all effects have finished.
    // Crude, but might optimise the code a bit.
    if (haveAllEffectsFinished == true) {
        invoke(m_effectsTimer, "stop");

        for (int i = 0; i < m_effects.length(); i++) {
            QObject *effect = m_effects.at(i);
            delete effect;
        }

        m_effects.clear();
    }
}

/*!
  Updates aimLine (rotation, length, anchor points, obscuring etc.).

  This function is called by aimLineTimer on every update.
  */
void CcfQmlBaseScenario::updateAimLine()
{
    CcfQmlBaseUnit *unit = ccfUnit(m_units.at(m_aimLine->getInt("unitIndex")));

    if (unit == 0)
        return;

    if (m_aimLine->isVisible()) {
        qreal x1 = unit->x() + unit->getCenterX();
        qreal y1 = unit->y() + unit->getCenterY();
        qreal x2 = m_mouseAreaMain->getReal("mouseX");
        qreal y2 = m_mouseAreaMain->getReal("mouseY");

        m_aimLine->setX(x1);
        m_aimLine->setY(y1);

        qreal newRotation = CcfEngineHelpers::rotationAngle(x2, y2, x1, y1);

        if (m_aimLineRotation != newRotation) {
            setAimLineRotation(newRotation);
            m_aimLine->setHeight(CcfEngineHelpers::targetDistance(x1, y1, x2, y2));


            // If obscuring should be turned off for some actions (movement)
            // an if clause here would do the trick.
            qreal terrainObscure = mmain->terrain()->checkForTerrainInLOS(x1, y1, x2, y2, unit);
            qreal propsObscure = CcfEngineHelpers::checkForObstaclesInLOS(m_props,
                                                                          x1, y1, x2, y2,
                                                                          new QObject());

            qreal unitsObscure = 0.0;
            QObject *targetUnit = unitAt(x2, y2);

            int targetIndex = -1;
            if (targetUnit != 0) {
                targetIndex = targetUnit->getInt("unitIndex");
            } else {
                targetIndex = -1;
            }

            unitsObscure = CcfEngineHelpers::checkForObstaclesInLOS(getAllUnitsButOne(targetIndex),
                                                                    x1, y1, x2, y2, unit);

            // Conditions here should be redesigned to save time.
            // There is no need to update aimLine if a given Beginning
            // is further than current one.
            if (terrainObscure != 0) {
                if (terrainObscure < 0) {
                    m_aimLine->set("obscureBeginning", -terrainObscure);
                } else {
                    m_aimLine->set("invisibleBeginning", terrainObscure);
                }
            } else {
                m_aimLine->set("obscureBeginning", m_aimLine->height());
                m_aimLine->set("invisibleBeginning", m_aimLine->height());
            }

            if (propsObscure != 0) {
                if (propsObscure < 0) {
                    m_aimLine->set("obscureBeginning", -propsObscure);
                } else {
                    m_aimLine->set("invisibleBeginning", propsObscure);
                }
            }

            if (unitsObscure != 0) {
                if (unitsObscure < 0) {
                    m_aimLine->set("obscureBeginning", -unitsObscure);
                } else {
                    m_aimLine->set("invisibleBeginning", unitsObscure);
                }
            }
        }
    } else {
        qreal tempRotation = CcfEngineHelpers::rotationAngle(unit->x() + unit->getCenterX(),
                                                             unit->y() + unit->getCenterY(),
                                                             m_mouseAreaMain->getReal("mouseX"),
                                                             m_mouseAreaMain->getReal("mouseY"));
        unit->setDefenceSphereRotation(unit->rotation() + CcfEngineHelpers::angleTo8Step(tempRotation));
    }
}

/*!
  Returns a unit found under \a x and \a y. If no unit is found, 0 is returned.

  \sa unitIndexAt
  */
QObject *CcfQmlBaseScenario::unitAt(qreal x, qreal y)
{
    for (int i = 0; i < m_units.count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem *>(m_units.at(i));
        QPointF mapped = item->mapFromItem(m_zoomArea, QPointF(x, y));
        if (item->contains(mapped)) {
            return m_units.at(i);
        }
    }

    return 0;
}

/*!
  Similarly to unitAt, but it returns an index of the unit found under \a x and \a y.

  \sa unitAt
  */
int CcfQmlBaseScenario::unitIndexAt(qreal x, qreal y)
{
    for (int i = 0; i < m_units.count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem *>(m_units.at(i));
        QPointF mapped = item->mapFromItem(m_zoomArea, QPointF(x, y));
        if (item->contains(mapped)) {
            return i;
        }
    }

    return 1;
}

/*!
  Updates window's width.
  */
void CcfQmlBaseScenario::updateWidth()
{
    QObject *menu = child("menu");
    if (mmain->config()->getWindowWidth() < menu->getInt("contentWidth")) {
        menu->set("width", mmain->config()->getWindowWidth());
    } else {
        menu->set("width", menu->getInt("contentWidth"));
    }
}

/*!
  Changes the side the player is playing on.
  */
void CcfQmlBaseScenario::togglePlayer()
{
    QStringList sides;
    // Find all available sides.
    for (int i = 0; i < m_units.count(); ++i) {
        if (!sides.contains(m_units.at(i)->getString("unitSide"))) {
            sides.append(m_units.at(i)->getString("unitSide"));
        }
    }

    // Switch to next one in line.
    for (int i = 0; i < sides.length(); ++i) {
        if (sides.at(i) == mmain->scenarioState()->getPlayerSide()) {
            if (i != (sides.length() - 1)) {
                mmain->scenarioState()->setPlayerSide(sides.at(i + 1));
            } else {
                mmain->scenarioState()->setPlayerSide(sides.at(0));
            }

            mmain->global()->statusMessage("Player side changed to: " + mmain->scenarioState()->getPlayerSide());
            break;
        }
    }

    hideNonPlayerUnits();
    m_scenarioWinStatus = "no";
    checkScenarioFinished();
}

/*!
  Hides all units not belonging to current player (regardless of their visibility).
  */
void CcfQmlBaseScenario::hideNonPlayerUnits()
{
    for (int i = 0; i < m_units.count(); ++i) {
        if (m_units.at(i)->getString("unitSide") != mmain->scenarioState()->getPlayerSide())
            m_units.at(i)->set("visible", false);
    }
}

/*!
  Sets side marks on units.
  */
void CcfQmlBaseScenario::setSideMarks()
{
    for (int i = 0; i < m_units.count(); ++i) {
        m_units.at(i)->set("sideMarkSource", mmain->scenarioState()->getSidePath(m_units.at(i)->getString("unitSide")));
    }
}

/*!
  Checks whether scenario objectives are all met, and displays relevan info when
  it's needed.
  */
void CcfQmlBaseScenario::checkScenarioFinished()
{
    //// Experimental - unit destruction detection
    // It's probable that this should be done elsewhere.
    bool areAllEnemiesDestroyed = true;
    bool areAllAlliesDestroyed = true;
    for (int i = 0; i < m_units.count(); ++i) {
        QObject *currentUnit = m_units.at(i);
        if ((currentUnit->getString("unitSide") != mmain->scenarioState()->getPlayerSide()) && (currentUnit->getString("state") == "healthy")) {
            areAllEnemiesDestroyed = false;
        } else if ((currentUnit->getString("unitSide") == mmain->scenarioState()->getPlayerSide()) &&(currentUnit->getString("state") == "healthy")) {
            areAllAlliesDestroyed = false;
        }
    }

    if (m_scenarioWinStatus == "no") {
        if (areAllEnemiesDestroyed) {
            m_scenarioWinStatus = "won";
            mmain->global()->statusMessage("All enemies destroyed. You have won!");
        } else if (areAllAlliesDestroyed) {
            m_scenarioWinStatus = "lost";
            mmain->global()->statusMessage("All allies destroyed. You have lost!");
        }
    }
}

/*!
  Updates the rubber band (dimensions).
  */
void CcfQmlBaseScenario::updateRubberBand(qreal x, qreal y)
{
    if (qFuzzyCompare(x, 0.0))
        x = m_mouseAreaMain->getReal("mouseX");
    if (qFuzzyCompare(y, 0.0))
        y = m_mouseAreaMain->getReal("mouseY");

    qreal rubberX = 0, rubberY = 0, rubberX2 = 0, rubberY2 = 0; // 2 edges of the rubber band,
    // in root's coordinates.
    QQuickItem *rubberBand = item("rubberBand");
    //    int rbRotation = rubberBand->getInt("rubberBandRotation");

    // Adjusting rubber band's shape:
    if ((x > rubberBand->x()) && (y > rubberBand->y())) {
        // Bottom-right quarter
        rubberBand->set("rubberBandRotation", 0);
        rubberBand->setWidth(x - rubberBand->x());
        rubberBand->setHeight(y - rubberBand->y());

        rubberX = rubberBand->x();
        rubberY = rubberBand->y();
        rubberX2 = rubberBand->x() + rubberBand->width();
        rubberY2 = rubberBand->y() + rubberBand->height();
    } else if ((x > rubberBand->x()) && (y < rubberBand->y())) {
        // Top-right quarter
        rubberBand->set("rubberBandRotation", 270);
        rubberBand->setWidth(rubberBand->y() - y);
        rubberBand->setHeight(x - rubberBand->x());

        rubberX = rubberBand->x();
        rubberY = rubberBand->y() - rubberBand->width();
        rubberX2 = rubberBand->x() + rubberBand->height();
        rubberY2 = rubberBand->y();
    } else if ((x < rubberBand->x()) && (y > rubberBand->y())) {
        // Bottom-left quarter
        rubberBand->set("rubberBandRotation", 90);
        rubberBand->setWidth(y - rubberBand->y());
        rubberBand->setHeight(rubberBand->x() - x);

        rubberX = rubberBand->x() - rubberBand->height();
        rubberY = rubberBand->y();
        rubberX2 = rubberBand->x();
        rubberY2 = rubberBand->y() + rubberBand->width();
    } else if ((x < rubberBand->x()) && (y < rubberBand->y())) {
        // Top-left quarter
        rubberBand->set("rubberBandRotation", 180);
        rubberBand->setWidth(rubberBand->x() - x);
        rubberBand->setHeight(rubberBand->y() - y);

        rubberX = rubberBand->x() - rubberBand->width();
        rubberY = rubberBand->y() - rubberBand->height();
        rubberX2 = rubberBand->x();
        rubberY2 = rubberBand->y();
    } else if ((x == rubberBand->x()) || (y == rubberBand->y())) {
        rubberBand->set("height", 2);
        rubberBand->set("width", 2);

        rubberX = rubberBand->x();
        rubberY = rubberBand->y();
        rubberX2 = rubberBand->x();
        rubberY2 = rubberBand->y();
    }

    //    test1.x = rubberX;
    //    test1.y = rubberY;
    //    test2.x = rubberX2;
    //    test2.y = rubberY2;

    // Selecting units:
    for (int i = 0; i < m_units.count(); ++i) {
        CcfQmlBaseUnit *unit = ccfUnit(m_units.at(i));

        if (unit->getSelected() == true)
            continue;

        // TODO: rewrite using QML Item::contains
        if ((unit->x() <= rubberX2) && (unit->x() >= rubberX)
                && (unit->y() <= rubberY2) && (unit->y() >= rubberY)) {
            selectUnit(unit->getUnitIndex(), Qt::ControlModifier);
        }
    }
}

/*!
  Selects a single unit, based on some rules (it cannot be destroyed or belong
  to the enemy).
  */
void CcfQmlBaseScenario::selectUnit(int index, int modifier) //Qt::KeyboardModifier
{
    CcfQmlBaseUnit *unit = ccfUnit(m_units.at(index));
    if ((unit->getUnitSide() != mmain->scenarioState()->getPlayerSide())
            || (unit->state() != "healthy")) {
        return;
    }

    if ((modifier == Qt::NoModifier) && (mmain->config()->getUiMode() == "DESKTOP")) {
        deselectAllUnits();
        unit->setSelected(true);
        invoke(child("soldierMenu"), "populateSoldiers", Q_ARG(QVariant, QVariant(unit->soldiers())));
    } else if ((modifier == Qt::ControlModifier) || (mmain->config()->getUiMode() == "MOBILE")) {
        int selected = selectedUnits().length();
        if (unit->getSelected() == true)
            unit->setSelected(false);
        else if (unit->getSelected() == false)
            unit->setSelected(true);

        if (selected > 1) {
            invoke(child("soldierMenu"), "clear");
        } else if (selected == 1) {
            // TODO: figure out why this "if" is here.
        }
    }
}

/*!
  Deselects a given unit.
  */
void CcfQmlBaseScenario::deselectUnit(int index)
{
    m_units.at(index)->set("selected", false);
}

/*!
  Deselects all units.
  */
void CcfQmlBaseScenario::deselectAllUnits()
{
    invoke(child("soldierMenu"), "clear");
    for (int i = 0; i < m_units.count(); ++i) {
        deselectUnit(i);
    }
}

/*!
  Selects a unit from gameArea (as opposed to roster).
  */
void CcfQmlBaseScenario::selectUnitFromGameArea(QObject *mouse)
{
    CcfQmlBaseUnit *unit = ccfUnit(unitAt(mouse->getReal("x"), mouse->getReal("y")));

    if (unit == 0) {
        deselectAllUnits();
        return;
    }

    selectUnit(unit->getUnitIndex(), mouse->getInt("modifiers"));
}

/*!
  Selects a unit from roster menu.

  TODO: fix, as it does not work currently.
  */
void CcfQmlBaseScenario::selectUnitFromRoster(QObject *mouse)
{
    CcfQmlBaseUnit *unit = NULL;

    unit = m_roster->getUnitAt(mouse->getReal("x"), mouse->getReal("y"));

    if (unit != NULL) {
        selectUnit(unit->getUnitIndex(), mouse->getInt("modifiers"));
    }
}

/*!
  Correctly positions the context menu (so that it does not go beyond the screen).
  */
void CcfQmlBaseScenario::setContextMenuPosition(QQuickItem *menu, qreal x, qreal y)
{
    if ((x + menu->width()) > width())
        menu->setX(width() - menu->width());
    else
        menu->setX(x);

    if ((y + menu->height()) > height())
        menu->setY(height() - menu->height());
    else
        menu->setY(y);
}

/*!
  Handles digit presses, returns a number corresponding to digit that was pressed.
  */
int CcfQmlBaseScenario::digitPressed(QObject *event)
{
    int result = -1;
    int key = event->getInt("key");

    if (key == Qt::Key_1)
        result = 1;
    else if (key == Qt::Key_2)
        result = 2;
    else if (key == Qt::Key_3)
        result = 3;
    else if (key == Qt::Key_4)
        result = 4;
    else if (key == Qt::Key_5)
        result = 5;
    else if (key == Qt::Key_6)
        result = 6;
    else if (key == Qt::Key_7)
        result = 7;
    else if (key == Qt::Key_8)
        result = 8;
    else if (key == Qt::Key_9)
        result = 9;
    else if (key == Qt::Key_0)
        result = 10;

    return result;
}

/*!
  Updates visibility of opposing units.
  */
void CcfQmlBaseScenario::updateUnitVisibility()
{
    QObjectList friendlyUnitsList = playerUnits(mmain->scenarioState()->getPlayerSide());
    QObjectList enemyUnitsList = enemyUnits(mmain->scenarioState()->getPlayerSide());

    for (int i = 0; i < friendlyUnitsList.length(); ++i) {
        for (int j = 0; j < enemyUnitsList.length(); ++j) {
            CcfQmlBaseUnit *enemy = ccfUnit(enemyUnitsList.at(j));

            if (enemy->isVisible() == true)
                continue;

            CcfQmlBaseUnit *friendly = ccfUnit(friendlyUnitsList.at(i));

            qreal x1 = friendly->x() + friendly->getCenterX();
            qreal y1 = friendly->y() + friendly->getCenterY();
            qreal x2 = enemy->x() + enemy->getCenterX();
            qreal y2 = enemy->y() + enemy->getCenterY();

            if (mmain->terrain()->isTargetVisible(x1, y1, x2, y2)
                    && !CcfEngineHelpers::isObstacleInLOS(m_props,
                                                          x1, y1, x2, y2,
                                                          friendly)) {
                mmain->logger()->log("Now visible: " + enemy->objectName()
                                     + ", spotted by: " + friendly->objectName());
                enemy->setVisible(true);
            }
        }
    }
}

/*!
  Handles left mouse click.
  */
void CcfQmlBaseScenario::handleLeftMouseClick(QObject *mouse)
{
    if ((!m_contextMenu->isVisible()) && (m_aimLine->isVisible())) {
        if (mouse->getInt("modifiers") == Qt::ShiftModifier) {
            placeWaypoint(m_aimLine->getInt("unitIndex"),
                          m_mouseAreaMain->getReal("mouseX"),
                          m_mouseAreaMain->getReal("mouseY"));
        } else {
            performContextAction(m_aimLine->getInt("unitIndex"),
                                 m_mouseAreaMain->getReal("mouseX"),
                                 m_mouseAreaMain->getReal("mouseY"));
        }
    } else {
        cleanContextAction();
        selectUnitFromGameArea(mouse);
    }
}

/*!
  Handles right mouse click.
  */
void CcfQmlBaseScenario::handleRightMouseClick(QObject *mouse)
{
    cleanContextAction();
    if (m_aimLine->isVisible() == true)
        cancelAllSelectedOrders();

    QObject *unitObject = NULL;
    unitObject = unitAt(m_mouseAreaMain->getReal("mouseX"), m_mouseAreaMain->getReal("mouseY"));

    if ((unitObject == m_mouseAreaMain) || (unitObject == NULL)
            || (unitObject->getString("unitSide") != mmain->scenarioState()->getPlayerSide())) {
        deselectAllUnits();
        return;
    }

    CcfQmlBaseUnit *unit = ccfUnit(unitObject);

    if (!unit->getSelected()) {
        selectUnitFromGameArea(mouse);
    }

    // Fixes context menu at the centre of unit object.
    QPointF mappedCoords = mapFromItem(m_gameArea,
                                       QPointF((unit->x() + unit->getCenterX()) * m_zoom,
                                               (unit->y() + unit->getCenterY()) * m_zoom));

    setContextMenuPosition(m_contextMenu,
                           mappedCoords.x() - (m_gameArea->getReal("contentX")),
                           mappedCoords.y() - (m_gameArea->getReal("contentY")));

    m_contextMenu->setVisible(true);
    //    m_contextMenu->set("unitIndex", unit->getUnitIndex());
    m_aimLine->set("unitIndex", unit->getUnitIndex());
}

/*!
  Handles left mouse click on roster menu.
  */
void CcfQmlBaseScenario::handleLeftMouseClickRoster(QObject *mouse)
{
    if (!m_contextMenu->isVisible()) {
        cleanContextAction();
        selectUnitFromRoster(mouse);
    }
}

/*!
  Handles right mouse click on roster menu.
  */
void CcfQmlBaseScenario::handleRightMouseClickRoster(QObject *mouse)
{
    cleanContextAction();

    qreal mousex = mouse->getReal("x");
    qreal mousey = mouse->getReal("y");

    CcfQmlBaseUnit *unit = m_roster->getUnitAt(mousex, mousey);

    if (unit != 0) {
        if (!unit->getSelected()) {
            selectUnitFromRoster(mouse);
        }

        QPointF rosterUnitCoords = m_roster->childCenterCoords(mousex, mousey);
        // Fixes context menu at the centre of unit object.
        setContextMenuPosition(m_contextMenu,
                               child("menu")->getReal("x") + rosterUnitCoords.x(),
                               height() + child("menu")->getReal("y") + rosterUnitCoords.y());

        // Displays the context menu. This is suboptimal.
        m_contextMenu->setVisible(true);
    }
}

/*!
  Handles "press and hold" action of left mouse button.
  */
void CcfQmlBaseScenario::handlePressAndHoldLeft(QObject *mouse)
{
    QQuickItem *rubberBand = item("rubberBand");
    rubberBand->setX(mouse->getReal("x"));
    rubberBand->setY(mouse->getReal("y"));
    rubberBand->setHeight(2);
    rubberBand->setWidth(2);
    rubberBand->setVisible(true);

    if (mouse->getInt("modifiers") == Qt::NoModifier)
        deselectAllUnits();

    QObject *rubberBandTimer = child("rubberBandTimer");
    if (rubberBandTimer->getBool("running") == false)
        invoke(rubberBandTimer, "start");
}

/*!
  Handles "press and hold" action of right mouse button.
  */
void CcfQmlBaseScenario::handlePressAndHoldRight(QObject *mouse)
{
    // TODO: transform Map.qml into C++ and simplify this code!
    //    child("map")->property("mapItem").value<QObject *>()->children();
    QObject *mapItem = child("map")->property("mapItem").value<QObject *>();
    QString infoString;

    metaObject()->invokeMethod(mapItem, "terrainInfoString", Q_RETURN_ARG(QString, infoString),
                               Q_ARG(qreal, mouse->getReal("x")),
                               Q_ARG(qreal, mouse->getReal("y")));

    //    map.item.terrainInfoString(mouse.x, mouse.y);
    child("terrainInfoText")->set("text", infoString);
}

/*!
  Invoked when mouse button is released.
  */
void CcfQmlBaseScenario::handleMouseReleased() {
    QObject *rubberBandTimer = child("rubberBandTimer");
    QObject *terrainInfoText = item("terrainInfoText");
    if (rubberBandTimer->getBool("running") == true) {
        invoke(rubberBandTimer, "stop");
        QQuickItem *rubberBand = item("rubberBand");
        rubberBand->setVisible(false);
        rubberBand->setHeight(2);
        rubberBand->setWidth(2);
    } else if (terrainInfoText->getString("text") != "") {
        terrainInfoText->set("text", "");
    }
}

/*!
  Handles key press.

  Uses C++ bindings heavily to check keyboard shortcuts in CcfConfig.
  */
void CcfQmlBaseScenario::handleKeyPress(QObject *event)
{
    int digit = digitPressed(event);
    int modifiers = event->getInt("modifiers");
    int key = event->getInt("key");
    CcfConfig *config = mmain->config();

    if (modifiers == Qt::ControlModifier) {
        if (digit != -1)
            groupUnits(digit);
    } else {
        // Development key bindings.
        if (key == Qt::Key_BracketRight) {
            QObject *mapItem = child("map")->property("mapItem").value<QObject *>();
            mapItem->set("hipsometricMapInFront", !mapItem->getBool("hipsometricMapInFront"));
        } else if (key == Qt::Key_BracketLeft) {
            togglePlayer();
        } else
            // end of dev key bindings
            if (key == config->keyForFunction("zoom in")) {
                zoomIn();
            } else if (key == config->keyForFunction("zoom out")) {
                zoomOut();
            } else if (key == config->keyForFunction("quit")) {
                mmain->quit();
            } else if (key == config->keyForFunction("toggle top menu")) {
                invoke(child("topMenu"), "toggleMenu");
            } else if (key == config->keyForFunction("toggle bottom menu")) {
                invoke(child("bottomMenu"), "toggleMenu");
            } else if (key == config->keyForFunction("follow")) {
                QObjectList selectedOnes = selectedUnits();
                if ((m_followingTimer->getBool("running") == false) && (selectedOnes.length() > 0)) {
                    //                    unitIndex = selectedOnes[0].unitIndex;
                    startFollowingUnit(selectedOnes.at(0)->getInt("unitIndex"));
                } else if (m_followingTimer->getBool("running") == true) {
                    if (selectedOnes.length() > 0) {
                        int unitIndex = selectedOnes.at(0)->getInt("unitIndex");

                        if (m_followingTimer->getInt("index") == unitIndex) {
                            stopFollowingUnit();
                        } else {
                            startFollowingUnit(unitIndex);
                        }
                    } else {
                        stopFollowingUnit();
                    }
                } else {
                    mmain->logger()->log("No unit selected to follow.");
                }
            } else if (selectedUnitsCount() > 0) {
                if (key == config->keyForFunction("Stop")) {
                    QObjectList selectedOnes = selectedUnits();
                    for (int i = 0; i < selectedOnes.length(); ++i) {
                        invoke(selectedOnes.at(i), "cancelOrder");
                    }
                } else if (key == config->keyForFunction("Move fast")) {
                    scheduleContextAction("Move fast");
                } else if (key == config->keyForFunction("Move")) {
                    scheduleContextAction("Move");
                } else if (key == config->keyForFunction("Sneak")) {
                    scheduleContextAction("Sneak");
                } else if (key == config->keyForFunction("Attack")) {
                    scheduleContextAction("Attack");
                } else if (key == config->keyForFunction("Smoke")) {
                    scheduleContextAction("Smoke");
                } else if (key == config->keyForFunction("Defend")) {
                    scheduleContextAction("Defend");
                } else if (key == config->keyForFunction("Ambush")) {
                    scheduleContextAction("Ambush");
                }
            }

        if (key == config->keyForFunction("pause")) {
            togglePause();
        }

        // Digit reading.
        if (digit != -1)
            selectGroup(digit);
    }
}

/*!
  Handles key press.

  Uses C++ bindings heavily to check keyboard shortcuts in CcfConfig.
  */
void CcfQmlBaseScenario::handleWheelEventMouseAreaMain(QObject *wheel)
{
    QPointF delta = wheel->property("angleDelta").toPointF();

    if (wheel->getInt("modifiers") == Qt::ControlModifier) {
        m_zoom += delta.y()/800;
        emit zoomChanged();
    } else if (wheel->getInt("modifiers") == Qt::ShiftModifier) {
        int magnitude = 0;
        if (delta.x() != 0)
            magnitude = delta.x();
        else
            magnitude = delta.y();

        invoke(m_gameArea, "flick", Q_ARG(qreal, magnitude * 5), Q_ARG(qreal, 1.0));
    } else {
        wheel->set("accepted", false);
    }
}

// Property getters:
/*!
  Returns the scenario file path.
  */
QString CcfQmlBaseScenario::getScenarioFile() const
{
    return m_scenarioFile;
}

/*!
  Returns the scenario win status.
  */
QString CcfQmlBaseScenario::getScenarioWinStatus() const
{
    return m_scenarioWinStatus;
}

/*!
  Returns aim line rotation in degrees.
  */
int CcfQmlBaseScenario::getAimLineRotation() const
{
    return m_aimLineRotation;
}

/*!
  Returns true if this is a campaign.
  */
bool CcfQmlBaseScenario::getIsCampaign() const
{
    return m_isCampaign;
}

/*!
  Returns the path to map file.
  */
QString CcfQmlBaseScenario::getMapFile() const
{
    return m_mapFile;
}

/*!
  Returns the zoom level.
  */
qreal CcfQmlBaseScenario::getZoom() const
{
    return m_zoom;
}

/*!
  Returns the central point for zooming.
  */
QPoint CcfQmlBaseScenario::getZoomPoint() const
{
    return m_zoomPoint;
}

/*!
  Returns true if the game is paused.
  */
bool CcfQmlBaseScenario::getPaused() const
{
    return m_paused;
}

/*!
  Returns the current menu's background colour.
  */
QColor CcfQmlBaseScenario::getMenuBackgroundColor() const
{
    return m_menuBackgroundColor;
}

/*!
  Returns a list of all units in this scenario.
  */
QObjectList CcfQmlBaseScenario::getUnits() const
{
    return m_units;
}

// Property setters:
/*!
  Sets the \a scenarioFile, initialises the whole scenario object runtime.
  */
void CcfQmlBaseScenario::setScenarioFile(const QString &scenarioFile)
{
    bool wasChaged = false;
    if (scenarioFile != m_scenarioFile)
        wasChaged = true;

    m_scenarioFile = scenarioFile;

    if (wasChaged)
        emit scenarioFileChanged();

    if (scenarioFile != "") {
        // If it's desktop, menus should be unrolled:
        if (mmain->config()->getUiMode() == "DESKTOP") {
            invoke(child("topMenu"), "openMenu");
            // TODO: turn back on for release. It's off now because it's annoying ;)
            //                bottomMenu.openMenu();
        }

        connect(mmain->config(), &CcfConfig::windowWidthChanged, this, &CcfQmlBaseScenario::updateWidth);
        updateWidth();
        connect(child("topMenu"), SIGNAL(save()), this, SLOT(saveGameToFile()));
        connect(child("loadGame"), SIGNAL(gameEntryClicked(const QString &)),
                this, SIGNAL(loadScenario(const QString &)));
    }
}

/*!
  Sets the \a scenarioWinStatus.
  */
void CcfQmlBaseScenario::setScenarioWinStatus(const QString &scenarioWinStatus)
{
    bool wasChaged = false;
    if (scenarioWinStatus != m_scenarioWinStatus)
        wasChaged = true;

    m_scenarioWinStatus = scenarioWinStatus;

    if (wasChaged)
        emit scenarioWinStatusChanged();
}

/*!
  Sets \a aimLineRotation.
  */
void CcfQmlBaseScenario::setAimLineRotation(int aimLineRotation)
{
    bool wasChaged = false;
    if (aimLineRotation != m_aimLineRotation)
        wasChaged = true;

    m_aimLineRotation = aimLineRotation;

    if (wasChaged)
        emit aimLineRotationChanged();
}

/*!
  Determies whether this scenario is part of a campaign with \a isCampaign.
  */
void CcfQmlBaseScenario::setIsCampaign(bool isCampaign)
{
    bool wasChaged = false;
    if (isCampaign != m_isCampaign)
        wasChaged = true;

    m_isCampaign = isCampaign;

    if (wasChaged)
        emit isCampaignChanged();
}

/*!
  Sets scenario's \a mapFile.
  */
void CcfQmlBaseScenario::setMapFile(const QString &mapFile)
{
    bool wasChaged = false;
    if (mapFile != m_mapFile)
        wasChaged = true;

    m_mapFile = mapFile;

    if (wasChaged)
        emit mapFileChanged();
}

/*!
  Sets map's \a zoom level.
  */
void CcfQmlBaseScenario::setZoom(qreal zoom)
{
    bool wasChaged = false;
    if (zoom != m_zoom)
        wasChaged = true;

    m_zoom = zoom;

    if (wasChaged)
        emit zoomChanged();
}

/*!
  Sets the point (\a zoomPoint) which is the central point for zooming.
  */
void CcfQmlBaseScenario::setZoomPoint(const QPoint &zoomPoint)
{
    bool wasChaged = false;
    if (zoomPoint != m_zoomPoint)
        wasChaged = true;

    m_zoomPoint = zoomPoint;

    if (wasChaged)
        emit zoomPointChanged();
}

/*!
  Sets the game into \a paused or unpaused state.
  */
void CcfQmlBaseScenario::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != m_paused)
        wasChaged = true;

    m_paused = paused;

    if (wasChaged)
        emit pausedChanged();
}

/*!
  Sets menu background colour (\a menuBackgroundColor).
  */
void CcfQmlBaseScenario::setMenuBackgroundColor(const QColor &menuBackgroundColor)
{
    bool wasChaged = false;
    if (menuBackgroundColor != m_menuBackgroundColor)
        wasChaged = true;

    m_menuBackgroundColor = menuBackgroundColor;

    if (wasChaged)
        emit menuBackgroundColorChanged();
}

/*!
  Sets the \a units taking part in this scenario.
  */
void CcfQmlBaseScenario::setUnits(QObjectList units)
{
    m_units = units;
    emit unitsChanged();
}
