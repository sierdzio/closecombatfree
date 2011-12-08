import QtQuick 1.1

Item {
    property int centerX: hullWidth/2
    property int centerY: hullHeight/2
    property color hullColor: "#7b8259"
    property int hullWidth: 62 // mm - should be properly recalculated later on
    property int hullHeight: 100 // mm - should be properly recalculated later on

    id: root
    width: hullWidth
    height: hullHeight
}
