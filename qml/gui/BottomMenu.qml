import QtQuick 1.1

Item {
    property bool open: trigger.poppedUp
    property color backgroundColor: "#dd333232"
    property int visibleHeight: 0
    property int popupSize: 30

    signal openMenu();
    onOpenMenu: trigger.poppedUp = true;

    signal toggleMenu();
    onToggleMenu: trigger.poppedUp = !trigger.poppedUp;

    id: root
    state: "closed"

    PopUpArrow {
        id: trigger
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        rotation: 90
        size: popupSize
        z: menu.z + 1
    }

    states: [
        State {
            when: (open == false)
            name: "closed"
        },

        State {
            when: (open == true)
            name: "opened"
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
                NumberAnimation {
                    target: menu
                    properties: "y"
                    to: -(menu.height + 5)
                    duration: 300
                }
            }
        },

        Transition {
            from: "opened"
            to: "closed"

            SequentialAnimation {
                NumberAnimation {
                    target: menu
                    properties: "y"
                    to: 0
                    duration: 300
                }
                ScriptAction {
                    script: menu.visible = false;
                }
            }
        }
    ]
}
