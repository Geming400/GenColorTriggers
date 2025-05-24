#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>

#include "../utils/utils.hpp"
#include "../utils/customColors.hpp"

using namespace geode::prelude;
using namespace keybinds;

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

const std::vector<int> allCustomColors = {
	(int) CustomColors::BG,
	(int) CustomColors::G1,
	(int) CustomColors::G2,
	(int) CustomColors::LINE,
	(int) CustomColors::THREEDL,
	(int) CustomColors::OBJ,
	(int) CustomColors::LBG,
	(int) CustomColors::MG,
	(int) CustomColors::MG2,

	(int) CustomColors::LIGHTER,
	(int) CustomColors::BLACK,
	(int) CustomColors::WHITE
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

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(MyLevelEditorLayer, LevelEditorLayer) {
	/**
	 * Generate color triggers for each channels
	 * @return the number of generated color triggers
	 */
	int genColorTriggers(GameObject* center, CCPoint offset) {	
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

		auto colorChannels = modUtils::getColorChannelsFromLevelString(levelString);

		if (!colorChannels) {
			Notification::create("No colour channels found!", NotificationIcon::Warning)->show();
			return 0;
		}

		for (const auto& [colorChannel, colorTriggerContent] : colorChannels.value()) {
			auto obj = static_cast<EffectGameObject*>(createObject(COLOR_TRIGGER_OBJ_ID, offset, false));
			
			modUtils::colorTriggerContentToColorTrigger(obj, colorTriggerContent);

			if (m_currentLayer != -1) {
            	obj->m_editorLayer = m_currentLayer;
			}
			
			if (isColorSet(colorTriggerContent.colorAction->m_color)) {
				offset.y += 30;
			}

			objects.push_back(obj);
		}
		

		if (!(objects.size() == 0)) {
			editorUI->deselectAll();
		}

		editorUI->selectObjects(objects.inner(), false);

		return objects.size();
	}
};

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;
		
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			if (event->isDown()) {
				auto selectedObjects = CCArrayExt<GameObject>(getSelectedObjects());

				if (selectedObjects.size() == 1) {
					GameObject* obj = selectedObjects[0];
					
					CCPoint offset = obj->getPosition();

					double offset_x = Mod::get()->getSettingValue<double>("offset-x");
					double offset_y = Mod::get()->getSettingValue<double>("offset-y");

					if (Mod::get()->getSettingValue<bool>("use-gd-grid-space")) {
						offset_x = modUtils::coordinateToGDgridPos(offset_x);
						offset_y = modUtils::coordinateToGDgridPos(offset_y);
					}

					offset.x += offset_x;
					offset.y += offset_y;

					int colorTriggersNum = static_cast<MyLevelEditorLayer*>(editorLayer)->genColorTriggers(selectedObjects[0], offset);
					if (colorTriggersNum == 0) {
						Notification::create("Created 0 color triggers.", NotificationIcon::Warning)->show();
					} else {
						Notification::create(fmt::format("Sucessfully generated {} color triggers!", colorTriggersNum), NotificationIcon::Success)->show();
					}
				} else {
					if (selectedObjects.size() == 0) {
						Notification::create("You must select at least 1 object!", NotificationIcon::Error)->show();
					} else {
						Notification::create("You must select only 1 object!", NotificationIcon::Error)->show();
					}
				}
			}
			// Return Propagate if you want other actions with the same bind to
			// also be fired, or Stop if you want to halt propagation
			return ListenerResult::Propagate;
		}, "genColorTriggers"_spr);

		log::info("Registered keybind 'genColorTriggers' !");

		return true;
	}
};