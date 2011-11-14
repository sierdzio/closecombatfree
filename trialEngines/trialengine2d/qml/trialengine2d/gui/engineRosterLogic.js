var unitsList;

function colorForStatus(statusMessage) {
    var result = "ERROR";

    if (statusMessage == "READY") {
        result = "#115511";
    } else if (statusMessage == "MOVING") {
        result = "#118811";
    } else if (statusMessage == "MOVING FAST") {
        result = "#118811";
    } else if (statusMessage == "SNEAKING") {
        result = "#118811";
    } else if (statusMessage == "AMBUSHING") {
        result = "#888888";
    } else if (statusMessage == "DEFENDING") {
        result = "#888888";
    } else if (statusMessage == "KIA") {
        result = "#000000";
    } else if (statusMessage == "DAMAGED") {
        result = "#aa1111";
    } else if (statusMessage == "WOUNDED") {
        result = "#555511";
    } else if (statusMessage == "INCAPACITATED") {
        result = "#885511";
    } else if (statusMessage == "ROTATING") {
        result = "#118811";
    } else if (statusMessage == "FIRING") {
        result = "#881111";
    }

    return result;
}
