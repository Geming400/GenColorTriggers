#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_MOBILE
    $on_mod(Loaded) {
        Mod::get()->setSavedValue<bool>("show-editor-button", true);
    }
#endif