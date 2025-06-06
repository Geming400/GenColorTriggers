#include "parser.hpp"

#include "../utils/customColors.hpp"

#define LVL_COLOR_CHANNELS "kS38"

#define MAP_HAS_COLOR_PROPERTY(map, property) map.contains(fmt::to_string((int) property))
#define PROPERTY_TO_STRING(property) static_cast<std::string>(fmt::to_string((int) property))

// TO_<TYPE>
#define PROPERTY_VALUE_TO_STRING(map, property) map[PROPERTY_TO_STRING(property)]
#define PROPERTY_VALUE_TO_INT(map, property) std::stoi(map[PROPERTY_TO_STRING(property)])
#define PROPERTY_VALUE_TO_FLOAT(map, property) std::stof(map[PROPERTY_TO_STRING(property)])
// #define PROPERTY_VALUE_TO_BOOL(map, property) PROPERTY_VALUE_TO_INT(map, property) == 1
#define PROPERTY_VALUE_TO_BOOL(map, property) (bool) PROPERTY_VALUE_TO_INT(map, property)
#define PROPERTY_VALUE_TO_HSV(map, property) colorChannelsParser::rawHSVtoHSVValue(PROPERTY_VALUE_TO_STRING(map, property))

#define MAP_PROPERTY_VALUE_IS_VALID(map, property) !(PROPERTY_VALUE_TO_STRING(map, property) == "")
#define MAP_PROPERTY_VALUE_CHECK(map, property) MAP_HAS_COLOR_PROPERTY(map, property) && MAP_PROPERTY_VALUE_IS_VALID(map, property)

ccHSVValue colorChannelsParser::rawHSVtoHSVValue(std::string& rawHSVString) {
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
    float saturation = std::stof(HSVProperties[1]);
    float brightness = std::stof(HSVProperties[2]);
    bool isSaturationAdditive = std::stoi(HSVProperties[3]) == 1;
    bool isBrightnessAdditive = std::stoi(HSVProperties[4]) == 1;

    hsv.h = hue;
    hsv.s = saturation;
    hsv.v = brightness;
    hsv.absoluteSaturation = isSaturationAdditive;
    hsv.absoluteBrightness = isBrightnessAdditive;

    return hsv;
}

// This function looks ugly but whatever
std::optional<modUtils::ColorTriggerContent> colorChannelsParser::rawColorChannelToColorTrigger(std::string& rawColorChannelString) {
    ColorAction* colorAction = ColorAction::create();
    colorAction->retain();
    int targetChannelID;

    modUtils::ColorTriggerContent colorTriggerContent;

    std::map<std::string, std::string> colorProperties = modUtils::stringDictToMap(rawColorChannelString, "_");

    // The color channel ID is like needed for every color channel strings, so that's why were looking for that
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
        float fromOpacity = PROPERTY_VALUE_TO_FLOAT(colorProperties, ColorString::FROM_OPACITY);
        colorAction->m_fromOpacity = fromOpacity;
        colorAction->m_currentOpacity = fromOpacity;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::INHERITED_COLOR_CHANNEL)) {
        int copyID = PROPERTY_VALUE_TO_INT(colorProperties, ColorString::INHERITED_COLOR_CHANNEL);
        colorAction->m_copyID = copyID;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::HSV)) {
        ccHSVValue hsv = PROPERTY_VALUE_TO_HSV(colorProperties, ColorString::HSV);
        colorAction->m_copyHSV = hsv;
    }
    if (MAP_PROPERTY_VALUE_CHECK(colorProperties, ColorString::COPY_OPACITY)) {
        bool copyOpacity = PROPERTY_VALUE_TO_BOOL(colorProperties, ColorString::COPY_OPACITY);
        colorAction->m_copyOpacity = copyOpacity;
    }

    colorTriggerContent.colorAction = colorAction;

    return colorTriggerContent;
}


std::optional<std::vector<modUtils::ColorTriggerContent>> colorChannelsParser::getColorChannelsFromLevelString(const std::string& levelString) {
    std::vector<modUtils::ColorTriggerContent> out;

    std::string levelStart = levelString.substr(0, levelString.find(";", 0));
    std::string splittedLevelStart = geode::utils::string::split(levelStart, ";")[0];
    std::map<std::string, std::string> colorChannels = modUtils::stringDictToMap(splittedLevelStart, ",");

    if (colorChannels.contains(LVL_COLOR_CHANNELS)) {
        std::string colorChannelsProperties = colorChannels[LVL_COLOR_CHANNELS];

        int i = 0;
        for (auto &&rawColorChannel : geode::utils::string::split(colorChannelsProperties, "|"))
        {
            auto colorTrigger = rawColorChannelToColorTrigger(rawColorChannel);
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

void colorChannelsParser::colorTriggerContentToColorTrigger(EffectGameObject* colorTrigger, modUtils::ColorTriggerContent triggerContent, const bool useLegacyHSV) {
    ColorAction* colorAction = triggerContent.colorAction;
    int colorChannelID = triggerContent.targetChannelID;

    colorTrigger->m_targetColor = colorChannelID;
    colorTrigger->m_triggerTargetColor = colorAction->m_color;
    colorTrigger->m_usesBlending = colorAction->m_blending;
    colorTrigger->m_usesPlayerColor1 = colorAction->m_playerColor == 1;
    colorTrigger->m_usesPlayerColor2 = colorAction->m_playerColor == 2;
    colorTrigger->m_copyColorID = colorAction->m_copyID;
    colorTrigger->m_hsvValue = colorAction->m_copyHSV;
    colorTrigger->m_copyOpacity = colorAction->m_copyOpacity;
    colorTrigger->m_opacity = colorAction->m_fromOpacity;
    colorTrigger->m_duration = 0;
    colorTrigger->m_legacyHSV = useLegacyHSV;

    auto label = colorTrigger->getChildByType<CCLabelBMFont>(0);

    std::string channelIDstring = fmt::to_string(colorChannelID);
    if (customColorNames.contains(colorChannelID)) {
        label->setString(customColorNames.at(colorChannelID).c_str());
    } else {
        label->setString(channelIDstring.c_str());
    }

    colorAction->release(); // We don't need it anymore
}