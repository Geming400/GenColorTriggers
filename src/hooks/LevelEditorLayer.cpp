#include "LevelEditorLayer.hpp"
#include "../parser/parser.hpp"
#include "../utils/customColors.hpp"
#include <math.h>


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
	(int) CustomColors::MG2,
	(int) CustomColors::P1,
	(int) CustomColors::P2
};

/* For debugging:
template<typename T>
void printVector(T vec) {
	log::info("(");
	for (auto& elem : vec)
    	log::info("  {}", elem);
	log::info(")");
	log::info("size = {}", vec.size());
}


void printVector(std::vector<modUtils::ColorTriggerContent> vec) {
	log::info("(");
	for (auto& elem : vec)
    	log::info("  {}", elem.targetChannelID);
	log::info(")");
	log::info("size = {}", vec.size());
}
*/

std::vector<modUtils::ColorTriggerContent> MyLevelEditorLayer::getGeneratableColorChannels(std::vector<modUtils::ColorTriggerContent> colorChannels, GeneratorOptions options) {
	// 1
	std::vector<modUtils::ColorTriggerContent> generatableColorChannels;
	for (const auto &colorTriggerContent : colorChannels) {
		if (options.m_parseBuiltinColorChannels) {
			if (!modUtils::isInVector(allowedCustomColors, colorTriggerContent.targetChannelID) && colorTriggerContent.targetChannelID >= 1000) { continue; }
		} else {
			if (colorTriggerContent.targetChannelID >= 1000) { continue; }
		}

		generatableColorChannels.push_back(colorTriggerContent);
	}


	// 2
	std::unordered_set<unsigned int> colorChannelIDs;

	auto selectedObjects = CCArrayExt<GameObject>(m_editorUI->getSelectedObjects());

	for (const auto& obj : selectedObjects) {
		if (options.m_genForSelectedObjects) {
			std::optional<int> baseColorID; // Normally there always should be a base color, but just in case
			std::optional<int> detailColorID;

			if (obj->m_baseColor) baseColorID = obj->m_baseColor->m_colorID;
			else baseColorID = std::nullopt;

			if (obj->m_detailColor) detailColorID = obj->m_detailColor->m_colorID;
			else detailColorID = std::nullopt;

			if (baseColorID) colorChannelIDs.insert(*baseColorID);
			if (detailColorID) colorChannelIDs.insert(*detailColorID);
		}
	}

	if (options.m_genForSelectedObjects) {
		std::vector<modUtils::ColorTriggerContent> newGeneratableColorChannels;

		for (size_t i = 0; i < generatableColorChannels.size(); i++) {
			auto colorChannel = generatableColorChannels.at(i);

			if (ranges::contains(colorChannelIDs, colorChannel.targetChannelID))
				newGeneratableColorChannels.push_back(colorChannel);
		}

		return newGeneratableColorChannels;
	} else {
		return generatableColorChannels;
	}
}

size_t MyLevelEditorLayer::genColorTriggers(GameObject* center, CCPoint offset, GeneratorOptions options) {	
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

	auto generatableColorChannels = getGeneratableColorChannels(colorChannels.value(), options);
	int colorChannelsNum = generatableColorChannels.size();

	float prettifyXoffset;
	int numColumns = std::ceil(static_cast<double>(colorChannelsNum) / static_cast<double>(forEachTriggers));
	if (numColumns > 0) {
		prettifyXoffset = 15 * (numColumns - 1);
		log::debug("prettifyXoffset = {}", prettifyXoffset);

		offset.x -= prettifyXoffset;
	} else {
		log::debug("numColumns <= 0");
	}
	
	int i = 1;
	CCArrayExt<EffectGameObject> objects;
	for (const auto& colorTriggerContent : generatableColorChannels) {
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
			log::warn("Exceeded 'objects' vector limit somehow, idfk what you did but ok (push limit size = {})", vectorSizePushLimit);
		}

		i++;
	}
	

	if (!(objects.size() == 0)) {
		editorUI->deselectAll();
	}

	return objects.size();
}