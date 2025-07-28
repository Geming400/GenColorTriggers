#pragma once

class PositionableNotification; // I am going to kill msvc
class ColorTriggerGenUI; // I am fucking going to kill myself because of those stupid building errors

#include <Geode/Geode.hpp>
#include "../utils/PositionableNotification.hpp"
#include "../utils/utils.hpp"
#include "../ui/generatorOptions.hpp"
#include "../ui/colorTriggerGeneratorUI.hpp"

using namespace geode::prelude;

#include <Geode/modify/EditorUI.hpp>
class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        PositionableNotification* m_waitingForSelectionNotification;
        std::optional<GeneratorOptions> m_genOptions; // Last generator options

        ColorTriggerGenUI* m_genUI;
    };

    void onGenerateColorTriggers2(CCObject*);
    void onGenerateColorTriggers(CCObject*);
    void generateColorTriggers(const GeneratorOptions options);

    static PositionableNotification* createWaitingForSelectionNotif();
    static std::string createWaitingForSelectionNotifText();

    void createMoveMenu();
    bool init(LevelEditorLayer* editorLayer);
};