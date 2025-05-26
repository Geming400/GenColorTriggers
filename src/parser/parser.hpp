#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "../utils/utils.hpp"
#include "../utils/objectProperties.hpp"

namespace colorChannelsParser {
    ccHSVValue rawHSVtoHSVValue(std::string& rawHSVString);

    std::optional<modUtils::ColorTriggerContent> rawColorToColorTrigger(std::string& rawColorString);
    std::optional<std::vector<modUtils::ColorTriggerContent>> getColorChannelsFromLevelString(std::string& levelString);

    /**
     * Uses a `ColorTriggerContent` struct given from `rawColorToColorTrigger()` to change to property of a given `EffectGameObject*` according to the struct
     * @param colorTrigger the `EffectGameObject` which properties will be changed
     * @param triggerContent the `TriggerContent` struct used for changing the `EffectGameObject` instance's members
     */
    void colorTriggerContentToColorTrigger(EffectGameObject* colorTrigger, modUtils::ColorTriggerContent triggerContent);
}