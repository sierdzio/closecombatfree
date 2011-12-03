import QtQuick 1.1

Rectangle {
    property int size: 50
    property int currentZoom: 100
    property color backgroundColor: "#bb333232"

    signal zoomIn();
    signal zoomOut();

    color: backgroundColor
    width: (2 * height) + zoomText.paintedWidth
    height: size
    radius: 5

    Component.onCompleted: {
        zoomInButton.zoomButtonClicked.connect(zoomIn);
        zoomOutButton.zoomButtonClicked.connect(zoomOut);
    }

    ZoomButton {
        id: zoomInButton
        text: "+"
        size: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }

    Text {
        id: zoomText
        text: currentZoom + "%" //(currentZoom * 100) + "%"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: size/2
        width: 40
        color: "#ffffff"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ZoomButton {
        id: zoomOutButton
        text: "-"
        size: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }
}
