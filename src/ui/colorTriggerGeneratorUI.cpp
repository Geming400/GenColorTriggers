#include "colorTriggerGeneratorUI.hpp"

#include "../hooks/LevelEditorLayer.hpp"
#include <Geode/ui/GeodeUI.hpp>

float constexpr borderUiSize = 3.5f;

float constexpr sectionSize = 0.8f;
float constexpr checkboxSize = 0.8f;
float constexpr checkboxTextSize = 0.4f;

const std::string generateForSelectedObjectsInfoButton_description = fmt::format(
    "This setting is used to generate color triggers for a selection of object.\n"
    "\n"
    "Once you'll click on the `Generate` button, you will need to **select some objects**, press the `F10` keybind (if you modified it, press the keybind you chose).\n"
    "And it'll generate color triggers for your selection of objects !"
);

bool ColorTriggerGenUI::setup(GeneratorOptions value, std::function<void(const GeneratorOptions)> callback) {
    this->setTitle("Color trigger generator options");
    this->setID(POPUP_ID);

    m_options = value;
    m_callback = callback;

    createLeftSide();
    createRightSide();
    createBottom();

    return true;
}

ColorTriggerGenUI* ColorTriggerGenUI::create(GeneratorOptions options, std::function<void(const GeneratorOptions)> callback) {
    auto ret = new ColorTriggerGenUI();
    if (ret->initAnchored(500.0f, 240.0f, options, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void ColorTriggerGenUI::createLeftSide() {
    m_generatorSectionLabel = CCLabelBMFont::create("Generator", "bigFont.fnt");
    m_generatorSectionLabel->setPosition({ 100, 190 });
    m_generatorSectionLabel->setScale(sectionSize);
    m_mainLayer->addChild(m_generatorSectionLabel);

    m_leftSideSeparator = createSeparator({ 25, 175 });
    m_mainLayer->addChild(m_leftSideSeparator);

    // m_useGdGridSpaceCheckbox

    m_useGdGridSpaceCheckbox = CCMenuItemExt::createTogglerWithStandardSprites(checkboxSize, [this](CCMenuItemToggler* obj) {
        m_options.m_useGdGridSpace = !obj->isOn(); // idk, when it's toggled on, `obj->isOn()` returns false
    });
    m_useGdGridSpaceCheckbox->setPosition({ 20, 150 });
    m_useGdGridSpaceCheckbox->toggle(m_options.m_useGdGridSpace);
    m_useGdGridSpaceCheckbox->setID("use-gd-grid-space-checkbox");
    m_buttonMenu->addChild(m_useGdGridSpaceCheckbox);

    // m_useGdGridSpaceText

    m_useGdGridSpaceText = CCLabelBMFont::create("Use GD's grid space", "bigFont.fnt");
    m_useGdGridSpaceText->setPosition({ 110, 150 });
    m_useGdGridSpaceText->setScale(checkboxTextSize);
    m_useGdGridSpaceText->setID("use-gd-grid-space-text");
    m_mainLayer->addChild(m_useGdGridSpaceText);

    // m_useGdGridSpaceInfoButton

    m_useGdGridSpaceInfoButton = createInfoButtonFromSetting("use-gd-grid-space");
    m_useGdGridSpaceInfoButton->setID("use-gd-grid-space-info-button");
    m_useGdGridSpaceInfoButton->setPosition({ 185, 155 });
    m_buttonMenu->addChild(m_useGdGridSpaceInfoButton);


    // m_offsetXinput

    m_offsetXinput = TextInput::create(100.0f, "Offset X");
    m_offsetXinput->setPosition({ 58, 110 });
    m_offsetXinput->setCommonFilter(CommonFilter::Float);
    m_offsetXinput->setCallback([this](std::string string) {
        auto toFloat = utils::numFromString<float>(string);
        if (toFloat.isOk()) {
            m_options.m_offsetX = *toFloat;
        }
        else log::warn("User entered a non-float like input: {}\nError: {}", string, toFloat.unwrapErr());
    });
    m_offsetXinput->setID("offset-x-input");
    m_offsetXinput->setLabel("Offset x");
    m_offsetXinput->setString(
        fmt::to_string((float) Mod::get()->getSettingValue<double>("offset-x")), // No need to use static_cast with double to float
        true
    );
    m_mainLayer->addChild(m_offsetXinput);

    // m_offsetYinput

    m_offsetYinput = TextInput::create(100.0f, "Offset Y");
    m_offsetYinput->setPosition({ 58, 65 });
    m_offsetYinput->setCommonFilter(CommonFilter::Float);
    m_offsetYinput->setCallback([this](std::string string) {
        auto toFloat = utils::numFromString<float>(string);
        if (toFloat.isOk()) {
            m_options.m_offsetY = *toFloat;
        }
        else log::warn("User entered a non-float like input: {}\nError: {}", string, toFloat.unwrapErr());
    });
    m_offsetYinput->setID("offset-y-input");
    m_offsetYinput->setLabel("Offset y");
    m_offsetYinput->setString(
        fmt::to_string((float) Mod::get()->getSettingValue<double>("offset-y")), // No need to use static_cast with double to float
        true
    );
    m_mainLayer->addChild(m_offsetYinput);


    // m_moreLabel

    m_moreLabel = CCLabelBMFont::create("More :", "bigFont.fnt");
    m_moreLabel->setPosition({ 50, 30 });
    m_moreLabel->setScale(.75f);
    m_moreLabel->setID("more-label");
    m_mainLayer->addChild(m_moreLabel);

    // m_moreButton

    constexpr float scale = .7f;

    const auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    m_moreButton = CCMenuItemExt::createSpriteExtra(btnSprite, [](CCMenuItemSpriteExtra* button) {
        openSettingsPopup(Mod::get()); // from <Geode/ui/GeodeUI.hpp>
    });
    m_moreButton->setPosition({ 117, 30 });

    m_moreButton->setScale(scale);
    m_moreButton->m_baseScale = scale;
    m_moreButton->setID("more-button");
    m_buttonMenu->addChild(m_moreButton);
};

void ColorTriggerGenUI::createRightSide() {
    m_parserSectionLabel = CCLabelBMFont::create("Parser", "bigFont.fnt");
    m_parserSectionLabel->setPosition({ 400, 190 });
    m_parserSectionLabel->setScale(sectionSize);
    m_mainLayer->addChild(m_parserSectionLabel);

    m_rightSideSeparator = createSeparator({ 325, 175 });
    m_mainLayer->addChild(m_rightSideSeparator);

    // m_generateForSelectedObjectsCheckbox

    m_generateForSelectedObjectsCheckbox = CCMenuItemExt::createTogglerWithStandardSprites(checkboxSize, [this](CCMenuItemToggler* obj) {
        m_options.m_genForSelectedObjects = !obj->isOn();
    });
    m_generateForSelectedObjectsCheckbox->setPosition({ 335, 150 });
    m_generateForSelectedObjectsCheckbox->toggle(m_options.m_genForSelectedObjects);
    m_generateForSelectedObjectsCheckbox->setID("generate-for-selected-objects-checkbox");
    m_buttonMenu->addChild(m_generateForSelectedObjectsCheckbox);

    // m_generateForSelectedObjectsText

    m_generateForSelectedObjectsText = CCLabelBMFont::create("Generate only for\nselected objects", "bigFont.fnt");
    m_generateForSelectedObjectsText->setPosition({ 420, 150 });
    m_generateForSelectedObjectsText->setScale(checkboxTextSize);
    m_generateForSelectedObjectsText->setID("generate-for-selected-objects-text");
    m_mainLayer->addChild(m_generateForSelectedObjectsText);

    // m_generateForSelectedObjectsInfoButton

    m_generateForSelectedObjectsInfoButton = createInfoButton(
        "Generate only for selected objects",
        generateForSelectedObjectsInfoButton_description
    );
    m_generateForSelectedObjectsInfoButton->setID("generate-for-selected-objects-info-button");
    m_generateForSelectedObjectsInfoButton->setPosition({ 490, 160 });
    m_buttonMenu->addChild(m_generateForSelectedObjectsInfoButton);

    // m_parseBuiltinColorChannelsCheckbox

    m_parseBuiltinColorChannelsCheckbox = CCMenuItemExt::createTogglerWithStandardSprites(checkboxSize, [this](CCMenuItemToggler* obj) {
        m_options.m_parseBuiltinColorChannels = !obj->isOn();
    });
    m_parseBuiltinColorChannelsCheckbox->setPosition({ 335, 110 });
    m_parseBuiltinColorChannelsCheckbox->toggle(m_options.m_parseBuiltinColorChannels);
    m_parseBuiltinColorChannelsCheckbox->setID("parse-builtin-color-channels-checkbox");
    m_buttonMenu->addChild(m_parseBuiltinColorChannelsCheckbox);


    // m_parseBuiltinColorChannelsText

    m_parseBuiltinColorChannelsText = CCLabelBMFont::create("Parse built-in\ncolor channels", "bigFont.fnt");
    m_parseBuiltinColorChannelsText->setPosition({ 411, 110 });
    m_parseBuiltinColorChannelsText->setScale(checkboxTextSize);
    m_parseBuiltinColorChannelsText->setID("parse-builtin-color-channels-text");
    m_mainLayer->addChild(m_parseBuiltinColorChannelsText);

    // m_parseBuiltinColorChannelsInfoButton

    m_parseBuiltinColorChannelsInfoButton = createInfoButtonFromSetting("include-builtin-color-channels");
    m_parseBuiltinColorChannelsInfoButton->setID("parse-builtin-color-channels-info-button");
    m_parseBuiltinColorChannelsInfoButton->setPosition({ 470, 120 });
    m_buttonMenu->addChild(m_parseBuiltinColorChannelsInfoButton);
};

void ColorTriggerGenUI::createBottom() {
    m_generateButton = CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create("Generate", "goldFont.fnt", "GJ_button_04.png", .8f),
        [this](CCMenuItemSpriteExtra*) {
            m_callback(m_options);

            this->removeMeAndCleanup();
        }
    );

    m_generateButton->setPosition({ 250, 25 });
    m_generateButton->setID("generate-button");
    m_buttonMenu->addChild(m_generateButton);
}

CCMenuItemSpriteExtra* ColorTriggerGenUI::createInfoButton(std::string title, std::string content) {
    auto sprite = CCMenuItemExt::createSpriteExtra(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
    [title, content](CCMenuItemSpriteExtra* infoButton) {
        MDPopup::create(
        true,
        title,
        content,
        "OK"
    )->show();
    });

    constexpr float scale = 0.5f;
    sprite->m_baseScale = scale; // If we don't do that then the button will scale back up to 1.f when clicked
    sprite->setScale(scale);
    return sprite;
};

CCMenuItemSpriteExtra* ColorTriggerGenUI::createInfoButtonFromSetting(std::string title, std::string_view settingID) {
    return createInfoButton(title, Mod::get()->getSetting(settingID)->getDescription().value_or("# No description."));
};

CCMenuItemSpriteExtra* ColorTriggerGenUI::createInfoButtonFromSetting(std::string_view settingID) {
    return createInfoButtonFromSetting(Mod::get()->getSetting(settingID)->getDisplayName(), settingID);
};

CCLayerColor* ColorTriggerGenUI::createSeparator() {
    return createSeparator({ 0, 0 });
}

CCLayerColor* ColorTriggerGenUI::createSeparator(CCPoint pos, float width, float height) {
    auto layer = CCLayerColor::create(ccc4(129, 71, 35, (int) 255*3/4), width, height);
    layer->setPosition({ pos.x, pos.y });

    return layer;
}

GeneratorOptions ColorTriggerGenUI::getOptions() { return m_options; }

void ColorTriggerGenUI::setCallback(std::function<void(const GeneratorOptions)> callback) {
    m_callback = callback;
};