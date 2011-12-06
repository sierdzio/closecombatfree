import QtQuick 1.1

Rectangle {
    property int size: 30
    property string text: "o"
    property color backgroundColor: "#dd333232"

    signal zoomButtonClicked();

    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    width: size
    height: size
    radius: 5

    Text {
        color: "#adadad"
        text: parent.text
        anchors.verticalCenterOffset: -2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
        font.bold: true

        MouseArea {
            id: mouseAreaZoom
            acceptedButtons: Qt.LeftButton
            anchors.fill: parent

            onClicked: zoomButtonClicked();
        }
    }
}
