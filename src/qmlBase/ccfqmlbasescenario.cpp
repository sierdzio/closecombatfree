#include "ccfqmlbasescenario.h"

CcfQmlBaseScenario::CcfQmlBaseScenario(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_zoom = 1.0;
    m_paused = false;
}

// Non-properties

/*!
  Checks which order markers should be visible and clears those no longer needed.
  */
void CcfQmlBaseScenario::calculateOrderMarkerVisibility(int index) {
    var orderMarker = orderMarkersContainer[index];
    var unit = unitsLoader.item.children[index];

    var anyOrdersLeft = false;
    var orders = unit.orderQueue;

    if (orders.length == 0) {
        for (var i = 0; i < orderMarker.length; i++) {
            if (orderMarker[i] != 0) {
                orderMarker[i].destroy();
                orderMarker[i] = 0;
            }
        }
    } else {
        for (var i = 0; i < orders.length; i++) {
            if (orders[i].performed == true) {
                if (i < orderMarker.length) {
                    if (orderMarker[i] != 0) {
                        orderMarker[i].destroy();
                        orderMarker[i] = 0;
                    }
                }
            } else {
                anyOrdersLeft = true;
            }
        }
    }

    // Clean markers on queue finish
    if (anyOrdersLeft == false) {
        orderMarker = new Array;
    }
}

/*!
  Crreates an order marker and places it on game map.
  */
void CcfQmlBaseScenario::setOrderMarker(int index, int orderNumber, const QString &orderName,
                                        qreal targetX, qreal targetY) {
    // This component renders an order marker.
    var component = Qt.createComponent("../../qml/gui/OrderMarker.qml");
    var marker;

    if (component.status == Component.Ready) {
        marker = component.createObject(itemContainer);
        marker.visible = true;
        marker.index = index;
        marker.number = orderNumber;
        marker.dragComplete.connect(modifyTargetFromMarker);
        orderMarkersContainer[index][orderNumber] = marker;
    }

    marker.x = (targetX - marker.centerX);
    marker.y = (targetY - marker.centerY);
    marker.orderColor = EngineHelpers.colorForOrder(orderName);
    marker.visible = true;
}

/*!
  When user drags order marker about the game area, unit's orders are being updated.
  */
void CcfQmlBaseScenario::modifyTargetFromMarker(int unitIndex, int orderNumber) {
    var marker = orderMarkersContainer[unitIndex][orderNumber];
    var newX = marker.x + marker.centerX;
    var newY = marker.y + marker.centerY;
    var unit = unitsLoader.item.children[unitIndex];

    // Not sure whether this should stay!
    //    unit.cancelOrder();
    unit.modifyOrder(orderNumber, newX, newY);
}

/*!
  Initialises order markers' container.
  */
void CcfQmlBaseScenario::initOrderMarkers() {
    for (var i = 0; i < unitsLoader.item.children.length; i++) {
        orderMarkersContainer[i] = new Array;
    }
}

// Property getters:

QString CcfQmlBaseScenario::getScenarioFile()
{
    return m_scenarioFile;
}

QString CcfQmlBaseScenario::getScenarioWinStatus()
{
    return m_scenarioWinStatus;
}

int CcfQmlBaseScenario::getAimLineRotation()
{
    return m_aimLineRotation;
}

int CcfQmlBaseScenario::getRubberBandRotation()
{
    return m_rubberBandRotation;
}

int CcfQmlBaseScenario::getUnitIndex()
{
    return m_unitIndex;
}

bool CcfQmlBaseScenario::getIsCampaign()
{
    return m_isCampaign;
}

QString CcfQmlBaseScenario::getMapFile()
{
    return m_mapFile;
}

QList<QObject *> CcfQmlBaseScenario::getEffectsContainer()
{
    return m_effectsContainer;
}

//QList<QObject *> CcfQmlBaseScenario::getOrderMarkersContainer()
//{
//    return m_orderMarkersContainer;
//}

QList<QObject *> CcfQmlBaseScenario::getUnitGroups()
{
    return m_unitGroups;
}

qreal CcfQmlBaseScenario::getZoom()
{
    return m_zoom;
}

QPoint CcfQmlBaseScenario::getZoomPoint()
{
    return m_zoomPoint;
}

bool CcfQmlBaseScenario::getPaused()
{
    return m_paused;
}

QColor CcfQmlBaseScenario::getMenuBackgroundColor()
{
    return m_menuBackgroundColor;
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

void CcfQmlBaseScenario::setEffectsContainer(const QList<QObject *> &effectsContainer)
{
    bool wasChaged = false;
    if (effectsContainer != m_effectsContainer)
        wasChaged = true;

    m_effectsContainer = effectsContainer;

    if (wasChaged)
        emit effectsContainerChanged();
}

//void CcfQmlBaseScenario::setOrderMarkersContainer(const QList<QObject *> &orderMarkersContainer)
//{
//    qDebug(qPrintable("O. markers length: %d"), orderMarkersContainer.length());
//    bool wasChaged = false;
//    if (orderMarkersContainer != m_orderMarkersContainer)
//        wasChaged = true;

//    m_orderMarkersContainer = orderMarkersContainer;

//    if (wasChaged)
//        emit orderMarkersContainerChanged();
//}

void CcfQmlBaseScenario::setUnitGroups(const QList<QObject *> &unitGroups)
{
    bool wasChaged = false;
    if (unitGroups != m_unitGroups)
        wasChaged = true;

    m_unitGroups = unitGroups;

    if (wasChaged)
        emit unitGroupsChanged();
}

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
