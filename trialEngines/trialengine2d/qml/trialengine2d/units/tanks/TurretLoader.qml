import QtQuick 1.1

Item {
    property string turretFile: ""
//    property color turretColor: "#7b8259"
    property int turretBodySize: 60
    property int centerX: turretBodySize/2
    property int centerY: turretBodySize/2
    property bool firing: false
    property bool smoking: false

    onFiringChanged: turret.item.firing = firing;
    onSmokingChanged: turret.item.smoking = smoking;

    Loader {
        id: turret
        source: turretFile
        anchors.fill: parent

        onLoaded: {
            item.turretBodySize = turretBodySize;
        }
    }
}
