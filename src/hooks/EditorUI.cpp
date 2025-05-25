#include <Geode/Geode.hpp>


#include "LevelEditorLayer.hpp"
#include "../utils/utils.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_DESKTOP
	#include <geode.custom-keybinds/include/Keybinds.hpp>
	using namespace keybinds;
#endif

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
	struct Fields {
		Notification* m_parsingNotification;
		LevelEditorLayer* m_levelEditorLayer;
	};

	void onGenerateColorTriggers(CCObject*) {
		if (!m_fields->m_levelEditorLayer) {
			log::error("The editor layer wasn't found ??");
			Notification::create("The editor layer wasn't found ??", NotificationIcon::Error)->show();

			return;
		}

		m_fields->m_parsingNotification = Notification::create("Parsing level...", NotificationIcon::Loading, 5);

		auto selectedObjects = CCArrayExt<GameObject>(getSelectedObjects());

		if (selectedObjects.size() == 1 && Mod::get()->getSettingValue<bool>("show-parsing-notif")) {
			m_fields->m_parsingNotification->show();
		}

		if (selectedObjects.size() == 1) {
			GameObject* obj = selectedObjects[0];
			
			CCPoint offset = obj->getPosition();

			double offset_x = Mod::get()->getSettingValue<double>("offset-x");
			double offset_y = Mod::get()->getSettingValue<double>("offset-y");

			if (Mod::get()->getSettingValue<bool>("use-gd-grid-space")) {
				offset_x = modUtils::coordinateToGDgridPos(offset_x, false);
				offset_y = modUtils::coordinateToGDgridPos(offset_y, false);
			}

			offset.x += offset_x;
			offset.y += offset_y;

			int colorTriggersNum = static_cast<MyLevelEditorLayer*>(m_fields->m_levelEditorLayer)->genColorTriggers(selectedObjects[0], offset);
			m_fields->m_parsingNotification->hide(); // instantly hide the notification
			if (colorTriggersNum == 0) {
				Notification::create("Created 0 color triggers.", NotificationIcon::Warning)->show();
			} else {
				Notification::create(fmt::format("Sucessfully generated {} color triggers!", colorTriggersNum), NotificationIcon::Success)->show();
			}
		} else {
			m_fields->m_parsingNotification->hide(); // instantly hide the notification
			if (selectedObjects.size() == 0) {
				Notification::create("You must select at least 1 object!", NotificationIcon::Error)->show();
			} else {
				Notification::create("You must select only 1 object!", NotificationIcon::Error)->show();
			}
		}
	}

	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		m_fields->m_levelEditorLayer = editorLayer;

        if (!Mod::get()->getSavedValue<bool>("first-time-loading", false)) { // if their are going into the editor for the first time after installing this then show a popup
            Mod::get()->setSavedValue<bool>("first-time-loading", true);

            auto alert = FLAlertLayer::create("Hello!", "To generate color triggers please press 'F10' !", "Ok!");
            alert->m_scene = this;
            alert->show();
	    }

		#ifdef GEODE_IS_DESKTOP
			this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
				if (event->isDown()) {
					onGenerateColorTriggers(nullptr);
				}
				// Return Propagate if you want other actions with the same bind to
				// also be fired, or Stop if you want to halt propagation
				return ListenerResult::Propagate;
			}, "genColorTriggers"_spr);

			log::info("Registered keybind 'genColorTriggers' !");
		#else
			log::info("User is on mobile, cannot register a keybind.");
		#endif

		return true;
	}

	void createMoveMenu() {
		EditorUI::createMoveMenu();

		if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
			auto* btn = this->getSpriteButton("genColorTriggers.png"_spr, menu_selector(MyEditorUI::onGenerateColorTriggers), nullptr, 0.9f);
			m_editButtonBar->m_buttonArray->addObject(btn);

			auto rows = GameManager::sharedState()->getIntGameVariable("0049");
			auto columns = GameManager::sharedState()->getIntGameVariable("0050");
			m_editButtonBar->reloadItems(rows, columns);
		}
	}
};