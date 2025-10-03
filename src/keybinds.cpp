#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_DESKTOP

#include <geode.custom-keybinds/include/OptionalAPI.hpp>
using namespace keybinds;

$execute {
    // BindManager::get()->registerBindable({
    //     // ID, should be prefixed with mod ID
    //     "genColorTriggers"_spr,
    //     // Name
    //     "Generate color triggers",
    //     // Description, leave empty for none
    //     "Generate color triggers for every channels",
    //     // Default binds
    //     { Keybind::create(KEY_F9, Modifier::None) },
    //     // Category; use slashes for specifying subcategories. See the
    //     // Category class for default categories
    //     Category::EDITOR,
    //     false // repeatable = false
    // });

    (void)[&]() -> Result<> {
        GEODE_UNWRAP(BindManagerV2::registerBindable(GEODE_UNWRAP(BindableActionV2::create(
            // ID, should be prefixed with mod ID
            "genColorTriggers"_spr,
            // Name
            "Generate color triggers",
            // Description, leave empty for none
            "Generate color triggers for every channels",
            // Default binds
            { GEODE_UNWRAP(KeybindV2::create(KEY_F9, ModifierV2::None)) },
            // Category; use slashes for specifying subcategories. See the
            // Category class for default categories
            GEODE_UNWRAP(CategoryV2::create(Category::EDITOR)),
            false // repeatable = false
        ))));
        return Ok();
    }();
}
#endif