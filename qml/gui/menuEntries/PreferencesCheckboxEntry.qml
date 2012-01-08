import QtQuick 1.1

Text {
    property bool checked: true
    property bool enabled: true

    id: mainText
    color: "#adadad"
    text: "Entry text"
    width: paintedWidth  + 10 + checkBox.width
    height: paintedHeight
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    font.pointSize: 8
    font.bold: true

    Rectangle {
        id: checkBox
        color: "#00000000"
        border.width: 1
        border.color: "#adadad"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        height: 15
        width: 15
        radius: 5

        Image {
            id: tickImage
            anchors.fill: parent
            source: "../../../img/gui/checkBoxTick.png"
            visible: checked
        }
    }

    MouseArea {
        id: mouseAreaEntry
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent
        enabled: mainText.enabled

        onClicked: {
            checked = !checked;
        }
    }
}
