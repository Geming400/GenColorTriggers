#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

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

class ColorTriggerGenUI : public geode::Popup<GeneratorOptions> {
    GeneratorOptions m_options;

    // Left side

    Ref<CCLabelBMFont> m_generatorSectionLabel;
    Ref<CCLayerColor> m_leftSideSeparator;

    Ref<CCMenuItemToggler> m_useGdGridSpaceCheckbox;
    Ref<CCLabelBMFont> m_useGdGridSpaceText;

    // Float text input
    Ref<TextInput> m_offsetXfield;
    // Float text input
    Ref<TextInput> m_offsetYfield;

    // Right side

    Ref<CCLabelBMFont> m_parserSectionLabel;
    Ref<CCLayerColor> m_rightSideSeparator;

    Ref<CCMenuItemToggler> m_generateForSelectedObjectsCheckbox;
    Ref<CCLabelBMFont> m_generateForSelectedObjectsText;

    Ref<CCMenuItemToggler> m_parseBuiltinColorChannelsCheckbox;
    Ref<CCLabelBMFont> m_parseBuiltinColorChannelsText;

    // Bottom

    Ref<CCMenuItemSpriteExtra> m_generateButton;


    void createLeftSide();
    void createRightSide();
    void createBottom();

    CCMenuItemSpriteExtra* createInfoButton();
    CCLayerColor* createSeparator();
    CCLayerColor* createSeparator(CCPoint pos, float width = 150.0f, float height = 2.0f);

protected:
    bool setup(GeneratorOptions value) override;

public:
    static ColorTriggerGenUI* create(GeneratorOptions options = GeneratorOptions::fromSettingValues());
};