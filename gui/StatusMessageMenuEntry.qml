import QtQuick 1.1
import "../base/engineLogicHelpers.js" as Logic

Rectangle {
    property string entryText: ""
    property color entryTextColor: "#000000"
    property string entryPicture: ""

    id: root
    color: "#5e6c24"
    border.color: "#1e1c00"
    border.width: 1
    width: 175
    height: entry.height

    Text {
        id: entry
        height: paintedHeight
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: logo.left
        color: entryTextColor
        text: entryText
        font.pointSize: 9
        font.family: "Ubuntu Condensed"
        verticalAlignment: Text.Top
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    Image {
        id: logo
        height: entry.height
        width: entry.height
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.leftMargin: 2
        anchors.topMargin: 2
        source: entryPicture
    }
}