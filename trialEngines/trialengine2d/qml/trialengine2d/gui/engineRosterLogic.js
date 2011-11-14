var unitsList;

function colorForStatus(statusMessage) {
    var result = "ERROR";

    if (statusMessage == "READY") {
        result = "#115511";
    } else if (statusMessage == "MOVING") {
        result = "#11aa11";
    } else if (statusMessage == "MOVING FAST") {
        result = "#11aa11";
    } else if (statusMessage == "SNEAKING") {
        result = "#11aa11";
    } else if (statusMessage == "AMBUSHING") {
        result = "#aaaaaa";
    } else if (statusMessage == "DEFENDING") {
        result = "#aaaaaa";
    } else if (statusMessage == "KIA") {
        result = "#000000";
    } else if (statusMessage == "DAMAGED") {
        result = "#aa1111";
    } else if (statusMessage == "WOUNDED") {
        result = "#555511";
    } else if (statusMessage == "INCAPACITATED") {
        result = "#885511";
    } else if (statusMessage == "ROTATING") {
        result = "#11aa11";
    } else if (statusMessage == "FIRING") {
        result = "#aa1111";
    }

    return result;
}
