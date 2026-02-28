// Copy of the old geode's Notification.
// I just need it to be in a different queue okay...

#pragma once

#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/binding/TextAlertPopup.hpp>
#include "../utils/ZStringView.hpp"

using namespace geode::prelude;

enum VerticalAlignement {
    LEFT,
    MIDDLE,
    RIGHT
};

enum HorizontalAlignement {
    TOP,
    CENTER,
    BOTTOM
};

struct Alignement {
    VerticalAlignement m_verticalAlignement;
    HorizontalAlignement m_horizontalAlignement;

    /**
     * @note Same as doing `Alignement(VerticalAlignement::MIDDLE, HorizontalAlignement::CENTER)`
     */
    static Alignement createAtCenter() {
        return Alignement(VerticalAlignement::MIDDLE, HorizontalAlignement::CENTER);
    }
};

class PositionableNotification final : public cocos2d::CCNodeRGBA {
protected:
    static cocos2d::CCArray* s_queue;
    cocos2d::extension::CCScale9Sprite* m_bg;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCSprite* m_icon = nullptr;
    float m_time;
    bool m_showing = false;

    bool init(ZStringView text, cocos2d::CCSprite* icon, float time);
    void updateLayout();

    static cocos2d::CCSprite* createIcon(NotificationIcon icon);

    void animateIn();
    void animateOut();
    void showNextNotification();
    void wait();

public:
    /**
      * Create a PositionableNotification, similar to TextAlertPopup but more customizable
      * @param text PositionableNotification text
      * @param icon Icon to show in the PositionableNotification
      * @param time Time to show the PositionableNotification on screen; pass 0 to show
      * the PositionableNotification indefinitely until hide() is called
      * @returns The new PositionableNotification. Make sure to call show() to show the
      * PositionableNotification
      */
    static PositionableNotification* create(
        ZStringView text,
        NotificationIcon icon = NotificationIcon::None,
        float time = NOTIFICATION_DEFAULT_TIME
    );
    /**
      * Create a PositionableNotification with a custom icon
      * @param text PositionableNotification text
      * @param icon Icon to show in the PositionableNotification
      * @param time Time to show the PositionableNotification on screen; pass 0 to show
      * the PositionableNotification indefinitely until hide() is called
      * @returns The new PositionableNotification. Make sure to call show() to show the
      * PositionableNotification
    */
    static PositionableNotification* create(
        ZStringView text,
        cocos2d::CCSprite* icon,
        float time = NOTIFICATION_DEFAULT_TIME
    );

    void setString(ZStringView text);
    void setIcon(NotificationIcon icon);
    void setIcon(cocos2d::CCSprite* icon);
    void setTime(float time);

    /**
      * Set the wait time to default, wait the time and hide the PositionableNotification.
      * Equivalent to setTime(NOTIFICATION_DEFAULT_TIME)
      */
    void waitAndHide();

    /**
      * Adds the notification to the current scene if it doesn't have a
      * parent yet, and displays the show animation. If the time for the
      * notification was specified, the notification waits that time and
      * then automatically hides
      * @param pos the position of where to show the notification
      */
    void show(CCPoint pos);
    /**
      * Adds the notification to the current scene if it doesn't have a
      * parent yet, and displays the show animation. If the time for the
      * notification was specified, the notification waits that time and
      * then automatically hides
      * @param alignement the alignement on the screen of where to show the notification
      */
    void show(Alignement alignement);
    /**
      * Adds the notification to the current scene if it doesn't have a
      * parent yet, and displays the show animation. If the time for the
      * notification was specified, the notification waits that time and
      * then automatically hides
      * @note it will show the notification at the default notification pos
      */
    void show();

    /**
      * Hide the PositionableNotification. If you passed a time to the create function,
      * this function doesn't need to be called manually, unless you want
      * to prematurily hide the PositionableNotification
      */
    void hide();

    /**
      * Cancels the showing of the PositionableNotification if it's in the queue.
      * Otherwise, it hides the PositionableNotification if it's currently showing.
      */
    void cancel();
};