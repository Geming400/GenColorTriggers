#include "colorTriggerGeneratorUI.hpp"

#ifdef UI_TEST
#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        
        auto genUI = ColorTriggerGenUI::create();
        genUI->m_scene = this;
        genUI->show();

        return true;
    }
};
#endif