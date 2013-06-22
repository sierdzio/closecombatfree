import QtQuick 2.1

Item {
    signal clicked
    property string text: "Default:"
    property string imageSource: "../../../img/menus/CardMenuDefault.png"

    id: root
    height: image.height
    width: image.width

    Text {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        text: root.text
        font.pointSize: 15

        Image {
            id: image
            anchors.left: parent.left
            anchors.top: parent.bottom

            source: root.imageSource            
            fillMode: Image.PreserveAspectFit
        }
    }

    MouseArea {
        anchors.fill: parent

        acceptedButtons: Qt.LeftButton
        hoverEnabled: false
        onClicked: root.clicked();
    }
}
