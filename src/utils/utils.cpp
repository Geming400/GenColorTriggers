#include "utils.hpp"
#include "objectProperties.hpp"
#include "customColors.hpp"

#define LVL_COLOR_CHANNELS "kS38"

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

#define MAP_HAS_COLOR_PROPERTY(map, property) map.contains(fmt::to_string((int) property))
#define PROPERTY_TO_STRING(property) static_cast<std::string>(fmt::to_string((int) property))

// TO_<TYPE>
#define PROPERTY_VALUE_TO_STRING(map, property) map[PROPERTY_TO_STRING(property)]
#define PROPERTY_VALUE_TO_INT(map, property) std::stoi(map[PROPERTY_TO_STRING(property)])
#define PROPERTY_VALUE_TO_FLOAT(map, property) std::stof(map[PROPERTY_TO_STRING(property)])
// #define PROPERTY_VALUE_TO_BOOL(map, property) PROPERTY_VALUE_TO_INT(map, property) == 1
#define PROPERTY_VALUE_TO_BOOL(map, property) (bool) PROPERTY_VALUE_TO_INT(map, property)
#define PROPERTY_VALUE_TO_HSV(map, property) modUtils::colorChannelsParser::rawHSVtoHSVValue(PROPERTY_VALUE_TO_STRING(map, property))

#define MAP_PROPERTY_VALUE_IS_VALID(map, property) !(PROPERTY_VALUE_TO_STRING(map, property) == "")
#define MAP_PROPERTY_VALUE_CHECK(map, property) MAP_HAS_COLOR_PROPERTY(map, property) && MAP_PROPERTY_VALUE_IS_VALID(map, property)

ccHSVValue modUtils::colorChannelsParser::rawHSVtoHSVValue(std::string& rawHSVString) {
    // HSV format:
    // 33a0.8a0.6a0a0
    // 33 = Hue                (int)   [0]
    // 0.8 = Saturation        (float) [1]
    // 0.6 = Brightness        (float) [2]
    // 0 = Additive saturation (bool)  [3]
    // 0 = Additive brightness (bool)  [4]
    //
    // separator = "a" (robtop was so lazy :sob:)

    std::vector<std::string> HSVProperties = geode::utils::string::split(rawHSVString, "a");

    ccHSVValue hsv;

    int hue = std::stoi(HSVProperties[0]);
    float saturation = std::stof(HSVProperties[2]);
    float brightness = std::stof(HSVProperties[3]);
    bool isSaturationAdditive = std::stoi(HSVProperties[4]) == 1;
    bool isBrightnessAdditive = std::stoi(HSVProperties[5]) == 1;

    hsv.h = hue;
    hsv.s = saturation;
    hsv.v = brightness;
    hsv.absoluteSaturation = isSaturationAdditive;
    hsv.absoluteBrightness = isBrightnessAdditive;

    return hsv;
}

// This function looks ugly but whatever
std::optional<modUtils::ColorTriggerContent> modUtils::colorChannelsParser::rawColorToColorTrigger(std::string& rawColorString) {
    ColorAction* colorAction = ColorAction::create();
    colorAction->retain();
    int targetChannelID;

    ColorTriggerContent colorTriggerContent;

    std::map<std::string, std::string> colorProperties = stringDictToMap(rawColorString, "_");

    // The color channel ID like needed for every color channels, so that's why were looking for that
    if (!(MAP_HAS_COLOR_PROPERTY(colorProperties, ColorString::COLOR_CHANNEL) && MAP_PROPERTY_VALUE_IS_VALID(colorProperties, ColorString::COLOR_CHANNEL))) {
        log::debug("A color channel was parsed with no useful value found");
        return std::nullopt;
    }

    // m_fromColor

    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::FROM_RED)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::FROM_RED);
        colorAction->m_fromColor.r = color;
        colorAction->m_color.r = color;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::FROM_GREEN)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::FROM_GREEN);
        colorAction->m_fromColor.g = color;
        colorAction->m_color.g = color;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::FROM_BLUE)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::FROM_BLUE);
        colorAction->m_fromColor.b = color;
        colorAction->m_color.b = color;
    }

    // m_toColor

    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::TO_RED)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::TO_RED);
        colorAction->m_toColor.r = color;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::TO_GREEN)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::TO_GREEN);
        colorAction->m_toColor.g = color;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::TO_BLUE)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::TO_BLUE);
        colorAction->m_toColor.b = color;
    }

    // others

    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::PLAYER_COLOR)) {
        int color = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::PLAYER_COLOR);
        colorAction->m_playerColor = color;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::BLENDING)) {
        bool blending = PROPERTY_VALUE_TO_BOOL(colorProperties, ColorString::BLENDING);
        colorAction->m_blending = blending;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::COLOR_CHANNEL)) {
        int colorChannel = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::COLOR_CHANNEL);
        colorTriggerContent.targetChannelID = colorChannel;
    } 
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::FROM_OPACITY)) {
        bool fromOpacity = PROPERTY_VALUE_TO_FLOAT(colorProperties, ColorString::FROM_OPACITY);
        colorAction->m_fromOpacity = fromOpacity;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::INHERITED_COLOR_CHANNEL)) {
        int copyID = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::INHERITED_COLOR_CHANNEL);
        colorAction->m_copyID = copyID;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::HSV)) {
        ccHSVValue hsv = PROPERTY_VALUE_TO_HSV(colorProperties, ColorString::HSV);
        colorAction->m_copyHSV = hsv;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::DELTA_TIME)) {
        float deltaTime = PROPERTY_VALUE_TO_FLOAT(colorProperties, ColorString::DELTA_TIME);
        colorAction->m_deltaTime = deltaTime;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::TO_OPACITY)) {
        float toOpacity = PROPERTY_VALUE_TO_FLOAT(colorProperties, ColorString::TO_OPACITY);
        colorAction->m_toOpacity = toOpacity;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::DURATION)) {
        float duration = PROPERTY_VALUE_TO_FLOAT(colorProperties, ColorString::DURATION);
        colorAction->m_duration = duration;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::COPY_OPACITY)) {
        bool copyOpacity = PROPERTY_VALUE_TO_BOOL(colorProperties, ColorString::COPY_OPACITY);
        colorAction->m_copyOpacity = copyOpacity;
    }

    colorTriggerContent.colorAction = colorAction;

    return colorTriggerContent;
}

std::optional<std::vector<modUtils::ColorTriggerContent>> modUtils::colorChannelsParser::getColorChannelsFromLevelString(std::string& levelString) {
    std::vector<modUtils::ColorTriggerContent> out;

    std::string levelStart = geode::utils::string::split(levelString, ";")[0];
    std::map<std::string, std::string> colorChannels = stringDictToMap(levelStart, ",");

    if (colorChannels.contains(LVL_COLOR_CHANNELS)) {
        std::string colorChannelsProperties = colorChannels[LVL_COLOR_CHANNELS];

        int i = 0;
        for (auto &&rawColorChannel : geode::utils::string::split(colorChannelsProperties, "|"))
        {
            auto colorTrigger = rawColorToColorTrigger(rawColorChannel);
            if (colorTrigger) {
                out.push_back(colorTrigger.value());
            }
            i++;
        }
        
        return out;
    } else {
        return std::nullopt; // This shouldn't normally happen, but I added this just in case
    }
}

void modUtils::colorChannelsParser::colorTriggerContentToColorTrigger(EffectGameObject* colorTrigger, modUtils::ColorTriggerContent triggerContent) {
    ColorAction* colorAction = triggerContent.colorAction;
    int colorChannelID = triggerContent.targetChannelID;

    colorTrigger->m_targetColor = colorChannelID;
    colorTrigger->m_triggerTargetColor = colorAction->m_color;
    colorTrigger->m_usesBlending = colorAction->m_blending;
    colorTrigger->m_usesPlayerColor1 = colorAction->m_playerColor == 1;
    colorTrigger->m_usesPlayerColor1 = colorAction->m_playerColor == 2;
    colorTrigger->m_copyColorID = colorAction->m_copyID;
    colorTrigger->m_hsvValue = colorAction->m_copyHSV;
    colorTrigger->m_legacyHSV = true; // idk it's true by default
    colorTrigger->m_copyOpacity = colorAction->m_copyOpacity;
    colorTrigger->m_duration = 0;

    auto label = colorTrigger->getChildByType<CCLabelBMFont>(0);

    std::string channelIDstring = fmt::to_string(colorChannelID);
    if (customColorNames.contains(colorChannelID)) {
        label->setString(customColorNames.at(colorChannelID).c_str());
    } else {
        label->setString(channelIDstring.c_str());
    }

    colorAction->release(); // We don't need it anymore
}