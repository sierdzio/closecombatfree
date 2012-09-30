#ifndef CCFQMLBASESCENARIO_H
#define CCFQMLBASESCENARIO_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtQml/QQmlListReference>

#include "logic/ccfobjectbase.h"

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

class CcfMain;
class CcfQmlBaseRosterMenu;

class QQmlComponent;
class QQuickItem;

/*!
  The CcfQmlBaseScenario class is an experiment to see if moving most functionality
  from Scenario.qml to here is sane, doable, and does not require too much effort
  and nasty workarounds.
 */
class CcfQmlBaseScenario : public CcfObjectBase
{
    Q_OBJECT

    /*!
      Specifies scenario file to be loaded. This is quite crutial, and allowes for deferred loading
      of game levels.
      */
    Q_PROPERTY(QString scenarioFile READ getScenarioFile WRITE setScenarioFile NOTIFY scenarioFileChanged)

    /*!
      Holds the status of the scenario (won, lost, etc.).

      // TODO: enum!
      */
    Q_PROPERTY(QString scenarioWinStatus READ getScenarioWinStatus WRITE setScenarioWinStatus NOTIFY scenarioWinStatusChanged)

    /*!
      Pretty much a private property for controlling aim line's rotation.

      TODO: remove property, handle solely in C++.
      */
    Q_PROPERTY(int aimLineRotation READ getAimLineRotation WRITE setAimLineRotation NOTIFY aimLineRotationChanged)

    /*!
      Also a private property. Needed to resolve some problematic parts of rubber band handling.

      TODO: remove property, handle in c++.
      */
//    Q_PROPERTY(int rubberBandRotation READ getRubberBandRotation WRITE setRubberBandRotation NOTIFY rubberBandRotationChanged)

    /*!
      Is this a single scenario, or a campaign? Convenience bool (same info can
      be taken from other sources
      */
    Q_PROPERTY(bool isCampaign READ getIsCampaign WRITE setIsCampaign NOTIFY isCampaignChanged)

    /*!
      Map path for campaigns.
      */
    Q_PROPERTY(QString mapFile READ getMapFile WRITE setMapFile NOTIFY mapFileChanged)

    // Holds unit groups (ones created with CTRL + digit).
//    Q_PROPERTY(QList<QObject *> unitGroups READ getUnitGroups WRITE setUnitGroups NOTIFY unitGroupsChanged)

    /*!
      Holds current zoom factor of the game area.
      */
    Q_PROPERTY(qreal zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)

    /*!
      Specifies the point on the screen, where the zooming should occur. Defaults to center of the screen.
      */
    Q_PROPERTY(QPoint zoomPoint READ getZoomPoint WRITE setZoomPoint NOTIFY zoomPointChanged)

    /*!
      Is true when the game is paused.
      */
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)

    /*!
      A background colour for menus.
      */
    Q_PROPERTY(QColor menuBackgroundColor READ getMenuBackgroundColor WRITE setMenuBackgroundColor NOTIFY menuBackgroundColorChanged)

    /*!
      A convenience list of units available in current scenario. Includes both friendly and enemy units.
      */
    Q_PROPERTY(QQmlListReference units READ getUnits WRITE setUnits NOTIFY unitsChanged)

//    property var unitGroups: new Array(10);

public:
    explicit CcfQmlBaseScenario(QQuickItem *parent = 0);

    Q_INVOKABLE void initConveniencePointers();

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();

    Q_INVOKABLE QList<QObject *> playerUnits(const QString &player = QString());
    Q_INVOKABLE QList<QObject *> enemyUnits(const QString &player = QString());
    Q_INVOKABLE void groupUnits(int groupNumber);
    Q_INVOKABLE void selectGroup(int groupNumber);

    Q_INVOKABLE bool checkIfUnitCanFire(const QString &scheduledOperation);
    Q_INVOKABLE void performContextAction(int index, qreal targetX, qreal targetY);
    Q_INVOKABLE void placeWaypoint(int index, qreal targetX, qreal targetY);
    Q_INVOKABLE void issueWaypointOrder(CcfQmlBaseUnit *unit, qreal x, qreal y);
    Q_INVOKABLE void issueActionOrder(CcfQmlBaseUnit *unit, qreal x, qreal y);
    Q_INVOKABLE void actionFinished(int index, qreal targetX, qreal targetY);

    Q_INVOKABLE void firingActionFinished(int index, qreal targetX, qreal targetY);
    Q_INVOKABLE void cleanContextAction();
    Q_INVOKABLE void cancelAllSelectedOrders();

    Q_INVOKABLE QList<QObject *> selectedUnits();
    Q_INVOKABLE int selectedUnitsCount();
    // TODO: reconsider if that is needed. If it's used only in c++, then there is no
    // need to have it.
    Q_INVOKABLE QList<QObject *> getAllUnitsButOne(int unitToOmit);

    Q_INVOKABLE void updateEffects();
    Q_INVOKABLE void updateAimLine();

    Q_INVOKABLE QObject *unitAt(qreal x, qreal y);
    Q_INVOKABLE int unitIndexAt(qreal x, qreal y);

    Q_INVOKABLE void togglePlayer();
    Q_INVOKABLE void hideNonPlayerUnits();
    Q_INVOKABLE void setSideMarks();
    Q_INVOKABLE void checkScenarioFinished();

    Q_INVOKABLE void centerViewOnUnit(QObject *unit);
    Q_INVOKABLE void centerViewOn(qreal x, qreal y);
    Q_INVOKABLE void startFollowingUnit(int index);
    Q_INVOKABLE void stopFollowingUnit();
    Q_INVOKABLE bool isFollowingOn();
    Q_INVOKABLE void updateFollowingUnit();
    Q_INVOKABLE void handleUnitMovement(bool isMoving, int unitIndex);

    Q_INVOKABLE void updateRubberBand(qreal x, qreal y);
    Q_INVOKABLE void selectUnit(int index, int modifier); // Qt::KeyboardModifier
    Q_INVOKABLE void deselectUnit(int index);
    Q_INVOKABLE void deselectAllUnits();

    Q_INVOKABLE void selectUnitFromGameArea(QObject *mouse);
    Q_INVOKABLE void selectUnitFromRoster(QObject *mouse);

    Q_INVOKABLE void setContextMenuPosition(QQuickItem *menu, qreal x, qreal y);
    Q_INVOKABLE int digitPressed(QObject *event);
    Q_INVOKABLE void updateUnitVisibility();

    Q_INVOKABLE void handleLeftMouseClick(QObject *mouse);
    Q_INVOKABLE void handleRightMouseClick(QObject *mouse);
    Q_INVOKABLE void handleLeftMouseClickRoster(QObject *mouse);
    Q_INVOKABLE void handleRightMouseClickRoster(QObject *mouse);
    Q_INVOKABLE void handlePressAndHoldLeft(QObject *mouse);
    Q_INVOKABLE void handlePressAndHoldRight(QObject *mouse);
    Q_INVOKABLE void handleMouseReleased();
    Q_INVOKABLE void handleKeyPress(QObject *event);
    Q_INVOKABLE void handleWheelEventMouseAreaMain(QObject *wheel);

public slots:
    // TODO: add optional argument with a list of units to be affected. When empty,
    // it will use selected units
    Q_INVOKABLE void scheduleContextAction(const QString &operation);

signals:
    void togglePause();
    void closeScenario(); // TODO: clean containers to preserve memory
    void loadScenario(const QString &path);

protected slots:
    void updateWidth();
//    void saveGameToFile();

private:
    QObject *createEffect(QObject *parent);
    void onTogglePause();

    CcfMain *m_mainInstance;
    QQmlComponent *m_effectsComponent;
    // TODO: maybe replace with particle system?
    QObjectList m_effects;

    // Convenience pointers:
    QQuickItem *m_aimLine;
    QQuickItem *m_gameArea;
    QQuickItem *m_zoomArea;
    QQuickItem *m_contextMenu;
    QObject *m_effectsTimer;
    QObject *m_rotationTimer;
    QObject *m_followingTimer;
    QObject *m_mouseAreaMain;

    CcfQmlBaseRosterMenu *m_roster;
    QObjectList m_props;
    // TODO: consider changing to QVector
    QMap<int, QObjectList > m_unitGroups;

    // // // //
    // Property handling:
    // // // //
protected:
    // Property getters:
    QString getScenarioFile() const;
    QString getScenarioWinStatus() const;
    int getAimLineRotation() const;
//    int getRubberBandRotation() const;
    bool getIsCampaign() const;
    QString getMapFile() const;
//    QList<QObject *> getUnitGroups();
    qreal getZoom() const;
    QPoint getZoomPoint() const;
    bool getPaused() const;
    QColor getMenuBackgroundColor() const;
    QQmlListReference getUnits() const;

    //Property setters:
    void setScenarioFile(const QString &scenarioFile);
    void setScenarioWinStatus(const QString &scenarioWinStatus);
    void setAimLineRotation(int aimLineRotation);
//    void setRubberBandRotation(int rubberBandRotation);
    void setIsCampaign(bool isCampaign);
    void setMapFile(const QString &mapFile);
//    void setUnitGroups(const QList<QObject *> &unitGroups);
    void setZoom(qreal zoom);
    void setZoomPoint(const QPoint &zoomPoint);
    void setPaused(bool paused);
    void setMenuBackgroundColor(const QColor &menuBackgroundColor);
    void setUnits(QQmlListReference units);

signals:
    void scenarioFileChanged();
    void scenarioWinStatusChanged();
    void aimLineRotationChanged();
//    void rubberBandRotationChanged();
    void isCampaignChanged();
    void mapFileChanged();
//    void unitGroupsChanged();
    void zoomChanged();
    void zoomPointChanged();
    void pausedChanged();
    void menuBackgroundColorChanged();
    void unitsChanged();

private:
    QString m_scenarioFile;
    QString m_scenarioWinStatus;
    int m_aimLineRotation;
//    int m_rubberBandRotation;
    bool m_isCampaign;
    QString m_mapFile;
//    QList<QObject *> m_unitGroups;
    qreal m_zoom;
    QPoint m_zoomPoint;
    bool m_paused;
    QColor m_menuBackgroundColor;
    QQmlListReference m_units;
};

/*! @}*/

#endif // CCFQMLBASESCENARIO_H
