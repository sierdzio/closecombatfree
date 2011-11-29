import QtQuick 1.1

Rectangle {
    property int size: 30
    property string text: "o"
    property color backgroundColor: "#bb333232"

    signal zoomButtonClicked();

    color: backgroundColor
    width: size
    height: height
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
