#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_DESKTOP
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;

$execute {
    BindManager::get()->registerBindable({
        // ID, should be prefixed with mod ID
        "genColorTriggers"_spr,
        // Name
        "Generate color triggers",
        // Description, leave empty for none
        "Generate color triggers for every channels",
        // Default binds
        { Keybind::create(KEY_F9, Modifier::None) },
        // Category; use slashes for specifying subcategories. See the
        // Category class for default categories
        Category::EDITOR,
        false // repeatable = false
    });
}
#endif