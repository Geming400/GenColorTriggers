#include "colorTriggerGeneratorUI.hpp"

#include <functional>

float constexpr borderUiSize = 3.5f;

float constexpr sectionSize = 0.8f;
float constexpr checkboxSize = 0.8f;
float constexpr checkboxTextSize = 0.4f;

bool ColorTriggerGenUI::setup(GeneratorOptions value) {
    this->setTitle("Color trigger generator options");
    this->setID("colorTriggerGenPopup"_spr);

    m_options = value;

    createLeftSide();
    createRightSide();
    createBottom();

    return true;
}

ColorTriggerGenUI* ColorTriggerGenUI::create(GeneratorOptions options) {
    auto ret = new ColorTriggerGenUI();
    if (ret->initAnchored(500.0f, 240.0f, options)) {
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
        m_options.m_useGdGridSpace = obj->isOn();
    });
    m_useGdGridSpaceCheckbox->setPosition({ 20, 150 });
    m_useGdGridSpaceCheckbox->toggle(m_options.m_useGdGridSpace);
    m_buttonMenu->addChild(m_useGdGridSpaceCheckbox);

    // m_useGdGridSpaceText

    m_useGdGridSpaceText = CCLabelBMFont::create("Use GD's grid space", "bigFont.fnt");
    m_useGdGridSpaceText->setPosition({ 110, 150 });
    m_useGdGridSpaceText->setScale(checkboxTextSize);
    m_mainLayer->addChild(m_useGdGridSpaceText);

    // m_offsetXfield

    m_offsetXfield = TextInput::create(100.0f, "Offset X");
    m_offsetXfield->setPosition({ 58, 115 });
    m_offsetXfield->setCommonFilter(CommonFilter::Float);
    m_offsetXfield->setCallback([this](std::string string) {
        auto toFloat = utils::numFromString<float>(string);
        log::info("{}", toFloat.isOk());
        if (toFloat.isOk())
            m_options.m_offsetX = *toFloat;
    });
    m_mainLayer->addChild(m_offsetXfield);

    // m_offsetYfield

    m_offsetYfield = TextInput::create(100.0f, "Offset Y");
    m_offsetYfield->setPosition({ 58, 80 });
    m_offsetYfield->setCommonFilter(CommonFilter::Float);
    m_offsetYfield->setCallback([this](std::string string) {
        auto toFloat = utils::numFromString<float>(string);
        if (toFloat.isOk())
            m_options.m_offsetY = *toFloat;
    });
    m_mainLayer->addChild(m_offsetYfield);
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
        m_options.m_genForSelectedObjects= obj->isOn();
    });
    m_generateForSelectedObjectsCheckbox->setPosition({ 340, 150 });
    m_generateForSelectedObjectsCheckbox->toggle(m_options.m_genForSelectedObjects);
    m_buttonMenu->addChild(m_generateForSelectedObjectsCheckbox);

    // m_generateForSelectedObjectsText

    m_generateForSelectedObjectsText = CCLabelBMFont::create("Generate only for\nselected objects", "bigFont.fnt");
    m_generateForSelectedObjectsText->setPosition({ 425, 150 });
    m_generateForSelectedObjectsText->setScale(checkboxTextSize);
    m_mainLayer->addChild(m_generateForSelectedObjectsText);

    // m_parseBuiltinColorChannelsCheckbox

    m_parseBuiltinColorChannelsCheckbox = CCMenuItemExt::createTogglerWithStandardSprites(checkboxSize, [this](CCMenuItemToggler* obj) {
        m_options.m_parseBuiltinColorChannels = obj->isOn();
    });
    m_parseBuiltinColorChannelsCheckbox->setPosition({ 340, 110 });
    m_parseBuiltinColorChannelsCheckbox->toggle(m_options.m_parseBuiltinColorChannels);
    m_buttonMenu->addChild(m_parseBuiltinColorChannelsCheckbox);

    // m_parseBuiltinColorChannelsText

    m_parseBuiltinColorChannelsText = CCLabelBMFont::create("Parse built-in\ncolor channels", "bigFont.fnt");
    m_parseBuiltinColorChannelsText->setPosition({ 416, 110 });
    m_parseBuiltinColorChannelsText->setScale(checkboxTextSize);
    m_mainLayer->addChild(m_parseBuiltinColorChannelsText);
};

void ColorTriggerGenUI::createBottom() {
    m_generateButton = CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create("Generate", "goldFont.fnt", "GJ_button_04.png", .8f),
        [this](CCMenuItemSpriteExtra*) {
            this->removeMeAndCleanup();
        }
    );

    m_generateButton->setPosition({ 250, 25 });
    m_buttonMenu->addChild(m_generateButton);
}

CCMenuItemSpriteExtra* ColorTriggerGenUI::createInfoButton() {
    return nullptr; // TODO
};

CCLayerColor* ColorTriggerGenUI::createSeparator() {
    return createSeparator({ 0, 0 });
}

CCLayerColor* ColorTriggerGenUI::createSeparator(CCPoint pos, float width, float height) {
    auto layer = CCLayerColor::create(ccc4(129, 71, 35, (int) 255*3/4), width, height);
    layer->setPosition({ pos.x, pos.y });

    return layer;
}