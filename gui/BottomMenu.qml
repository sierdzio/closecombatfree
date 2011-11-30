import QtQuick 1.1

Item {
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"
    property int visibleHeight: 0

    id: root
//    height: 200
    state: "closed"

    PopUpArrow {
        id: trigger
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        //        size: root.height
        size: 30
        z: menu.z + 1
    }

    states: [
        State {
            when: (open == false)
            name: "closed"
//            AnchorChanges {
//                target: menu
//                anchors.top: root.bottom
//            }
        },

        State {
            when: (open == true)
            name: "opened"
//            AnchorChanges {
//                target: menu
//                anchors.top: root.top
//            }
        }
    ]

    transitions: [
        Transition {
            from: "closed"
            to: "opened"

            SequentialAnimation {
                ScriptAction {
                    script: menu.visible = true;
                }
//                AnchorAnimation {
//                    duration: 500
//                }
                NumberAnimation {
                    target: menu
                    properties: "y"
                    to: -menu.height
                    duration: 500
                }
            }
        },

        Transition {
            from: "opened"
            to: "closed"

            SequentialAnimation {
                AnchorAnimation {
                    duration: 500
                }
//                ScriptAction {
//                    script: menu.visible = false;
//                }
                NumberAnimation {
                    target: menu
                    properties: "y"
                    to: 0//-menu.height
                    duration: 500
                }
            }
        }
    ]
}
