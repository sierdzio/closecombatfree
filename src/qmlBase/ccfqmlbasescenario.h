#ifndef CCFQMLBASESCENARIO_H
#define CCFQMLBASESCENARIO_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtQuick/QQuickItem>
#include <QtQml/QQmlListReference>

/*!
 * \brief The CcfQmlBaseScenario class is an experiment to see if moving most functionality
 * from Scenario.qml to here is sane, doable, and does not require too much effort
 * and nasty workarounds.
 */
class CcfQmlBaseScenario : public QQuickItem
{
    Q_OBJECT

    // Many of those can be changed into local vars, probalby
    Q_PROPERTY(QString scenarioFile READ getScenarioFile WRITE setScenarioFile NOTIFY scenarioFileChanged)
    Q_PROPERTY(QString scenarioWinStatus READ getScenarioWinStatus WRITE setScenarioWinStatus NOTIFY scenarioWinStatusChanged)
    Q_PROPERTY(int aimLineRotation READ getAimLineRotation WRITE setAimLineRotation NOTIFY aimLineRotationChanged)
    Q_PROPERTY(int rubberBandRotation READ getRubberBandRotation WRITE setRubberBandRotation NOTIFY rubberBandRotationChanged)
    Q_PROPERTY(int unitIndex READ getUnitIndex WRITE setUnitIndex NOTIFY unitIndexChanged)
    // Is this a single scenario, or a campaign? Convenience bool (same info can
    // be taken from other sources
    Q_PROPERTY(bool isCampaign READ getIsCampaign WRITE setIsCampaign NOTIFY isCampaignChanged)
    // Map path for campaigns
    Q_PROPERTY(QString mapFile READ getMapFile WRITE setMapFile NOTIFY mapFileChanged)
    // Holds graphical effects (muzzle flashes, hit animations etc.).
//    Q_PROPERTY(QList<QObject *> effectsContainer READ getEffectsContainer WRITE setEffectsContainer NOTIFY effectsContainerChanged)
    // Holds unit groups (ones created with CTRL + digit).
//    Q_PROPERTY(QList<QObject *> unitGroups READ getUnitGroups WRITE setUnitGroups NOTIFY unitGroupsChanged)
    Q_PROPERTY(qreal zoom READ getZoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QPoint zoomPoint READ getZoomPoint WRITE setZoomPoint NOTIFY zoomPointChanged)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(QColor menuBackgroundColor READ getMenuBackgroundColor WRITE setMenuBackgroundColor NOTIFY menuBackgroundColorChanged)

    Q_PROPERTY(QQmlListReference units READ getUnits WRITE setUnits NOTIFY unitsChanged)

public:
    explicit CcfQmlBaseScenario(QQuickItem *parent = 0);

    // Property getters:
    QString getScenarioFile();
    QString getScenarioWinStatus();
    int getAimLineRotation();
    int getRubberBandRotation();
    int getUnitIndex();
    bool getIsCampaign();
    QString getMapFile();
//    QList<QObject *> getEffectsContainer();
//    QList<QObject *> getUnitGroups();
    qreal getZoom();
    QPoint getZoomPoint();
    bool getPaused();
    QColor getMenuBackgroundColor();

    QQmlListReference getUnits();

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

#endif // CCFQMLBASESCENARIO_H
