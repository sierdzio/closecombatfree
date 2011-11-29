import QtQuick 1.1

Rectangle {
    property int size: 50
    property string text: "o"
    property string additionalText: ""
    property color backgroundColor: "#bb333232"

    signal entryClicked()

    id: root
    color: backgroundColor
    width: size * 2
    height: size
    radius: 5

    Text {
        id: mainText
        color: "#adadad"
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: size/4
        font.bold: true
    }

    Text {
        id: additional
        color: "#adadad"
        text: parent.additionalText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: mainText.bottom
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: size/5
//        font.bold: true
        visible: (additional.text != "")? true : false;
    }

    MouseArea {
        id: mouseAreaEntry
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent

        onClicked: {
            entryClicked();
        }
    }
}
