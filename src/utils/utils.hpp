#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace modUtils {
    inline bool isInVector<typename T>(std::vector<T> vec, T toFind) {
        return std::find(vec.begin(), vec.end(), toFind) != vec.end();
    }

    /**
     * Takes in a CCPoint and convert into a gd grid pos
     * @param point the said point to be converted
     * @param onGrid if the returned coordinates are on gd's grid, if not set to `true` it will be in the center of the grid
     * @param scaleMod the scale multiplier
     */
    inline CCPoint coordinatesToGDgridPos(CCPoint point, bool onGrid = true, float scaleMod = 1) {
        CCPoint newPoint;

        newPoint.x = coordinateToGDgridPos(point.x, onGrid, scaleMod);
        newPoint.y = coordinateToGDgridPos(point.y, onGrid, scaleMod);

        return newPoint;
    }

    /**
     * Takes in a coordinate and convert into a gd grid pos
     * @param point the said coordinate to be converted
     * @param onGrid if the returned coordinates are on gd's grid, if not set to `true` it will be in the center of the grid
     * @param scaleMod the scale multiplier
     */
    inline constexpr double coordinateToGDgridPos(double coord, bool onGrid = true, float scaleMod = 1) {
        if (onGrid) {
            return (15 + coord*30) * scaleMod;
        }
        return coord*30 * scaleMod;
    }
}