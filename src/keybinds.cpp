#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;
using namespace geode::prelude;

$execute {
    BindManager::get()->registerBindable({
        // ID, should be prefixed with mod ID
        "genColorTriggers"_spr,
        // Name
        "Generate color triggers",
        // Description, leave empty for none
        "Generate color triggers for every channels",
        // Default binds
        { Keybind::create(KEY_F10, Modifier::None) },
        // Category; use slashes for specifying subcategories. See the
        // Category class for default categories
        Category::EDITOR
    });
}