import QtQuick 2.1
import "../../qml/units"
import "../../qml/maps"

// Should be in C++!
QtObject {
    /*
        This still has to be thought out. Currently, I'll focus on implementing
        "mapList" campaigns. In the future, more possibilities should be added,
        maybe here, or to other general Campaign type files.

        mapList campaing is a campaign much like those found in Close Combat 3,
        where there is a set of map clusters that the player has to go through.
        There is some inlinearity within clusters, but overal this campaign type
        remains linear.

        Map clusters should probably be introduced as another QML type - it would
        be handy not only here, but in other places, too (MP custom campaigns?).
      */
    property string campaignType: "mapList" // {mapList, freeMap, worldMap}
    property string campaignName: "default mapList campaign"
    property string campaignDescription: ("Lorem ipsum dolor sit amet, "
    + "consectetur adipisicing elit, sed do eiusmod tempor incididunt ut "
    + "labore et dolore magna aliqua.");
    property string campaignImage: "../../img/campaigns/campaign_tst1_logo.png"

    property list<MapCluster> mapClusters; // better to use "var" in Qt5
    property list<Unit> friendlyUnitsList; // we'll see if that's a good idea
                                           // foes will be kept in mapClusters.
                                           // This way they can change in every
                                           // cluster.
                                           // Support for many sides should be added!

    objectName: "Campaign"
}
