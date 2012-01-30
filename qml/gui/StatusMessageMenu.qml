import QtQuick 1.1
import "../../qml/gui/menuEntries"

Rectangle {
    property int entryWidth: 175
    property bool empty: true
    property color backgroundColor: "#7e8c24"

    id: root
    height: 200
    width: 180
    color: backgroundColor
    border.color: "#1e1c00"
    border.width: 2

    Component.onCompleted: {
        Global.newStatusMessage.connect(newStatusMessageReceived);
    }

    function newStatusMessageReceived(message, object) {
        messageModel.append({"message": message});
        if (empty == true)
            empty = false;
    }

    ListModel {
        id: messageModel
    }

    ListView {
        id: messages
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.leftMargin: 2
        spacing: 2
//        clip: true

            model: messageModel

            delegate: StatusMessageMenuEntry {
                width: entryWidth
                entryText: message
                backgroundColor: root.backgroundColor
            }
    }
}
