#include <Geode/Geode.hpp>
#include "EditorUI.hpp"

using namespace geode::prelude;

#define GET_BUTTON(node_id) static_cast<CCMenuItemSpriteExtra*>(resumeMenu->getChildByID(node_id));

#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    MyEditorUI* getHookedEditorUI() {
        return static_cast<MyEditorUI*>(m_editorLayer->m_editorUI);
    }

    void setButtonOpacity(CCMenuItemSpriteExtra* button, unsigned short opacity) {
        auto btnSprite = button->getChildByType<ButtonSprite*>(0);
        btnSprite->getChildByType<CCLabelBMFont*>(0)->setOpacity(opacity);
        btnSprite->getChildByType<CCScale9Sprite*>(0)->setOpacity(opacity);
    }

    /**
     * @note this will assume that `button`'s original opacity was 255
     */
    void toggleButton(CCMenuItemSpriteExtra* button, bool state) {
        if (state == false) {
            button->setEnabled(false);
            setButtonOpacity(button, 127);
        } else {
            button->setEnabled(true);
            setButtonOpacity(button, 255);
        }
    }

    void toggleButtons(float) {
        if (getHookedEditorUI()->m_fields->m_genOptions) {
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