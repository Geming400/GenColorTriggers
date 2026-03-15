#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;


/**
 * A struct that defines which color trigger to generate, or where to generate them
 */
struct GeneratorOptions {
    // Generator

    bool smallSteps;
    float offsetX;
    float offsetY;

    // Parser

    bool genForSelectedObjects;
    bool parseBuiltinColorChannels;


    static GeneratorOptions fromSettingValues() {
        GeneratorOptions options;
        Mod* mod = Mod::get();

        options.smallSteps = mod->getSettingValue<bool>("use-gd-grid-space");
        options.offsetX = (float) mod->getSettingValue<double>("offset-x");
        options.offsetY = (float) mod->getSettingValue<double>("offset-y");

        options.genForSelectedObjects = false;
        options.parseBuiltinColorChannels = mod->getSettingValue<bool>("include-builtin-color-channels");
        
        return options;
    }
};