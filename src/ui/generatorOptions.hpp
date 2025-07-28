#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;


/**
 * A struct that defines which color trigger to generate, or where to generate them
 */
struct GeneratorOptions {
    // Generator

    bool m_useGdGridSpace;
    float m_offsetX;
    float m_offsetY;

    // Parser

    bool m_genForSelectedObjects;
    bool m_parseBuiltinColorChannels;


    static GeneratorOptions fromSettingValues() {
        GeneratorOptions options;
        Mod* mod = Mod::get();

        options.m_useGdGridSpace = mod->getSettingValue<bool>("use-gd-grid-space");
        options.m_offsetX = (float) mod->getSettingValue<double>("offset-x");
        options.m_offsetY = (float) mod->getSettingValue<double>("offset-y");

        options.m_genForSelectedObjects = false;
        options.m_parseBuiltinColorChannels = mod->getSettingValue<bool>("include-builtin-color-channels");
        
        return options;
    }
};