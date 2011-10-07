import QtQuick 1.1
import "tanks"
import "gui"

Rectangle {
    property variant __handledObject // Keeps currently handled object. Very bad implementation,
                                     // to be fixed later. At the very least, move it into JS script.
    property string __scheduledOperation

    id: root
    width: 1000
    height: 800
    color: "lightgray"

    Loader {
        id: contextLoader
        rotation: 0
        z: root.z + 1
    }

    Text {
        id: textHello
        text: qsTr("Hello World")
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
    Tank_tst1 {
        id: tank1
        x: parent.width - 150
        y: 100
    }
    Tank_tst1 {
        id: tank2
        x: parent.width - 300
        y: 100
    }
    Tank_tst1 {
        id: tank3
        x: parent.width - 450
        y: 100
        rotation: 180
    }
    Tank_tst1 {
        id: tank4
        x: parent.width - 600
        y: 100
        rotation: 0
    }

    MouseArea {
        id: mouseAreaMain
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        z: -1

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                if (contextLoader.source != "") {
                    performContextAction(mouseAreaMain.mouseX, mouseAreaMain.mouseY);
                    return;
                }

                if (root.state == "")
                    root.state = "afterClick";
                else
                    root.state = "";
            }
            else if (mouse.button == Qt.RightButton) {
                // "Hide" context menu. This is suboptimal and should be changed.
                if (contextLoader.source != "") {
                    cleanContextAction();
                    return; // Makes app 'eat' this mouse click.
                }

                var child;
                child = root.childAt(mouseAreaMain.mouseX, mouseAreaMain.mouseY);

                if ((child == mouseAreaMain) || (child == mouseAreaTank3)) {
                    return;
                }
                if (child.centerX != "undefined") {
                    contextLoader.y = child.y + child.centerY;
                    contextLoader.x = child.x + child.centerX;
                    contextLoader.source = "gui/ContextMenu.qml";
                    __handledObject = child;
                    contextLoader.item.menuEntryClicked.connect(scheduleContextAction);
                }
            }
        }
    }
    MouseArea {
        id: mouseAreaTank3
        anchors.fill: tank3
        acceptedButtons: Qt.LeftButton
        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                tank3.fireTo(tank4.x, tank4.y);
                tank3.moveTo(600, 300);
            }
        }
    }

    states: [
        State {
            name: "afterClick"
            PropertyChanges {
                target: textHello
                color: "#730909"
                font.bold: true
                font.pointSize: 15
            }
        }
    ]

    function scheduleContextAction(operation) {
        __scheduledOperation = operation;
        contextLoader.visible = false;
    }

    function performContextAction(targetX, targetY) {
        if (__scheduledOperation == "Move") {
            __handledObject.moveTo(targetX, targetY);
        } else if (__scheduledOperation == "Attack") {
            __handledObject.fireTo(targetX, targetY);
        }

        cleanContextAction();
    }

    function cleanContextAction() {
        contextLoader.source = "";
        contextLoader.visible = true;
        __scheduledOperation = "";
        __handledObject = 0;
    }
}
