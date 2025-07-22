#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
    void onGenerateColorTriggers(CCObject*);

    void createMoveMenu();
    bool MyEditorUI::init(LevelEditorLayer* editorLayer);
};