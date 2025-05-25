#include "LevelEditorLayer.hpp"
#include "../utils/utils.hpp"
#include "../utils/customColors.hpp"


#define COLOR_TRIGGER_OBJ_ID 899
#define OBJ_RED 7
#define OBJ_GREEN 8
#define OBJ_BLUE 9
#define OBJ_TARGET_CHANNEL 23

const std::vector<int> allowedCustomColors = {
	(int) CustomColors::BG,
	(int) CustomColors::G1,
	(int) CustomColors::G2,
	(int) CustomColors::LINE,
	(int) CustomColors::THREEDL,
	(int) CustomColors::OBJ,
	(int) CustomColors::LBG,
	(int) CustomColors::MG,
	(int) CustomColors::MG2
};

/**
 * Checks if a given color is set inside of gd.
 * @param color the color that will be used to do the check
 * @note this will check if the `r`, `g` and `b` values are all set to 255
 */
bool isColorSet(const ccColor3B& color) {
	return !((color.r == 255) && (color.g == 255) && (color.b == 255));
}

#define GET_COLOR_ACTION_DICT(levelSettings) levelSettings->m_effectManager->m_colorActionDict
#define CCDICT_TO_CCDICTEXT(key, value, CCDict) 

int MyLevelEditorLayer::genColorTriggers(GameObject* center, CCPoint offset) {	
	CCArrayExt<EffectGameObject> objects;

	auto editorUI = this->m_editorUI;

	/*
	for (const auto& [colorID, color] : CCDictionaryExt<int, ColorAction>(GET_COLOR_ACTION_DICT(m_levelSettings))) {
		if (Mod::get()->getSettingValue<bool>("include-builtin-color-channels")) {
			if (modUtils::isInVector(allowedCustomColors, colorID)) { continue; }
		} else {
			if (modUtils::isInVector(allCustomColors, colorID)) { continue; }
		}

		auto obj = static_cast<EffectGameObject*>(createObject(COLOR_TRIGGER_OBJ_ID, offset, false));

		if (m_currentLayer != -1)
			obj->m_editorLayer = m_currentLayer;
		
		if (isColorSet(color->m_color)) {
			offset.y += 30;
		}

		objects.push_back(obj);
	}*/

	std::string levelString = getLevelString();

	auto colorChannels = modUtils::colorChannelsParser::getColorChannelsFromLevelString(levelString);

	if (!colorChannels) {
		Notification::create("No colour channels found!", NotificationIcon::Warning)->show();
		return 0;
	}
	
	for (const auto &colorTriggerContent : colorChannels.value()) {
		if (Mod::get()->getSettingValue<bool>("include-builtin-color-channels")) {
			if (!modUtils::isInVector(allowedCustomColors, colorTriggerContent.targetChannelID) && colorTriggerContent.targetChannelID >= 1000) { continue; }
		} else {
			if (colorTriggerContent.targetChannelID >= 1000) { continue; }
		}

		auto obj = static_cast<EffectGameObject*>(createObject(COLOR_TRIGGER_OBJ_ID, offset, false));
		
		modUtils::colorChannelsParser::colorTriggerContentToColorTrigger(obj, colorTriggerContent);

		if (m_currentLayer != -1) {
			obj->m_editorLayer = m_currentLayer;
		}
		
		offset.y += 30;

		objects.push_back(obj);
	}
	

	if (!(objects.size() == 0)) {
		editorUI->deselectAll();
	}

	return objects.size();
}