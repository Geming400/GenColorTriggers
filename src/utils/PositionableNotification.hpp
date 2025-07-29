#pragma once

#include <Geode/Geode.hpp>

#include "../hooks/EditorUI.hpp"

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

    Alignement(VerticalAlignement verticalAlignement, HorizontalAlignement horizontalAlignement) : 
        m_verticalAlignement(verticalAlignement),
        m_horizontalAlignement(horizontalAlignement) {}

    /**
     * @note Same as doing `Alignement(VerticalAlignement::MIDDLE, HorizontalAlignement::CENTER)`
     */
    static Alignement createAtCenter() {
        return Alignement(VerticalAlignement::MIDDLE, HorizontalAlignement::CENTER);
    }
};

class PositionableNotification : public Notification {
    protected:
        static CCArray* s_queue;
        bool m_keepAcrossSceneChanges = true;

        void showNextNotification();

    public:
        static CCPoint getMiddleScreenPos();

        /**
         * Create a notification, similar to TextAlertPopup but more customizable
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the
         * notification
         */
        static PositionableNotification* create(
            std::string const& text,
            NotificationIcon icon = NotificationIcon::None,
            float time = NOTIFICATION_DEFAULT_TIME
        ) {
            return static_cast<PositionableNotification*>(Notification::create(text, icon, time));
        };
        /**
         * Create a notification with a custom icon
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the
         * notification
         */
        static PositionableNotification* create(
            std::string const& text,
            cocos2d::CCSprite* icon,
            float time = NOTIFICATION_DEFAULT_TIME
        ) {
            return static_cast<PositionableNotification*>(Notification::create(text, icon, time));
        };

        /**
         * Cancels the showing of the notification if it's in the queue.
         * Otherwise, it hides the notification if it's currently showing.
         */
        void cancel();
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

        void hide();

        void wait();
        void setTime(float time);
        void waitAndHide();

        bool getKeepAcrossSceneChanges();
        void setKeepAcrossSceneChanges(bool value);

        void setTimeMember(float time);
};