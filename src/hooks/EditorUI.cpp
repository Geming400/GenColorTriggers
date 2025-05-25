#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>

#include "LevelEditorLayer.hpp"
#include "../utils/utils.hpp"

using namespace geode::prelude;
using namespace keybinds;

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
	struct Fields {
		Notification* m_parsingNotification;
	};

	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;
		
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			if (event->isDown()) {
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

					int colorTriggersNum = static_cast<MyLevelEditorLayer*>(editorLayer)->genColorTriggers(selectedObjects[0], offset);
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
			// Return Propagate if you want other actions with the same bind to
			// also be fired, or Stop if you want to halt propagation
			return ListenerResult::Propagate;
		}, "genColorTriggers"_spr);

		log::info("Registered keybind 'genColorTriggers' !");

		return true;
	}
};