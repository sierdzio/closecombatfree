import QtQuick 2.0
import "../../qml/maps"
import "../../qml/units"

QtObject {
    property string clusterName: "Default cluster"
    property string clusterDescription: ("Lorem ipsum dolor sit amet, "
    + "consectetur adipisicing elit, sed do eiusmod tempor incididunt ut "
    + "labore et dolore magna aliqua.");

    property list<Map> clusterMaps; // better to use "var" in Qt5
    property list<Unit> enemyUnitsList; // we'll see if that's a good idea

    objectName: "MapCluster"
}
