import QtQuick 1.1

Text {
    property bool enabled: true

    id: mainText
    color: "#adadad"
    text: "Single box entry"
    width: paintedWidth  + textInputBox.width
    height: paintedHeight
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
    font.pointSize: 8
    font.bold: true

    function getText() {
        return textInputElement.text;
    }

    function setText(newText) {
        textInputElement.text = newText;
    }

    Rectangle {
        id: textInputBox
        color: "#00000000"
        border.width: 1
        border.color: "#adadad"
        height: 15
        width: 50
        radius: 5

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        TextInput {
            id: textInputElement
            color: "#adadad"
            anchors.fill: parent
            anchors.margins: 2
            enabled: mainText.enabled

            horizontalAlignment: Text.AlignRight
            font.pointSize: 8
        }
    }
}
