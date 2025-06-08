#include "LevelEditorLayer.hpp"
#include "../utils/utils.hpp"
#include "../parser/parser.hpp"
#include "../utils/customColors.hpp"
#include <cmath>


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


std::vector<modUtils::ColorTriggerContent> getGeneratableColorChannels(std::vector<modUtils::ColorTriggerContent> colorChannels) {
	std::vector<modUtils::ColorTriggerContent> generatableColorChannels;
	for (const auto &colorTriggerContent : colorChannels) {
		if (Mod::get()->getSettingValue<bool>("include-builtin-color-channels")) {
			if (!modUtils::isInVector(allowedCustomColors, colorTriggerContent.targetChannelID) && colorTriggerContent.targetChannelID >= 1000) { continue; }
		} else {
			if (colorTriggerContent.targetChannelID >= 1000) { continue; }
		}

		generatableColorChannels.push_back(colorTriggerContent);
	}

	return generatableColorChannels;
}

int MyLevelEditorLayer::genColorTriggers(GameObject* center, CCPoint offset) {	
	const CCPoint originalOffset = offset;
	const int64_t forEachTriggers = Mod::get()->getSettingValue<int64_t>("for-each-triggers");
	const auto editorUI = this->m_editorUI;
	bool pushingIntoVector = true;

	std::string levelString = getLevelString();

	auto colorChannels = colorChannelsParser::getColorChannelsFromLevelString(levelString);
	if (!colorChannels) {
		Notification::create("No colour channels found! (This isn't supposed to happen btw)", NotificationIcon::Error)->show();
		log::error("No colour channels found! Please stop doing some tomfoolery to the gd editor or something, I beg :(");
		return 0;
	}

	auto generatableColorChannels = getGeneratableColorChannels(colorChannels.value());
	int colorChannelsNum = generatableColorChannels.size();

	float prettifyXoffset;
	int numColumns = ceil(static_cast<double>(colorChannelsNum) / static_cast<double>(forEachTriggers));
	if (numColumns > 0) {
		prettifyXoffset = 15 * (numColumns - 1);
		log::debug("prettifyXoffset = {}", prettifyXoffset);

		offset.x -= prettifyXoffset;
	} else {
		log::debug("numColumns < 0");
	}
	
	int i = 1;
	CCArrayExt<EffectGameObject> objects;
	for (const auto &colorTriggerContent : generatableColorChannels) {
		auto obj = static_cast<EffectGameObject*>(createObject(COLOR_TRIGGER_OBJ_ID, offset, false));
		
		colorChannelsParser::colorTriggerContentToColorTrigger(obj, colorTriggerContent, Mod::get()->getSettingValue<bool>("use-legacy-hsv"));

		if (m_currentLayer != -1) {
			obj->m_editorLayer = m_currentLayer;
		}
		
		if (i % static_cast<int>(forEachTriggers) == 0 && Mod::get()->getSettingValue<bool>("prettify-trigger-pos")) {
			offset.y = originalOffset.y;
			offset.x += 30;
		} else {
			offset.y += 30;
		}

		if (pushingIntoVector) {
			objects.push_back(obj);
		}

		if (objects.size() > vectorSizePushLimit) { // Additional check just in case the user did some tomfoolery
			pushingIntoVector = false;
			log::warn("Exceeded 'objects' vector limit somehow, idfk what you did but ok (= {})", vectorSizePushLimit);
		}

		i++;
	}
	

	if (!(objects.size() == 0)) {
		editorUI->deselectAll();
	}

	return objects.size();
}