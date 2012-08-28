#ifndef CCFQMLBASESCENARIO_H
#define CCFQMLBASESCENARIO_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtQuick/QQuickItem>
#include <QtQml/QQmlListReference>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
  The CcfQmlBaseScenario class is an experiment to see if moving most functionality
  from Scenario.qml to here is sane, doable, and does not require too much effort
  and nasty workarounds.
 */
class CcfQmlBaseScenario : public QQuickItem
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
    Q_PROPERTY(int rubberBandRotation READ getRubberBandRotation WRITE setRubberBandRotation NOTIFY rubberBandRotationChanged)

    /*!
      Stores (probably) current unit's index, for passing to scripts. This is a good candidate for removal

      // TODO: remove, handle in c++.
      */
    Q_PROPERTY(int unitIndex READ getUnitIndex WRITE setUnitIndex NOTIFY unitIndexChanged)

    /*!
      Is this a single scenario, or a campaign? Convenience bool (same info can
      be taken from other sources
      */
    Q_PROPERTY(bool isCampaign READ getIsCampaign WRITE setIsCampaign NOTIFY isCampaignChanged)

    /*!
      Map path for campaigns.
      */
    Q_PROPERTY(QString mapFile READ getMapFile WRITE setMapFile NOTIFY mapFileChanged)
    // Holds graphical effects (muzzle flashes, hit animations etc.).
//    Q_PROPERTY(QList<QObject *> effectsContainer READ getEffectsContainer WRITE setEffectsContainer NOTIFY effectsContainerChanged)
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

public:
    explicit CcfQmlBaseScenario(QQuickItem *parent = 0);

protected:
    // Property getters:
    QString getScenarioFile() const;
    QString getScenarioWinStatus() const;
    int getAimLineRotation() const;
    int getRubberBandRotation() const;
    int getUnitIndex() const;
    bool getIsCampaign() const;
    QString getMapFile() const;
//    QList<QObject *> getEffectsContainer();
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
    void setRubberBandRotation(int rubberBandRotation);
    void setUnitIndex(int unitIndex);
    void setIsCampaign(bool isCampaign);
    void setMapFile(const QString &mapFile);
//    void setEffectsContainer(const QList<QObject *> &effectsContainer);
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
    void rubberBandRotationChanged();
    void unitIndexChanged();
    void isCampaignChanged();
    void mapFileChanged();
//    void effectsContainerChanged();
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
    int m_rubberBandRotation;
    int m_unitIndex;
    bool m_isCampaign;
    QString m_mapFile;
//    QList<QObject *> m_effectsContainer;
//    QList<QObject *> m_unitGroups;
    qreal m_zoom;
    QPoint m_zoomPoint;
    bool m_paused;
    QColor m_menuBackgroundColor;
    QQmlListReference m_units;
};

/*! @}*/

#endif // CCFQMLBASESCENARIO_H
