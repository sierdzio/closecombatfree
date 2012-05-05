import QtQuick 2.0

QtObject {
    // Angle - 0-360 (degrees)
    property int windDirection: 0
    // Wind speed (km/h)
    property int windMagnitude: 0
    // Time of day
    property time timeOfDay: "12:00:00"
    // Rain magnitude. 0 means no rain
    property int rainMagnitude: 0

    // ... and so on. It may get bigger in the future.

    property string objectType: "weather"
}
