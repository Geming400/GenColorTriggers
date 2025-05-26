#include "utils.hpp"

double modUtils::coordinateToGDgridPos(double coord, bool onGrid, float scaleMod) {
    if (onGrid) {
        return (15 + coord*30) * scaleMod;
    }
    return coord*30 * scaleMod;
}

CCPoint modUtils::coordinatesToGDgridPos(CCPoint point, bool onGrid, float scaleMod) {
    CCPoint newPoint;

    newPoint.x = coordinateToGDgridPos(point.x, onGrid, scaleMod);
    newPoint.y = coordinateToGDgridPos(point.y, onGrid, scaleMod);

    return newPoint;
}

std::map<std::string, std::string> modUtils::stringDictToMap(std::string& stringDict, std::string sep) {
    std::map<std::string, std::string> dict;

    std::vector<std::string> splittedDict = geode::utils::string::split(stringDict, sep);

    int i = -1;
    for (auto &&e : splittedDict) {
        i += 1;

        if (i % 2 == 0) {    // is odd
            dict[e] = splittedDict[i + 1];
        } else { continue; } // is even
    }
    
    return dict;
}