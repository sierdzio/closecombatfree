import QtQuick 1.1

Rectangle {
    property int size: 30
    property string text: "o"
    property string additionalText: ""
    property color backgroundColor: "#bb333232"

    signal entryClicked()

    id: root
    color: backgroundColor
    border.color: Qt.tint(backgroundColor, "#88aaaaaa")
    border.width: 1

    width: {
        var result = 0;
        var textWidth = mainText.paintedWidth;
        if ((additional.visible == true) && (additional.paintedWidth > textWidth)) {
            textWidth = additional.paintedWidth;
        }

        if (size > textWidth)
            result = size;
        else
            result = textWidth;

        return (result + 2 * radius);
    }

    height: {
        var result = 0;
        var textHeight = mainText.paintedHeight;
        if (additional.visible == true) {
            textHeight += additional.paintedHeight;
        }

        result = textHeight;

        return (result + 2 * radius);
    }

    radius: size/8

    Text {
        id: mainText
        color: "#adadad"
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
//        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 10//size/4
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
        font.pointSize: 8//size/5
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
