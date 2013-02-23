#include "ccfqmlbasescenario.h"
#include "ccfmain.h"
#include "config/ccfconfig.h"
#include "qmlBase/ccfqmlbaseunit.h"
#include "qmlBase/ccfqmlbaserostermenu.h"
#include "logic/ccfscenariostate.h"
#include "ccfterrain.h"

#include <QVariant>
#include <QPointF>
#include <QQmlComponent>
#include <QQmlListProperty>
#include <QQuickItem>

/*!
  Standard constructor. Initialises properties to default values.
  */
CcfQmlBaseScenario::CcfQmlBaseScenario(QQuickItem *parent) : CcfObjectBase(parent)
{
    mZoom = 1.0;
    mPaused = false;
    mMenuBackgroundColor = QColor("#7e8c24");
    mAimLineRotation = 0;
    mIsCampaign = false;

    mMainInstance = CcfMain::instance();
    mScenarioState = new CcfScenarioState(this);
    mTerrain = new CcfTerrain(this);

    mEffectsComponent = new QQmlComponent(mMainInstance->engine(),
                                          QUrl::fromLocalFile("qml/effects/Effect.qml"));

    connect(this, &CcfQmlBaseScenario::togglePause, this, &CcfQmlBaseScenario::onTogglePause);
}

/*!
  Sets internal pointers to frequently used QML items, thus reducing the overhead
  of repeated searching for them at runtime.
  */
void CcfQmlBaseScenario::init()
{
    if (mScenarioFile != "") {
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
                    mUnits.append(unit);
                }
            }

            mapItem = map->property("mapItem").value<QObject *>();

            if(mapItem != 0)
                invoke(mapItem, "setUnits", Q_ARG(QVariant, QVariant::fromValue(mUnits) ));
            else
                mlogger->error("MapItem object was not properly initialised",
                               "Robin Hood is a jerk");
        } else {
            // This is a campaign
            // TODO: add some clever code here ;)
        }

        mScenarioState->setAvailableSides(unitSideList);

        mAimLine = item("aimLine");
        mGameArea = item("gameArea");
        mZoomArea = item("zoomArea");
        mContextMenu = item("contextMenu");
        mEffectsTimer = child("effectsTimer");
        mRotationTimer = child("rotationTimer");
        mFollowingTimer = child("followingTimer");
        mMouseAreaMain = child("mouseAreaMain");

        QObjectList mapItemKids = mapItem->children();
        for (int i = 0; i < mapItemKids.length() - 2; ++i) {
            mProps.append(mapItemKids.at(i + 2));
        }

        mRoster = findChild<CcfQmlBaseRosterMenu *>("roster");
        mRoster->populateUnits(mUnits);

        connect(mContextMenu, SIGNAL(menuEntryClicked(QString)), this, SLOT(scheduleContextAction(QString)));
        connect(mRotationTimer, SIGNAL(triggered()), this, SLOT(updateAimLine()));
        connect(mEffectsTimer, SIGNAL(triggered()), this, SLOT(updateEffects()));
        connect(mFollowingTimer, SIGNAL(triggered()), this, SLOT(updateFollowingUnit()));
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
    mZoom += 0.1;
}

/*!
  Zooms the game area out by 10%.
  */
void CcfQmlBaseScenario::zoomOut()
{
    mZoom -= 0.1;
}

/*!
  Returns a QObjectList of units that are on the side of \a player.
  */
QObjectList CcfQmlBaseScenario::playerUnits(const QString &player)
{
    QList<QObject *> unitsArray;
    for (int i = 0; i < mUnits.count(); ++i) {
        if (mUnits.at(i)->getString("unitSide") == player) {
            unitsArray.append(mUnits.at(i));
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
    for (int i = 0; i < mUnits.count(); ++i) {
        if (mUnits.at(i)->getString("unitSide") != player) {
            unitsArray.append(mUnits.at(i));
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
    if (groupNumber < mUnitGroups.count()) {
        for (int i = 0; i < mUnitGroups.value(groupNumber).length(); ++i) {
            if (mUnitGroups.value(groupNumber).at(i)->getBool("selected") == false)
                mUnitGroups.value(groupNumber).at(i)->set("groupNumber", 0);
        }
    }

    QObjectList group = selectedUnits();
    mUnitGroups.insert(groupNumber, group);

    for (int i = 0; i < group.length(); ++i) {
        group.at(i)->set("groupNumber", groupNumber);
    }

    //    console.log("Group " + groupNumber + " created.");
    mlogger->statusMessage(tr("Group %1 created.").arg(groupNumber));
}

/*!
  Selects a whole unit group.

  It has to be created first, of course.
  */
void CcfQmlBaseScenario::selectGroup(int groupNumber)
{
    if (groupNumber < 0)
        return;

    QObjectList group = mUnitGroups.value(groupNumber);
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
        if (mAimLine->getReal("invisibleBeginning") < mAimLine->height()) {
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
    CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(index));
    QString scheduledOperation = mAimLine->getString("scheduledOperation");

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

            qreal tempX = targetX + (unit->x() - mUnits.at(index)->getReal("x"));
            qreal tempY = targetY + (unit->y() - mUnits.at(index)->getReal("y"));

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
    CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(index));
    QString scheduledOperation = mAimLine->getString("scheduledOperation");

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

            qreal tempX = targetX + (unit->x() - mUnits.at(index)->getReal("x"));
            qreal tempY = targetY + (unit->y() - mUnits.at(index)->getReal("y"));

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

    unit->queueOrder(operation, x, y, mZoomArea);
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
        unit->moveTo(x, y, mZoomArea);
    } else if (operation == "Move fast") {
        unit->moveFastTo(x, y, mZoomArea);
    } else if (operation == "Sneak") {
        unit->sneakTo(x, y, mZoomArea);
    } else if (operation == "Attack") {
        unit->fireTo(x, y, mZoomArea);
    } else if (operation == "Smoke") {
        unit->smokeTo(x, y, mZoomArea);
    }
}

/*!
  Slot invoked when an unit finishes an action.
  */
void CcfQmlBaseScenario::actionFinished(int index, qreal targetX, qreal targetY)
{
    CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(index));

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

    mAimLine->set("unitIndex", units.at(0)->property("unitIndex"));
    mAimLine->set("scheduledOperation", operation);
    mContextMenu->setVisible(false);

    if (operation == "Stop") {
        // Iterate over every unit!
        for (int i = 0; i < units.length(); ++i) {
            units.at(i)->cancelOrder();
        }
        cleanContextAction();
    } else if (operation == "Follow") {
        if (mFollowingTimer->getBool("running"))
            stopFollowingUnit();
        else
            startFollowingUnit(units.at(0)->getInt("unitIndex"));
    } else {
        // Draw aim line for all move/attack operations.
        if ((operation != "Ambush") && (operation != "Defend")) {
            mAimLine->setX(units.at(0)->x() + units.at(0)->getCenterX());
            mAimLine->setY(units.at(0)->y() + units.at(0)->getCenterY());

            mAimLine->set("color", CcfEngineHelpers::colorForOrder(operation));
            invoke(mRotationTimer, "start");
            mAimLine->setVisible(true);
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
            invoke(mRotationTimer, "start");
        }
    }
}

/*!
  Toggles the in-game pause.
  */
void CcfQmlBaseScenario::onTogglePause()
{
    if (mPaused == true) {
        mPaused = false;
    } else {
        mPaused = true;
    }

    emit pausedChanged();
}

/*!
  Creates an instance of Effect.qml component.
  */
QObject *CcfQmlBaseScenario::createEffect(QObject *parent)
{
    if (mEffectsComponent->isReady()) {
        QObject *object = mEffectsComponent->create();
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
    QObject *unit = mUnits.at(index);

    if (unit->getInt("currentOrder") != -1) {
        // TODO: remove use of effect for smoke and attack - and optimise it away.
        QObject *effect = createEffect(mZoomArea);
        if (effect == 0)
            return;

        mEffects.append(effect);
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

        invoke(mEffectsTimer, "start");
    }
}

/*!
  Cleans context action (hides context menu and aimLine, etc.).
  */
void CcfQmlBaseScenario::cleanContextAction()
{
    invoke(mRotationTimer, "stop");
    mAimLine->setVisible(false);
    mAimLine->setHeight(5.0);
    mContextMenu->setVisible(false);
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
    for (int i = 0; i < mUnits.count(); ++i) {
        if (mUnits.at(i)->getBool("selected") == true)
            result.append(mUnits.at(i));
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

    for (int i = 0; i < mUnits.count(); ++i) {
        if (mUnits.at(i)->getInt("unitIndex") != unitToOmit) {
            result.append(mUnits.at(i));
        }
    }

    return result;
}

/*!
  Centers view on a given unit.
  */
void CcfQmlBaseScenario::centerViewOnUnit(QObject *unit)
{
    mGameArea->set("contentX", (((unit->getReal("x") + unit->getReal("centerX")) * mZoom)
                                - mGameArea->width()/2));
    mGameArea->set("contentY", (((unit->getReal("y") + unit->getReal("centerY")) * mZoom)
                                - mGameArea->height()/2));
}

/*!
  Centers view on given coordinates.
  */
void CcfQmlBaseScenario::centerViewOn(qreal x, qreal y)
{
    mGameArea->set("contentX", (x - mGameArea->width()/2));
    mGameArea->set("contentY", (y - mGameArea->height()/2));
}

/*!
  Starts following a unit specified by it's index.
  */
void CcfQmlBaseScenario::startFollowingUnit(int index)
{
    mFollowingTimer->set("index", index);
    mFollowingTimer->set("running", true);
    centerViewOnUnit(mUnits.at(index));
    child("followingInfoBox")->set("bodyText", QString("Unit name: " + mUnits.at(index)->getString("unitType")
                                                       + "\nDouble click to stop."));
    invoke(mFollowingTimer, "start");
}

/*!
  Stops following a unit.
  */
void CcfQmlBaseScenario::stopFollowingUnit()
{
    mFollowingTimer->set("index", -1);
    mFollowingTimer->set("running", false);
    invoke(mFollowingTimer, "stop");
}

/*!
  Returns true is any unit is being followed.
  */
bool CcfQmlBaseScenario::isFollowingOn()
{
    return mFollowingTimer->getBool("running");
}

/*!
  Invoked on every followTimer update.

  Checks whether a unit is moving, centers view on the unit etc. If a unit is
  stationary, timer is turned off.
  */
void CcfQmlBaseScenario::updateFollowingUnit()
{
    QObject *unit = mUnits.at(mFollowingTimer->getInt("index"));
    if (unit->getBool("moving") == true) {
        centerViewOnUnit(unit);
    } else {
        invoke(mFollowingTimer, "stop");
    }
}

/*!
  Restarts followTimer when a unit is on the move.
  */
void CcfQmlBaseScenario::handleUnitMovement(bool isMoving, int unitIndex)
{
    if (mFollowingTimer->getInt("index") != unitIndex) {
        return;
    }

    if (isMoving == true) {
        invoke(mFollowingTimer, "start");
    } else if (isMoving == false) {
        invoke(mFollowingTimer, "stop");
    }
}

/*!
  Triggered by effectsTimer, switches all effects' frames to the next one.
  */
void CcfQmlBaseScenario::updateEffects() {
    bool haveAllEffectsFinished = true;
    for (int i = 0; i < mEffects.length(); i++) {
        if (mEffects.at(i)->getBool("running") == true) {
            invoke(mEffects.at(i), "nextFrame");
            haveAllEffectsFinished = false;
        }
    }

    // Clear list if all effects have finished.
    // Crude, but might optimise the code a bit.
    if (haveAllEffectsFinished == true) {
        invoke(mEffectsTimer, "stop");

        for (int i = 0; i < mEffects.length(); i++) {
            QObject *effect = mEffects.at(i);
            delete effect;
        }

        mEffects.clear();
    }
}

/*!
  Updates aimLine (rotation, length, anchor points, obscuring etc.).

  This function is called by aimLineTimer on every update.
  */
void CcfQmlBaseScenario::updateAimLine()
{
    CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(mAimLine->getInt("unitIndex")));

    if (unit == 0)
        return;

    if (mAimLine->isVisible()) {
        qreal x1 = unit->x() + unit->getCenterX();
        qreal y1 = unit->y() + unit->getCenterY();
        qreal x2 = mMouseAreaMain->getReal("mouseX");
        qreal y2 = mMouseAreaMain->getReal("mouseY");

        mAimLine->setX(x1);
        mAimLine->setY(y1);

        qreal newRotation = CcfEngineHelpers::rotationAngle(x2, y2, x1, y1);

        if (mAimLineRotation != newRotation) {
            setAimLineRotation(newRotation);
            mAimLine->setHeight(CcfEngineHelpers::targetDistance(x1, y1, x2, y2));


            // If obscuring should be turned off for some actions (movement)
            // an if clause here would do the trick.
            qreal terrainObscure = mTerrain->checkForTerrainInLOS(x1, y1, x2, y2, unit);
            qreal propsObscure = CcfEngineHelpers::checkForObstaclesInLOS(mProps,
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
                    mAimLine->set("obscureBeginning", -terrainObscure);
                } else {
                    mAimLine->set("invisibleBeginning", terrainObscure);
                }
            } else {
                mAimLine->set("obscureBeginning", mAimLine->height());
                mAimLine->set("invisibleBeginning", mAimLine->height());
            }

            if (propsObscure != 0) {
                if (propsObscure < 0) {
                    mAimLine->set("obscureBeginning", -propsObscure);
                } else {
                    mAimLine->set("invisibleBeginning", propsObscure);
                }
            }

            if (unitsObscure != 0) {
                if (unitsObscure < 0) {
                    mAimLine->set("obscureBeginning", -unitsObscure);
                } else {
                    mAimLine->set("invisibleBeginning", unitsObscure);
                }
            }
        }
    } else {
        qreal tempRotation = CcfEngineHelpers::rotationAngle(unit->x() + unit->getCenterX(),
                                                             unit->y() + unit->getCenterY(),
                                                             mMouseAreaMain->getReal("mouseX"),
                                                             mMouseAreaMain->getReal("mouseY"));
        unit->setDefenceSphereRotation(unit->rotation() + CcfEngineHelpers::angleTo8Step(tempRotation));
    }
}

/*!
  Returns a unit found under \a x and \a y. If no unit is found, 0 is returned.

  \sa unitIndexAt
  */
QObject *CcfQmlBaseScenario::unitAt(qreal x, qreal y)
{
    for (int i = 0; i < mUnits.count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem *>(mUnits.at(i));
        QPointF mapped = item->mapFromItem(mZoomArea, QPointF(x, y));
        if (item->contains(mapped)) {
            return mUnits.at(i);
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
    for (int i = 0; i < mUnits.count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem *>(mUnits.at(i));
        QPointF mapped = item->mapFromItem(mZoomArea, QPointF(x, y));
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
    for (int i = 0; i < mUnits.count(); ++i) {
        if (!sides.contains(mUnits.at(i)->getString("unitSide"))) {
            sides.append(mUnits.at(i)->getString("unitSide"));
        }
    }

    // Switch to next one in line.
    for (int i = 0; i < sides.length(); ++i) {
        if (sides.at(i) == mScenarioState->getPlayerSide()) {
            if (i != (sides.length() - 1)) {
                mScenarioState->setPlayerSide(sides.at(i + 1));
            } else {
                mScenarioState->setPlayerSide(sides.at(0));
            }

            mlogger->statusMessage("Player side changed to: " + mScenarioState->getPlayerSide());
            break;
        }
    }

    hideNonPlayerUnits();
    mScenarioWinStatus = "no";
    checkScenarioFinished();
}

/*!
  Hides all units not belonging to current player (regardless of their visibility).
  */
void CcfQmlBaseScenario::hideNonPlayerUnits()
{
    for (int i = 0; i < mUnits.count(); ++i) {
        if (mUnits.at(i)->getString("unitSide") != mScenarioState->getPlayerSide())
            mUnits.at(i)->set("visible", false);
    }
}

/*!
  Sets side marks on units.
  */
void CcfQmlBaseScenario::setSideMarks()
{
    for (int i = 0; i < mUnits.count(); ++i) {
        mUnits.at(i)->set("sideMarkSource", mScenarioState->getSidePath(mUnits.at(i)->getString("unitSide")));
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
    for (int i = 0; i < mUnits.count(); ++i) {
        QObject *currentUnit = mUnits.at(i);
        if ((currentUnit->getString("unitSide") != mScenarioState->getPlayerSide()) && (currentUnit->getString("state") == "healthy")) {
            areAllEnemiesDestroyed = false;
        } else if ((currentUnit->getString("unitSide") == mScenarioState->getPlayerSide()) &&(currentUnit->getString("state") == "healthy")) {
            areAllAlliesDestroyed = false;
        }
    }

    if (mScenarioWinStatus == "no") {
        if (areAllEnemiesDestroyed) {
            mScenarioWinStatus = "won";
            mlogger->statusMessage("All enemies destroyed. You have won!");
        } else if (areAllAlliesDestroyed) {
            mScenarioWinStatus = "lost";
            mlogger->statusMessage("All allies destroyed. You have lost!");
        }
    }
}

/*!
  Updates the rubber band (dimensions).
  */
void CcfQmlBaseScenario::updateRubberBand(qreal x, qreal y)
{
    if (qFuzzyCompare(x, 0.0))
        x = mMouseAreaMain->getReal("mouseX");
    if (qFuzzyCompare(y, 0.0))
        y = mMouseAreaMain->getReal("mouseY");

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
    for (int i = 0; i < mUnits.count(); ++i) {
        CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(i));

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
    CcfQmlBaseUnit *unit = ccfUnit(mUnits.at(index));
    if ((unit->getUnitSide() != mScenarioState->getPlayerSide())
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
    mUnits.at(index)->set("selected", false);
}

/*!
  Deselects all units.
  */
void CcfQmlBaseScenario::deselectAllUnits()
{
    invoke(child("soldierMenu"), "clear");
    for (int i = 0; i < mUnits.count(); ++i) {
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

    unit = mRoster->getUnitAt(mouse->getReal("x"), mouse->getReal("y"));

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
    QObjectList friendlyUnitsList = playerUnits(mScenarioState->getPlayerSide());
    QObjectList enemyUnitsList = enemyUnits(mScenarioState->getPlayerSide());

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

            if (mTerrain->isTargetVisible(x1, y1, x2, y2)
                    && !CcfEngineHelpers::isObstacleInLOS(mProps,
                                                          x1, y1, x2, y2,
                                                          friendly)) {
                mlogger->log("Now visible: " + enemy->objectName()
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
    if ((!mContextMenu->isVisible()) && (mAimLine->isVisible())) {
        if (mouse->getInt("modifiers") == Qt::ShiftModifier) {
            placeWaypoint(mAimLine->getInt("unitIndex"),
                          mMouseAreaMain->getReal("mouseX"),
                          mMouseAreaMain->getReal("mouseY"));
        } else {
            performContextAction(mAimLine->getInt("unitIndex"),
                                 mMouseAreaMain->getReal("mouseX"),
                                 mMouseAreaMain->getReal("mouseY"));
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
    if (mAimLine->isVisible() == true)
        cancelAllSelectedOrders();

    QObject *unitObject = NULL;
    unitObject = unitAt(mMouseAreaMain->getReal("mouseX"), mMouseAreaMain->getReal("mouseY"));

    if ((unitObject == mMouseAreaMain) || (unitObject == NULL)
            || (unitObject->getString("unitSide") != mScenarioState->getPlayerSide())) {
        deselectAllUnits();
        return;
    }

    CcfQmlBaseUnit *unit = ccfUnit(unitObject);

    if (!unit->getSelected()) {
        selectUnitFromGameArea(mouse);
    }

    // Fixes context menu at the centre of unit object.
    QPointF mappedCoords = mapFromItem(mGameArea,
                                       QPointF((unit->x() + unit->getCenterX()) * mZoom,
                                               (unit->y() + unit->getCenterY()) * mZoom));

    setContextMenuPosition(mContextMenu,
                           mappedCoords.x() - (mGameArea->getReal("contentX")),
                           mappedCoords.y() - (mGameArea->getReal("contentY")));

    mContextMenu->setVisible(true);
    //    m_contextMenu->set("unitIndex", unit->getUnitIndex());
    mAimLine->set("unitIndex", unit->getUnitIndex());
}

/*!
  Handles left mouse click on roster menu.
  */
void CcfQmlBaseScenario::handleLeftMouseClickRoster(QObject *mouse)
{
    if (!mContextMenu->isVisible()) {
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

    CcfQmlBaseUnit *unit = mRoster->getUnitAt(mousex, mousey);

    if (unit != 0) {
        if (!unit->getSelected()) {
            selectUnitFromRoster(mouse);
        }

        QPointF rosterUnitCoords = mRoster->childCenterCoords(mousex, mousey);
        // Fixes context menu at the centre of unit object.
        setContextMenuPosition(mContextMenu,
                               child("menu")->getReal("x") + rosterUnitCoords.x(),
                               height() + child("menu")->getReal("y") + rosterUnitCoords.y());

        // Displays the context menu. This is suboptimal.
        mContextMenu->setVisible(true);
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
                if ((mFollowingTimer->getBool("running") == false) && (selectedOnes.length() > 0)) {
                    //                    unitIndex = selectedOnes[0].unitIndex;
                    startFollowingUnit(selectedOnes.at(0)->getInt("unitIndex"));
                } else if (mFollowingTimer->getBool("running") == true) {
                    if (selectedOnes.length() > 0) {
                        int unitIndex = selectedOnes.at(0)->getInt("unitIndex");

                        if (mFollowingTimer->getInt("index") == unitIndex) {
                            stopFollowingUnit();
                        } else {
                            startFollowingUnit(unitIndex);
                        }
                    } else {
                        stopFollowingUnit();
                    }
                } else {
                    mlogger->log("No unit selected to follow.");
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
        mZoom += delta.y()/800;
        emit zoomChanged();
    } else if (wheel->getInt("modifiers") == Qt::ShiftModifier) {
        int magnitude = 0;
        if (delta.x() != 0)
            magnitude = delta.x();
        else
            magnitude = delta.y();

        invoke(mGameArea, "flick", Q_ARG(qreal, magnitude * 5), Q_ARG(qreal, 1.0));
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
    return mScenarioFile;
}

/*!
  Returns the scenario win status.
  */
QString CcfQmlBaseScenario::getScenarioWinStatus() const
{
    return mScenarioWinStatus;
}

/*!
  Returns aim line rotation in degrees.
  */
int CcfQmlBaseScenario::getAimLineRotation() const
{
    return mAimLineRotation;
}

/*!
  Returns true if this is a campaign.
  */
bool CcfQmlBaseScenario::getIsCampaign() const
{
    return mIsCampaign;
}

/*!
  Returns the path to map file.
  */
QString CcfQmlBaseScenario::getMapFile() const
{
    return mMapFile;
}

/*!
  Returns the zoom level.
  */
qreal CcfQmlBaseScenario::getZoom() const
{
    return mZoom;
}

/*!
  Returns the central point for zooming.
  */
QPoint CcfQmlBaseScenario::getZoomPoint() const
{
    return mZoomPoint;
}

/*!
  Returns true if the game is paused.
  */
bool CcfQmlBaseScenario::getPaused() const
{
    return mPaused;
}

/*!
  Returns the current menu's background colour.
  */
QColor CcfQmlBaseScenario::getMenuBackgroundColor() const
{
    return mMenuBackgroundColor;
}

/*!
  Returns a list of all units in this scenario.
  */
QObjectList CcfQmlBaseScenario::getUnits() const
{
    return mUnits;
}

// Property setters:
/*!
  Sets the \a scenarioFile, initialises the whole scenario object runtime.
  */
void CcfQmlBaseScenario::setScenarioFile(const QString &scenarioFile)
{
    bool wasChaged = false;
    if (scenarioFile != mScenarioFile)
        wasChaged = true;

    mScenarioFile = scenarioFile;

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
    if (scenarioWinStatus != mScenarioWinStatus)
        wasChaged = true;

    mScenarioWinStatus = scenarioWinStatus;

    if (wasChaged)
        emit scenarioWinStatusChanged();
}

/*!
  Sets \a aimLineRotation.
  */
void CcfQmlBaseScenario::setAimLineRotation(int aimLineRotation)
{
    bool wasChaged = false;
    if (aimLineRotation != mAimLineRotation)
        wasChaged = true;

    mAimLineRotation = aimLineRotation;

    if (wasChaged)
        emit aimLineRotationChanged();
}

/*!
  Determies whether this scenario is part of a campaign with \a isCampaign.
  */
void CcfQmlBaseScenario::setIsCampaign(bool isCampaign)
{
    bool wasChaged = false;
    if (isCampaign != mIsCampaign)
        wasChaged = true;

    mIsCampaign = isCampaign;

    if (wasChaged)
        emit isCampaignChanged();
}

/*!
  Sets scenario's \a mapFile.
  */
void CcfQmlBaseScenario::setMapFile(const QString &mapFile)
{
    bool wasChaged = false;
    if (mapFile != mMapFile)
        wasChaged = true;

    mMapFile = mapFile;

    if (wasChaged)
        emit mapFileChanged();
}

/*!
  Sets map's \a zoom level.
  */
void CcfQmlBaseScenario::setZoom(qreal zoom)
{
    bool wasChaged = false;
    if (zoom != mZoom)
        wasChaged = true;

    mZoom = zoom;

    if (wasChaged)
        emit zoomChanged();
}

/*!
  Sets the point (\a zoomPoint) which is the central point for zooming.
  */
void CcfQmlBaseScenario::setZoomPoint(const QPoint &zoomPoint)
{
    bool wasChaged = false;
    if (zoomPoint != mZoomPoint)
        wasChaged = true;

    mZoomPoint = zoomPoint;

    if (wasChaged)
        emit zoomPointChanged();
}

/*!
  Sets the game into \a paused or unpaused state.
  */
void CcfQmlBaseScenario::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != mPaused)
        wasChaged = true;

    mPaused = paused;

    if (wasChaged)
        emit pausedChanged();
}

/*!
  Sets menu background colour (\a menuBackgroundColor).
  */
void CcfQmlBaseScenario::setMenuBackgroundColor(const QColor &menuBackgroundColor)
{
    bool wasChaged = false;
    if (menuBackgroundColor != mMenuBackgroundColor)
        wasChaged = true;

    mMenuBackgroundColor = menuBackgroundColor;

    if (wasChaged)
        emit menuBackgroundColorChanged();
}

/*!
  Sets the \a units taking part in this scenario.
  */
void CcfQmlBaseScenario::setUnits(QObjectList units)
{
    mUnits = units;
    emit unitsChanged();
}
