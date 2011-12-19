import QtQuick 1.1

Text {
    property bool enabled: true

    id: mainText
    color: "#adadad"
    text: "Window size"
    width: paintedWidth  + row.width
    height: paintedHeight
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    font.pointSize: 8
    font.bold: true

    function getHeight() {
        return inputHeight.text;
    }

    function getWidth() {
        return inputWidth.text;
    }

    function setHeight(newHeight) {
        inputHeight.text = newHeight;
    }

    function setWidth(newWidth) {
        inputWidth.text = newWidth;
    }

    Row {
        id: row
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        spacing: 3

        Rectangle {
            id: widthEdit
            color: "#00000000"
            border.width: 1
            border.color: "#adadad"
            height: 15
            width: 50
            radius: 5

            TextInput {
                id: inputWidth
                color: "#adadad"
                anchors.fill: parent
                anchors.margins: 2
                enabled: mainText.enabled

                horizontalAlignment: Text.AlignRight
                font.pointSize: 8
            }
        }

        Text {
            id: xSign
            color: "#adadad"
            text: "x"
            width: paintedWidth
            height: paintedHeight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 8
            font.bold: true
        }

        Rectangle {
            id: heightEdit
            color: "#00000000"
            border.width: 1
            border.color: "#adadad"
            height: 15
            width: 50
            radius: 5

            TextInput {
                id: inputHeight
                color: "#adadad"
                anchors.fill: parent
                anchors.margins: 2
                enabled: mainText.enabled

                horizontalAlignment: Text.AlignRight
                font.pointSize: 8
            }
        }

        Text {
            id: pixelsSign
            color: "#adadad"
            text: "pixels"
            width: paintedWidth
            height: paintedHeight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 8
            font.bold: true
        }
    }
}
