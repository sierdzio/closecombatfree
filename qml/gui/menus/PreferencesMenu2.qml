import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {
    width: 500
    height: 400

    GridLayout {
        anchors.fill: parent
        columns: 2

        CheckBox {
            text: "Option 1"
        }

        CheckBox {
            text: "Option 2"
        }

        CheckBox {
            text: "Option 3"
        }
    }
}
