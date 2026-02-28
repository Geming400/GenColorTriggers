#include <Geode/loader/SettingV3.hpp>
#include "./utils/utils.hpp"

using namespace geode::prelude;

$execute {
    listenForSettingChanges<bool>("show-editor-button", [](bool value) {
        // Note for the comment below, I didn't know that there was an argument for a default value
        // in `Mod::getSettingValue<T>`
        //
        // We store that setting as a saved value instead because this setting won't exist on mobile.
        // The value is initialiazed in 'main.cpp'
        #ifdef HAS_EDITOR_BUTTON
        Mod::get()->setSavedValue<bool>("show-editor-button", value);
        #else
        Mod::get()->setSavedValue<bool>("show-editor-button", true);
        #endif
    });
}