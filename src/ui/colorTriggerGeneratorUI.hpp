#pragma once

#include <Geode/Geode.hpp>

#include "../hooks/EditorUI.hpp"
#include "generatorOptions.hpp"

using namespace geode::prelude;


class ColorTriggerGenUI : public geode::Popup<GeneratorOptions, std::function<void(const GeneratorOptions)>> {
    GeneratorOptions m_options;
    std::function<void(const GeneratorOptions)> m_callback;

    // Left side

    Ref<CCLabelBMFont> m_generatorSectionLabel;
    Ref<CCLayerColor> m_leftSideSeparator;

    Ref<CCMenuItemToggler> m_useGdGridSpaceCheckbox;
    Ref<CCLabelBMFont> m_useGdGridSpaceText;
    Ref<CCMenuItemSpriteExtra> m_useGdGridSpaceInfoButton;

    // Float text inputs
    Ref<TextInput> m_offsetXinput;
    Ref<TextInput> m_offsetYinput;

    // Right side

    Ref<CCLabelBMFont> m_parserSectionLabel;
    Ref<CCLayerColor> m_rightSideSeparator;

    Ref<CCMenuItemToggler> m_generateForSelectedObjectsCheckbox;
    Ref<CCLabelBMFont> m_generateForSelectedObjectsText;
    Ref<CCMenuItemSpriteExtra> m_generateForSelectedObjectsInfoButton;

    Ref<CCMenuItemToggler> m_parseBuiltinColorChannelsCheckbox;
    Ref<CCLabelBMFont> m_parseBuiltinColorChannelsText;
    Ref<CCMenuItemSpriteExtra> m_parseBuiltinColorChannelsInfoButton;

    Ref<CCLabelBMFont> m_moreLabel;
    Ref<CCMenuItemSpriteExtra> m_moreButton;

    // Bottom

    Ref<CCMenuItemSpriteExtra> m_generateButton;


    void createLeftSide();
    void createRightSide();
    void createBottom();

    CCMenuItemSpriteExtra* createInfoButton(std::string title, std::string content);
    CCMenuItemSpriteExtra* createInfoButtonFromSetting(std::string title, std::string_view settingID);
    CCMenuItemSpriteExtra* createInfoButtonFromSetting(std::string_view settingID);
    CCLayerColor* createSeparator();
    CCLayerColor* createSeparator(CCPoint pos, float width = 150.0f, float height = 2.0f);

protected:
    bool setup(GeneratorOptions options, std::function<void(const GeneratorOptions)>) override;

public:
    static ColorTriggerGenUI* create(
        GeneratorOptions options,
        std::function<void(const GeneratorOptions)> callback
    );

    static constexpr auto POPUP_ID = "colorTriggerGenPopup"_spr;

    GeneratorOptions getOptions();

    void setCallback(std::function<void(const GeneratorOptions)> callback);
};