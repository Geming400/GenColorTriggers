#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

$execute {
    listenForSettingChanges("show-editor-button", [](bool value) {
        // We store that setting as a saved value instead because this setting won't exist on mobile.
        Mod::get()->setSavedValue<bool>("show-editor-button", value);
    });
}