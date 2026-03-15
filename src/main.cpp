// For context, I didn't know that there was an overload for Mod::getSettingValue<T>() that allowed for a default value

#include <Geode/Geode.hpp>
#include "./utils/utils.hpp"

using namespace geode::prelude;

/**
 * Extra check if the user uninstalled CK but
 * the 'show-editor-button' setting was disabled
 */
void doExtraCKCheck() {
    #ifdef HAS_EDITOR_BUTTON
    Mod::get()->setSavedValue<bool>("show-editor-button", true);
    #endif
}

$on_mod(Loaded) {
    #ifdef GEODE_IS_DESKTOP

    doExtraCKCheck();

    if (Mod::get()->getSavedValue<bool>("first-time-loading", true)) {
        Mod::get()->setSavedValue<bool>("show-editor-button", false);
    }

    if (Mod::get()->getSavedValue<bool>("show-editor-button", false) != Mod::get()->getSettingValue<bool>("show-editor-button")) {
        const bool newValue = Mod::get()->getSettingValue<bool>("show-editor-button");

        log::info("Desync found in saved value 'show-editor-button'. Setting saved and setting value to {}", newValue);
        Mod::get()->setSavedValue<bool>("show-editor-button", newValue);
        Mod::get()->setSettingValue<bool>("show-editor-button", newValue);
    }

    #else

    Mod::get()->setSavedValue<bool>("show-editor-button", true);

    #endif


    if (Mod::get()->getSavedValue<bool>("first-time-loading-v1.1.8", true)) {
        log::info("Detected that user launched gen color triggers v1.1.8 for the first time. Inverting 'use-gd-grid-space' setting (renamed to 'Small Steps' for clarity)");
        Mod::get()->setSavedValue<bool>("first-time-loading-v1.1.8", false);

        Mod::get()->setSettingValue("use-gd-grid-space", !Mod::get()->getSettingValue<bool>("use-gd-grid-space"));
    }
}