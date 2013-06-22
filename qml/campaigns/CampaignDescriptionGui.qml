import QtQuick 2.1

Item {
    id: root

    width: 800
    height: 600

    Image {
        id: campaignImage
        source: "../../img/campaigns/campaign_tst1_logo.png"

        anchors.left: root.left
        anchors.top: root.top
        anchors.topMargin: 15

        width: 150
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: campaignDescription
        text: "Lorem ipsum dolor sit amet\nThis is a description"

        anchors.left: root.left
        anchors.top: campaignImage.bottom
        anchors.topMargin: 15
        anchors.bottom: root.bottom

        width: campaignImage.width
    }

    // TODO: as another pane on the right, add unit list, or some force description
}
