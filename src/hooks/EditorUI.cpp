#include "EditorUI.hpp"

#include "Geode/loader/Log.hpp"
#include "Geode/ui/GeodeUI.hpp"
#include "LevelEditorLayer.hpp"
#include "../utils/utils.hpp"

// TODO: Remake
std::string bindAsString(std::string bindID, size_t defaultIndex = 0) {
	#ifdef CAN_USE_CUSTOM_KEYBINDS

	if (Loader::get()->isModInstalled(CUSTOM_KEYBINDS_MOD_ID)) {
		auto binds = BindManagerV2::getBindsFor(bindID);
		if (binds) {
			std::string strBind = binds.unwrap()[defaultIndex]->toString();

			// std::string strBind = BindManager::get()->getBindsFor(bindID)[defaultIndex]->toString();
			/*
			auto bind = BindManager::get()->getBindable(bindID);
			std::string strBind = (*bind).getDefaults()[defaultIndex]->toString();
			*/

			return strBind;
		} else {
			log::error("Wasn't able to get bind name from bindID {} (defaultIndex = {})", bindID, defaultIndex);
			return "";
		}
	}

	#endif

	return "press the button in the edit tab";
}

// stolen from https://github.com/geode-sdk/CustomKeybinds/blob/ba9cfc1562b19b52d52117b1f4b307e5868c7b60/src/Keybinds.cpp#L48-L49
std::string keybindAsString(enumKeyCodes key) {
	auto s = CCKeyboardDispatcher::get()->keyToString(key);
	return (s != nullptr) ? s : "Unknown keybind";
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

	double offset_x = options.offsetX;
	double offset_y = options.offsetY;

	// idk man
	// it works and that's enough
	if (!options.smallSteps) {
		offset_x = modUtils::coordinateToGDgridPos(offset_x, false);
		offset_y = modUtils::coordinateToGDgridPos(offset_y, false);
	}

	offset.x += offset_x;
	offset.y += offset_y;

	size_t colorTriggersNum = static_cast<MyLevelEditorLayer*>(m_editorLayer)->genColorTriggers(centerBlock, offset, options);
	if (colorTriggersNum == 0 && !options.genForSelectedObjects) { // Isn't supposed to happen since there are the bg colors, etc...
		log::warn("Generated 0 color triggers.");
		Notification::create("Generated 0 color triggers !!! (This is NOT supposed to happen)", NotificationIcon::Warning)->show();
	} else if (!options.genForSelectedObjects) {
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
	auto selectedObjects = CCArrayExt<GameObject>(this->getSelectedObjects());

	if (m_fields->m_genOptions) {
		m_fields->m_waitingForSelectionNotification->waitAndHide();
		if (selectedObjects.size() == 0) {
			m_fields->m_waitingForSelectionNotification->setString("Sucessfully cancelled !");
			m_fields->m_waitingForSelectionNotification->setScale(.75f);
			m_fields->m_waitingForSelectionNotification->setIcon(NotificationIcon::Success);

			m_fields->m_genOptions = std::nullopt;
			return;
		}

		this->generateColorTriggers(*m_fields->m_genOptions);

		return;
	}

	if (selectedObjects.size() == 1) {
		// If the popup is already opened, don't open it again
		if (
			m_editorLayer->getParent()->getChildByID(ColorTriggerGenUI::POPUP_ID)
			|| m_editorLayer->getParent()->getChildByID("waiting-for-selection-notification"_spr)
		) { return; }

		m_fields->m_centerBlock = selectedObjects[0];

		if (Mod::get()->getSettingValue<bool>("show-ui")) {
			m_fields->m_genUI = ColorTriggerGenUI::create(GeneratorOptions::fromSettingValues(), [this](const GeneratorOptions options) {
				if (options.genForSelectedObjects) {
					m_fields->m_genOptions = options;
					this->deselectAll();

					m_fields->m_waitingForSelectionNotification->stopAllActions();
					m_fields->m_waitingForSelectionNotification->setTime(0.f);
					m_fields->m_waitingForSelectionNotification->setString(MyEditorUI::createWaitingForSelectionNotifText());
					m_fields->m_waitingForSelectionNotification->setIcon(NotificationIcon::Info);
					m_fields->m_waitingForSelectionNotification->show(Alignement(MIDDLE, TOP));
				} else {
					this->generateColorTriggers(options);
				}
			});
			m_fields->m_genUI->show();
		} else {
			this->generateColorTriggers(GeneratorOptions::fromSettingValues());
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

	// Those 2 popups shouldn't get shown at the same time
	// Unless the user puts their keybind to F10
	if (Mod::get()->getSavedValue<bool>("first-time-loading", true)) { // if their are going into the editor for the first time after installing this then show a popup
		Mod::get()->setSavedValue<bool>("first-time-loading", false);

		FLAlertLayer* alert;

		if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
			alert = FLAlertLayer::create("Hello!", "To generate color triggers please go to the 'edit' tab.", "Dismiss");
		} else {
			alert = FLAlertLayer::create(
				"Hello!",
				fmt::format("To generate color triggers please press '{}'.", bindAsString("genColorTriggers"_spr)),
				"Dismiss"
			);
		}
		alert->m_scene = this;
		alert->show();
	}

	// if (
	// 	bindAsString("genColorTriggers"_spr) == keybindAsString(enumKeyCodes::KEY_F10)
	// 	&& !Mod::get()->getSavedValue<bool>("shown-keybind-incompability-popup", false)
	// ) {
	// 	Mod::get()->setSavedValue<bool>("shown-keybind-incompability-popup", true);

	// 	std::string content = fmt::format(
	// 		"The '**gen color trigger**' mod detected that the keybind for this mod was set to `{}`. However it **got changed in v1.1.2** to `{}`.\n"
	// 		"If you used the mod's description to find out what keybind this mod's used and it didn't work it's because you are still using the old keybind.\n\n"
	// 		"*(It got changed because the macos keybind for [devtools](https://geode-sdk.org/mods/geode.devtools) is `{}`)*",

	// 		keybindAsString(enumKeyCodes::KEY_F10),
	// 		keybindAsString(enumKeyCodes::KEY_F9),
	// 		keybindAsString(enumKeyCodes::KEY_F10)
	// 	);
	// 	MDPopup* popup = MDPopup::create("Possible keybind incompatibility found", content, "dismiss");
	// 	popup->m_scene = this;
	// 	popup->show();
	// }

	auto keybinds = Mod::get()->getSettingValue<std::vector<Keybind>>("generate-color-triggers");

	// The keybind was only available on desktops
	#ifdef GEODE_IS_DESKTOP
	if (
		!Mod::get()->getSavedValue<bool>("shown-new-keybinds-popup", false)
		&& keybinds.size() == 1 && keybinds[0].key == enumKeyCodes::KEY_F9 // F9 is the old keybind
	) {
		Mod::get()->setSavedValue<bool>("shown-new-keybinds-popup", true);

		// TODO: make it open the settings
		std::string content = fmt::format(
			"The keybind to open the gen color trigger's ui has changed. It has now been set to `U`. Robtop is now using the F9 key for the tracing system.\n\n"
			"**To prevent any conflicts, you should change the mod's keybind**. You can always change it in this mod's settings"
		);

		// auto newKeybind = Keybind::fromString("u");
		// if (newKeybind.isOk()) {
		// 	// TODO: doesn't really work. See how geode handles it internally
		// 	Mod::get()->setSavedValue<std::vector<Keybind>>("generate-color-triggers", { newKeybind.unwrap() });

		// 	SettingNodeValueChangeEventV3(
		// 		Mod::get(),
		// 		"generate-color-triggers"
		// 	).send(nullptr, false);
		// 	log::info("Set keybind for setting 'shown-new-keybinds-popup' to U");
		// } else {
		// 	log::error("Failed to create keybind 'U': {}", newKeybind.err());
		// }


		MDPopup* popup = MDPopup::create("Keybind incompatiblity warning", content, "Dismiss", "Open Settings", [](bool btn2) {
			if (btn2)
				openSettingsPopup(Mod::get());
		});
		popup->m_scene = this;
		popup->show();
	}
	#endif

	this->addEventListener(
		KeybindSettingPressedEventV3(Mod::get(), "generate-color-triggers"),
		[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
			if (down && !repeat) {
				onGenerateColorTriggers(nullptr);
			}
		}
	);

	log::info("Registered keybind 'genColorTriggers' !");

	return true;
}

void MyEditorUI::createMoveMenu() {
	EditorUI::createMoveMenu();

	if (Mod::get()->getSavedValue<bool>("show-editor-button") || Mod::get()->getSettingValue<std::vector<geode::Keybind>>("generate-color-triggers").empty()) {
		auto btn = this->getSpriteButton("genColorTriggers.png"_spr, menu_selector(MyEditorUI::onGenerateColorTriggers), nullptr, 0.9f);
		m_editButtonBar->m_buttonArray->addObject(btn);

		int rows = GameManager::sharedState()->getIntGameVariable("0049");
		int columns = GameManager::sharedState()->getIntGameVariable("0050");
		m_editButtonBar->reloadItems(rows, columns);
	}
}

PositionableNotification* MyEditorUI::createWaitingForSelectionNotif() {
	auto notif = PositionableNotification::create(
		MyEditorUI::createWaitingForSelectionNotifText(),
		NotificationIcon::Info,
		0
	);


	notif->setScale(0.6f);
	notif->setID("waiting-for-selection-notification"_spr);
	notif->retain();

	return notif;
}

std::string MyEditorUI::createWaitingForSelectionNotifText() {
	std::string ret;

	if (Mod::get()->getSavedValue<bool>("show-editor-button")) {
		ret = "Please select objects and then press the editor button again";
	} else {
		ret = fmt::format("Please select objects and open the gen colors trigger gui");
	}

	return ret;
}
