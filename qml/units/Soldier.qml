import QtQuick 1.1

Item {
    property string name: "John Smith"
    property string role: "Infantryman"
    property string status: "READY"
    property string weapon: ""
    property string soldierLogo: "qrc:/img/units/generic_soldier_logo.png"

    signal unitStatusChanged(string newStatus)
    function changeStatus(newStatusMessage) {
        status = newStatusMessage;
        unitStatusChanged(newStatusMessage);
    }
}
