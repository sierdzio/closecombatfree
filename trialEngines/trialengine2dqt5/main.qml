import QtQuick 2.0
import QtWebKit 1.0

Rectangle {
    id: main
    width: 360
    height: 360
    color: "lightgray"
    Text {
        id: text1
        text: qsTr("Hello World")
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
    }
    MouseArea {
        id: mousearea1
        anchors.fill: parent
        onClicked: {
//            Qt.quit();
            main.state = "afterClick"
        }

        WebView {
            id: web_view1
            width: 300
            height: 300
            anchors.top: parent.top
            anchors.topMargin: 4
            anchors.left: parent.left
            anchors.leftMargin: 4
            url: "about:blank"
//            url: "http://www.gravatar.com/avatar/e1eb64c6fbad857512a62c027f102162?s=300"
        }
    }
    states: [
        State {
            name: "afterClick"
//            when: mousearea1.pressed// == true

            PropertyChanges {
                target: web_view1
//                x: 0
                y: -312
                anchors.topMargin: -312
//                anchors.leftMargin: 0
            }

            PropertyChanges {
                target: text1
                color: "#730909"
                font.bold: true
                font.pointSize: 15
            }
        }
    ]

    transitions: Transition {
        from: ""; to: "afterClick"; reversible: true
        NumberAnimation {
            target: web_view1
            properties: "anchors.topMargin"; duration: 500; easing.type: Easing.InCirc
        }
    }
}
