#include "ccfqmlbasescenario.h"

/*!
  Standard constructor. Initialises properties to default values.
  */
CcfQmlBaseScenario::CcfQmlBaseScenario(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_zoom = 1.0;
    m_paused = false;
    m_menuBackgroundColor = QColor("#7e8c24");
    m_aimLineRotation = 0;
    m_rubberBandRotation = 0;
    m_unitIndex = -1;
    m_isCampaign = false;
}

// Non-properties:

// Property getters:

QString CcfQmlBaseScenario::getScenarioFile() const
{
    return m_scenarioFile;
}

QString CcfQmlBaseScenario::getScenarioWinStatus() const
{
    return m_scenarioWinStatus;
}

int CcfQmlBaseScenario::getAimLineRotation() const
{
    return m_aimLineRotation;
}

int CcfQmlBaseScenario::getRubberBandRotation() const
{
    return m_rubberBandRotation;
}

int CcfQmlBaseScenario::getUnitIndex() const
{
    return m_unitIndex;
}

bool CcfQmlBaseScenario::getIsCampaign() const
{
    return m_isCampaign;
}

QString CcfQmlBaseScenario::getMapFile() const
{
    return m_mapFile;
}

//QList<QObject *> CcfQmlBaseScenario::getEffectsContainer()
//{
//    return m_effectsContainer;
//}

//QList<QObject *> CcfQmlBaseScenario::getUnitGroups()
//{
//    return m_unitGroups;
//}

qreal CcfQmlBaseScenario::getZoom() const
{
    return m_zoom;
}

QPoint CcfQmlBaseScenario::getZoomPoint() const
{
    return m_zoomPoint;
}

bool CcfQmlBaseScenario::getPaused() const
{
    return m_paused;
}

QColor CcfQmlBaseScenario::getMenuBackgroundColor() const
{
    return m_menuBackgroundColor;
}

QQmlListReference CcfQmlBaseScenario::getUnits() const
{
    return m_units;
}

// Property setters:

void CcfQmlBaseScenario::setScenarioFile(const QString &scenarioFile)
{
    bool wasChaged = false;
    if (scenarioFile != m_scenarioFile)
        wasChaged = true;

    m_scenarioFile = scenarioFile;

    if (wasChaged)
        emit scenarioFileChanged();
}

void CcfQmlBaseScenario::setScenarioWinStatus(const QString &scenarioWinStatus)
{
    bool wasChaged = false;
    if (scenarioWinStatus != m_scenarioWinStatus)
        wasChaged = true;

    m_scenarioWinStatus = scenarioWinStatus;

    if (wasChaged)
        emit scenarioWinStatusChanged();
}

void CcfQmlBaseScenario::setAimLineRotation(int aimLineRotation)
{
    bool wasChaged = false;
    if (aimLineRotation != m_aimLineRotation)
        wasChaged = true;

    m_aimLineRotation = aimLineRotation;

    if (wasChaged)
        emit aimLineRotationChanged();
}

void CcfQmlBaseScenario::setRubberBandRotation(int rubberBandRotation)
{
    bool wasChaged = false;
    if (rubberBandRotation != m_rubberBandRotation)
        wasChaged = true;

    m_rubberBandRotation = rubberBandRotation;

    if (wasChaged)
        emit rubberBandRotationChanged();
}

void CcfQmlBaseScenario::setUnitIndex(int unitIndex)
{
    bool wasChaged = false;
    if (unitIndex != m_unitIndex)
        wasChaged = true;

    m_unitIndex = unitIndex;

    if (wasChaged)
        emit unitIndexChanged();
}

void CcfQmlBaseScenario::setIsCampaign(bool isCampaign)
{
    bool wasChaged = false;
    if (isCampaign != m_isCampaign)
        wasChaged = true;

    m_isCampaign = isCampaign;

    if (wasChaged)
        emit isCampaignChanged();
}

void CcfQmlBaseScenario::setMapFile(const QString &mapFile)
{
    bool wasChaged = false;
    if (mapFile != m_mapFile)
        wasChaged = true;

    m_mapFile = mapFile;

    if (wasChaged)
        emit mapFileChanged();
}

//void CcfQmlBaseScenario::setEffectsContainer(const QList<QObject *> &effectsContainer)
//{
//    bool wasChaged = false;
//    if (effectsContainer != m_effectsContainer)
//        wasChaged = true;

//    m_effectsContainer = effectsContainer;

//    if (wasChaged)
//        emit effectsContainerChanged();
//}

//void CcfQmlBaseScenario::setUnitGroups(const QList<QObject *> &unitGroups)
//{
//    bool wasChaged = false;
//    if (unitGroups != m_unitGroups)
//        wasChaged = true;

//    m_unitGroups = unitGroups;

//    if (wasChaged)
//        emit unitGroupsChanged();
//}

void CcfQmlBaseScenario::setZoom(qreal zoom)
{
    bool wasChaged = false;
    if (zoom != m_zoom)
        wasChaged = true;

    m_zoom = zoom;

    if (wasChaged)
        emit zoomChanged();
}

void CcfQmlBaseScenario::setZoomPoint(const QPoint &zoomPoint)
{
    bool wasChaged = false;
    if (zoomPoint != m_zoomPoint)
        wasChaged = true;

    m_zoomPoint = zoomPoint;

    if (wasChaged)
        emit zoomPointChanged();
}

void CcfQmlBaseScenario::setPaused(bool paused)
{
    bool wasChaged = false;
    if (paused != m_paused)
        wasChaged = true;

    m_paused = paused;

    if (wasChaged)
        emit pausedChanged();
}

void CcfQmlBaseScenario::setMenuBackgroundColor(const QColor &menuBackgroundColor)
{
    bool wasChaged = false;
    if (menuBackgroundColor != m_menuBackgroundColor)
        wasChaged = true;

    m_menuBackgroundColor = menuBackgroundColor;

    if (wasChaged)
        emit menuBackgroundColorChanged();
}

void CcfQmlBaseScenario::setUnits(QQmlListReference units)
{
    m_units = units;
    emit unitsChanged();
}
