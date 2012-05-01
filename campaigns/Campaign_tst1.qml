import QtQuick 2.0
import "../qml/campaigns"
import "../qml/maps"
import "../maps"
import "../units/tanks/tests"

CampaignMapList {
    campaignName: "Test campaign 1"

    CampaignMapList: [
        MapCluster {
            clusterName: "Initial invasion"
            clusterMaps: [
                Map_tst1 {},
                Map_tst3 {}
            ]

            enemyUnitsList: [
                Tank_tst1 { unitSide: "bad" },
                Tank_tst1 { unitSide: "bad" },
                Tank_tst1 { unitSide: "bad" }
            ]
        },

        MapCluster {
            clusterName: "Secondary map cluster"
            clusterDescription: "You have won in cluster 1, welcome to cluster 2."
            clusterMaps: [
                Map_tst2 {},
                Map_tst3 {}
            ]

            enemyUnitsList: [
                Tank_tst2 { unitSide: "bad" },
                Tank_tst2 { unitSide: "bad" },
                Tank_tst2 { unitSide: "bad" }
            ]
        }
    ]
}
