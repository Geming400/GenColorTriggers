#include <Geode/Geode.hpp>
#include "./utils/utils.hpp"

using namespace geode::prelude;

#ifndef CAN_USE_CUSTOM_KEYBINDS
$on_mod(Loaded) {
    Mod::get()->setSavedValue<bool>("show-editor-button", true);
}
#endif

#ifdef CAN_USE_CUSTOM_KEYBINDS
$on_mod(Loaded) {
    // Extra check if the user uninstalled CK but
    // the 'show-editor-button' setting was disabled
    if (!Loader::get()->isModLoaded(CUSTOM_KEYBINDS_MOD_ID)) {
        Mod::get()->setSavedValue<bool>("show-editor-button", true);
    }


    if (Mod::get()->getSavedValue<bool>("first-time-loading", true)) {
        Mod::get()->setSavedValue<bool>("show-editor-button", false);
    }

    if (Mod::get()->getSavedValue<bool>("show-editor-button", false) != Mod::get()->getSettingValue<bool>("show-editor-button")) {
        const bool newValue = Mod::get()->getSettingValue<bool>("show-editor-button");

        log::info("Desync found in saved value 'show-editor-button'. Setting saved and setting value to {}", newValue);
        Mod::get()->setSavedValue<bool>("show-editor-button", newValue);
        Mod::get()->setSettingValue<bool>("show-editor-button", newValue);
    }
}
#endif