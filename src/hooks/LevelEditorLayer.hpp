#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    /**
     * Generate color triggers for each channels
     * @return the number of generated color triggers
     */
    int genColorTriggers(GameObject* center, CCPoint offset);
};