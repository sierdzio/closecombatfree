import QtQuick 1.1

Rectangle {
    property int size: 50
    property bool poppedUp: false
    property color backgroundColor: "#dd333232"

    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1
    width: size
    height: width
    radius: 5

    Image {
        opacity: 0.7
        width: parent.width - 10
        height: width
        source: "../../img/gui/popUpArrow.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        rotation: (poppedUp)? 180 : 0
    }

    MouseArea {
        id: mouseAreaPopUp
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent

        onClicked: {
            poppedUp = !poppedUp;
        }
    }
}
