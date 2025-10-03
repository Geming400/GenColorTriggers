#pragma once

#include <Geode/Geode.hpp>
#include "../utils/utils.hpp"
#include "../ui/colorTriggerGeneratorUI.hpp"

using namespace geode::prelude;

constexpr int vectorSizePushLimit = 1050; // idk just in case very weird stuff happens

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    /**
     * Generate color triggers for each channels
     * @return the number of generated color triggers
     */
    size_t genColorTriggers(GameObject* center, CCPoint offset, GeneratorOptions options);

    std::vector<modUtils::ColorTriggerContent> getGeneratableColorChannels(std::vector<modUtils::ColorTriggerContent> colorChannels, bool includeBuiltInColorChannels);
    std::vector<modUtils::ColorTriggerContent> getGeneratableColorChannels(std::vector<modUtils::ColorTriggerContent> colorChannels, GeneratorOptions options);
};