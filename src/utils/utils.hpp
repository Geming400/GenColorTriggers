#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;


#ifdef GEODE_IS_DESKTOP
#define CAN_USE_CUSTOM_KEYBINDS
#endif

namespace modUtils {
    struct ColorTriggerContent {
        ColorAction* colorAction;
        int targetChannelID;
    };

    template<typename T>
    inline bool isInVector(std::vector<T> vec, T toFind) {
        return std::find(vec.begin(), vec.end(), toFind) != vec.end();
    }

    /**
     * Takes in a coordinate and convert into a gd grid pos
     * @param point the said coordinate to be converted
     * @param onGrid if the returned coordinates are on gd's grid, if not set to `true` it will be in the center of the grid
     * @param scaleMod the scale multiplier
     */
    double coordinateToGDgridPos(double coord, bool onGrid = true, float scaleMod = 1.0f);

    /**
     * Takes in a CCPoint and convert into a gd grid pos
     * @param point the said point to be converted
     * @param onGrid if the returned coordinates are on gd's grid, if not set to `true` it will be in the center of the grid
     * @param scaleMod the scale multiplier
     */
    CCPoint coordinatesToGDgridPos(CCPoint point, bool onGrid = true, float scaleMod = 1.0f);

    std::map<std::string, std::string> stringDictToMap(std::string& stringDict, std::string sep);
}