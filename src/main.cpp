#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_MOBILE
    $on_mod(Loaded) {
        Mod::get()->setSavedValue<bool>("show-editor-button", true);
    }
#endif

#ifdef GEODE_IS_DESKTOP
    $on_mod(Loaded) {
        if (Mod::get()->getSavedValue<bool>("first-time-loading", true)) {
            Mod::get()->setSavedValue<bool>("show-editor-button", false);
        }

        if (Mod::get()->getSavedValue<bool>("show-editor-button", false) != Mod::get()->getSettingValue<bool>("show-editor-button")) {
            const bool newValue = Mod::get()->getSavedValue<bool>("show-editor-button", false);

            log::info("Desync found in setting 'first-time-loading'. Setting saved and setting value to {}", newValue);
            Mod::get()->setSavedValue<bool>("show-editor-button", newValue);
            Mod::get()->setSettingValue<bool>("show-editor-button", newValue)
        }
    }
#endif