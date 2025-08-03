#include "EditorUI.hpp"

#include "LevelEditorLayer.hpp"

#ifdef GEODE_IS_DESKTOP
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

std::string bindAsString(std::string bindID, size_t defaultIndex = 0) {
	#ifdef GEODE_IS_DESKTOP

	std::string strBind = BindManager::get()->getBindsFor(bindID)[defaultIndex]->toString();
    
	/*
	auto bind = BindManager::get()->getBindable(bindID);
	std::string strBind = (*bind).getDefaults()[defaultIndex]->toString();
	*/

	return strBind;
	#else
	return "press the button in the edit tab"; // should never get called, but once again, just in case
	#endif
}

void MyEditorUI::generateColorTriggers(const GeneratorOptions options) {
	m_fields->m_genOptions = std::nullopt;

	auto selectedObjects = CCArrayExt<GameObject>(this->getSelectedObjects());
	
	if (!m_fields->m_centerBlock) {
		Notification::create("The center block doesn't seem to exist anymore.", NotificationIcon::Warning);
		return;
	}
	auto centerBlock = m_fields->m_centerBlock.lock();

	CCPoint offset = centerBlock->getPosition();

	double offset_x = options.m_offsetX;
	double offset_y = options.m_offsetY;

	if (options.m_useGdGridSpace) {
		offset_x = modUtils::coordinateToGDgridPos(offset_x, false);
		offset_y = modUtils::coordinateToGDgridPos(offset_y, false);
	}

	offset.x += offset_x;
	offset.y += offset_y;

	size_t colorTriggersNum = static_cast<MyLevelEditorLayer*>(m_editorLayer)->genColorTriggers(centerBlock, offset, options);
	if (colorTriggersNum == 0 && !options.m_genForSelectedObjects) { // Isn't supposed to happen since there are the bg colors, etc...
		log::warn("Generated 0 color triggers.");
		Notification::create("Generated 0 color triggers !!! (This is NOT supposed to happen)", NotificationIcon::Warning)->show();
	} else if (!options.m_genForSelectedObjects) {
		std::string colorTriggersNumStr = colorTriggersNum >= vectorSizePushLimit ? fmt::format("{}+", colorTriggersNum) : fmt::to_string(colorTriggersNum);
		Notification::create(fmt::format("Sucessfully generated {} color triggers!", colorTriggersNumStr), NotificationIcon::Success)->show();
	} else {
		log::info("Generated 0 color triggers (The objects hadn't any color data, so this should be normal)");
		auto notif = Notification::create("Generated 0 color triggers (The objects hadn't any color data)", NotificationIcon::Info);
		notif->setScale(.7f);
		notif->show();
	}
}

void MyEditorUI::onGenerateColorTriggers(CCObject*) {
	if (!m_editorLayer) { // Really NEVER should happen btw
		log::error("The editor layer wasn't found ??");
		Notification::create("The editor layer wasn't found ??", NotificationIcon::Error)->show();

		return;
	}

	/*
	log::info("m_isPaused = {}", m_isPaused);
	if (m_isPaused) return;
	*/

	auto selectedObjects = CCArrayExt<GameObject>(this->getSelectedObjects());

	if (m_fields->m_genOptions) {
		m_fields->m_waitingForSelectionNotification->waitAndHide();
		if (selectedObjects.size() == 0) {
			m_fields->m_waitingForSelectionNotification->setString("Sucessfully cancelled !");
			m_fields->m_waitingForSelectionNotification->setIcon(NotificationIcon::Success);

			m_fields->m_genOptions = std::nullopt;
			return;
		}

		generateColorTriggers(*m_fields->m_genOptions);

		return;
	}

	if (selectedObjects.size() == 1) {
		if (
			m_editorLayer->getParent()->getChildByID(ColorTriggerGenUI::POPUP_ID) ||
			m_editorLayer->getParent()->getChildByID("waiting-for-selection-notification"_spr)
		) { return; }

		m_fields->m_centerBlock = selectedObjects[0];

		if (Mod::get()->getSettingValue<bool>("show-ui")) {
			m_fields->m_genUI = ColorTriggerGenUI::create(GeneratorOptions::fromSettingValues(), [this](const GeneratorOptions options) {
				if (options.m_genForSelectedObjects) {
					m_fields->m_genOptions = options;
					this->deselectAll();

					m_fields->m_waitingForSelectionNotification->stopAllActions();
					m_fields->m_waitingForSelectionNotification->setTimeMember(0.f);
					m_fields->m_waitingForSelectionNotification->setString(MyEditorUI::createWaitingForSelectionNotifText());
					m_fields->m_waitingForSelectionNotification->setIcon(NotificationIcon::Info);
					m_fields->m_waitingForSelectionNotification->show(Alignement(MIDDLE, TOP));
				} else {
					generateColorTriggers(options);
				}
			});
			m_fields->m_genUI->show();
		} else {
			generateColorTriggers(GeneratorOptions::fromSettingValues());
		}
	} else {
		if (selectedObjects.size() == 0) {
			Notification::create("You must select at least 1 object!", NotificationIcon::Error)->show();
		} else {
			Notification::create("You must select only 1 object!", NotificationIcon::Error)->show();
		}
	}
}

bool MyEditorUI::init(LevelEditorLayer* editorLayer) {
	if (!EditorUI::init(editorLayer)) return false;

	m_fields->m_waitingForSelectionNotification = createWaitingForSelectionNotif();

	if (Mod::get()->getSavedValue<bool>("first-time-loading", true)) { // if their are going into the editor for the first time after installing this then show a popup
		Mod::get()->setSavedValue<bool>("first-time-loading", false);

		FLAlertLayer* alert;

		#ifdef GEODE_IS_DESKTOP
		if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
			alert = FLAlertLayer::create("Hello!", "To generate color triggers please go to the 'edit' tab.", "Dismiss");
		} else {
			alert = FLAlertLayer::create(
				"Hello!",
				fmt::format("To generate color triggers please press '{}'.", bindAsString("genColorTriggers"_spr)),
				"Dismiss"
			);
		}
		#else
		alert = FLAlertLayer::create("Hello!", "To generate color triggers please go to the 'edit' tab.", "Dismiss");
		#endif

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

void MyEditorUI::createMoveMenu() {
	EditorUI::createMoveMenu();

	if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
		auto btn = this->getSpriteButton("genColorTriggers.png"_spr, menu_selector(MyEditorUI::onGenerateColorTriggers), nullptr, 0.9f);
		m_editButtonBar->m_buttonArray->addObject(btn);

		int rows = GameManager::sharedState()->getIntGameVariable("0049");
		int columns = GameManager::sharedState()->getIntGameVariable("0050");
		m_editButtonBar->reloadItems(rows, columns);
	}
}

PositionableNotification* MyEditorUI::createWaitingForSelectionNotif() {
	auto notif = PositionableNotification::create(
		createWaitingForSelectionNotifText(),
		NotificationIcon::Info,
		0
	);

	notif->setID("waiting-for-selection-notification"_spr);
	notif->retain();

	return notif;
}

std::string MyEditorUI::createWaitingForSelectionNotifText() {
	std::string ret;

	#ifdef GEODE_IS_DESKTOP
	if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
		ret = "Please select objects and then press the editor button again";
	} else {
		ret = fmt::format("Please select objects and then press '{}' again", bindAsString("genColorTriggers"_spr));
	}
	#else
	ret = "Please select objects and then press the editor button again";
	#endif 

	return ret;
}