import QtQuick 1.1

Rectangle {
    property int entryWidth: 175

    id: root
    height: 200
    width: 180
    color: "#7e8c24"
    border.color: "#1e1c00"
    border.width: 2

    Component.onCompleted: {
        newStatusMessage.connect(newStatusMessageReceived);
    }

    function newStatusMessageReceived(message, object) {
        messageModel.append({"message": message});
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

            model: messageModel

            delegate: StatusMessageMenuEntry {
                width: entryWidth
                entryText: message
            }
    }
}
