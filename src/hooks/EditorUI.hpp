#pragma once

class ColorTriggerGenUI; // I am fucking going to kill myself because of those stupid building errors

#include <Geode/Geode.hpp>
#include "../ui/generatorOptions.hpp"
#include "../ui/colorTriggerGeneratorUI.hpp"
#include "../ui/PositionableNotification.hpp"

using namespace geode::prelude;

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        PositionableNotification* m_waitingForSelectionNotification;
        std::optional<GeneratorOptions> m_genOptions; // Last generator options
        WeakRef<GameObject> m_centerBlock;

        ColorTriggerGenUI* m_genUI;
    };

    void onGenerateColorTriggers(CCObject*);
    void generateColorTriggers(const GeneratorOptions options);

    static PositionableNotification* createWaitingForSelectionNotif();
    static std::string createWaitingForSelectionNotifText();

    void createMoveMenu();
    bool init(LevelEditorLayer* editorLayer);
};