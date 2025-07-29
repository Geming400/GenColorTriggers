#pragma once

#include <Geode/Geode.hpp>
#include "../utils/PositionableNotification.hpp"

using namespace geode::prelude;

#define GET_BUTTON(node_id) static_cast<CCMenuItemSpriteExtra*>(resumeMenu->getChildByID(node_id));

#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    PositionableNotification* getWaitingForSelectionNotif() {
        auto notif = this->getParent()->getChildByID("waiting-for-selection-notification"_spr);
        PositionableNotification* positionableNotif = nullptr;
        if (notif) positionableNotif = static_cast<PositionableNotification*>(notif);

        return positionableNotif;
    }

    /**
     * @note this will assume that `button`'s original opacity was 255
     */
    void toggleButton(CCMenuItemSpriteExtra* button, bool state) {
        if (state == false) {
            button->setEnabled(false);
            button->setOpacity(127);
        } else {
            button->setEnabled(true);
            button->setOpacity(0);
        }
    }

    void toggleButtons(float) {
        if (getWaitingForSelectionNotif()) {
            auto resumeMenu = this->getChildByID("resume-menu");

            auto resumeButton = GET_BUTTON("resume-button");
            auto saveAndPlayButton = GET_BUTTON("save-and-play-button");
            auto saveAndExitButton = GET_BUTTON("save-and-exit-button");
            auto saveButton = GET_BUTTON("save-button");
            auto exitButton = GET_BUTTON("exit-button");

            toggleButton(saveAndPlayButton, false);
            toggleButton(saveAndExitButton, false);
            toggleButton(exitButton, false);
        }
    }

    bool init(LevelEditorLayer* p0) {
        if (!EditorPauseLayer::init(p0)) return false;
        
        this->scheduleOnce(schedule_selector(MyEditorPauseLayer::toggleButtons), 0.f);

        return true;
    }
};