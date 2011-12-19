import QtQuick 1.1

Rectangle {
    property int size: 30
    property string text: "o"
    property string additionalText: ""
    property color backgroundColor: "#bb333232"
    property bool enabled: true

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

        return (result + (radius));
    }

    height: {
        var objectsHeight = mainText.paintedHeight + additional.paintedHeight;
        if (objectsHeight > 30) {
            return objectsHeight;
        } else {
            return 30;
        }
    }

    function getContentsHeight() {
        var result = 0;
        var textHeight = mainText.paintedHeight;
        if (additional.visible == true) {
            textHeight += additional.paintedHeight;
        }

        result = textHeight;

        return (result + (radius));
    }

    radius: size/8

    Text {
        id: mainText
        color: "#adadad"
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 8
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
        font.pointSize: 6
        visible: (parent.additionalText != "")? true : false;
    }

    MouseArea {
        id: mouseAreaEntry
        acceptedButtons: Qt.LeftButton
        anchors.fill: parent
        enabled: parent.enabled

        onClicked: {
            entryClicked();
        }
    }
}
