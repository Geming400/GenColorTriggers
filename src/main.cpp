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
    }
#endif