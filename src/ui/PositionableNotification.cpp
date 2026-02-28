#include <Geode/Geode.hpp>

#include "PositionableNotification.hpp"

using namespace geode::prelude;

constexpr auto NOTIFICATION_FADEIN = .3f;
constexpr auto NOTIFICATION_FADEOUT = 1.f;

CCArray* PositionableNotification::s_queue = nullptr;

bool PositionableNotification::init(ZStringView text, CCSprite* icon, float time) {
    if (!CCNodeRGBA::init()) return false;

    m_time = time;

    m_bg = CCScale9Sprite::create("square02b_small.png", { 0, 0, 40, 40 });
    m_bg->setColor({ 0, 0, 0 });
    this->addChild(m_bg);

    m_label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    m_label->setScale(.6f);
    m_bg->addChild(m_label);

    if ((m_icon = icon)) {
        m_bg->addChild(icon);
    }

    this->setScale(.75f);
    this->updateLayout();

    return true;
}

void PositionableNotification::updateLayout() {
    constexpr auto PADDING = 5.f;
    auto size = m_label->getScaledContentSize();

    float spaceForIcon = 0.f;
    if (m_icon) {
        limitNodeSize(m_icon, { size.height, size.height }, 1.f, .1f);
        spaceForIcon += m_icon->getScaledContentSize().width + PADDING;
    }
    size += CCSize { spaceForIcon + PADDING * 2, PADDING * 2 };
    m_bg->setContentSize(size);

    if (m_icon) {
        m_icon->setPosition({ size.height / 2, size.height / 2 });
        m_label->setPosition(size / 2 + CCSize { spaceForIcon / 2, .0f });
    }
    else {
        m_label->setPosition(size / 2);
    }
}

void PositionableNotification::showNextNotification() {
    m_showing = false;
    if (!s_queue) {
        s_queue = CCArray::create();
        s_queue->retain();
    }
    OverlayManager::get()->removeChild(this);
    // remove self from front of queue
    s_queue->removeFirstObject();
    if (auto obj = s_queue->firstObject()) {
        static_cast<PositionableNotification*>(obj)->show();
    }
    this->removeFromParent();
}

CCSprite* PositionableNotification::createIcon(NotificationIcon icon) {
    switch (icon) {
        default:
        case NotificationIcon::None: {
            return nullptr;
        } break;

        case NotificationIcon::Loading: {
            auto icon = CCSprite::create("loadingCircle.png");
            icon->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
            icon->setBlendFunc({ GL_ONE, GL_ONE });
            return icon;
        } break;

        case NotificationIcon::Success: {
            return CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        } break;

        case NotificationIcon::Warning: {
            return CCSprite::createWithSpriteFrameName("geode.loader/info-alert.png");
        } break;

        case NotificationIcon::Error: {
            return CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
        } break;

        case NotificationIcon::Info: {
            return CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        } break;
    }
}

PositionableNotification* PositionableNotification::create(ZStringView text, NotificationIcon icon, float time) {
    return PositionableNotification::create(text, createIcon(icon), time);
}

PositionableNotification* PositionableNotification::create(ZStringView text, CCSprite* icon, float time) {
    auto ret = new PositionableNotification();
    if (ret->init(text, icon, time)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void PositionableNotification::setString(ZStringView text) {
    m_label->setString(text.c_str());
    this->updateLayout();
}

void PositionableNotification::setIcon(NotificationIcon icon) {
    this->setIcon(createIcon(icon));
}

void PositionableNotification::setIcon(cocos2d::CCSprite* icon) {
    if (m_icon) {
        m_icon->removeFromParent();
    }
    if ((m_icon = icon)) {
        m_bg->addChild(icon);
    }
    this->updateLayout();
}

void PositionableNotification::setTime(float time) {
    m_time = time;
    this->wait();
}

void PositionableNotification::animateIn() {
    m_label->setOpacity(0);
    if (m_icon) {
        m_icon->setOpacity(0);
    }
    m_bg->setOpacity(0);
    m_label->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
    if (m_icon) {
        m_icon->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
    }
    m_bg->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 150));
}

void PositionableNotification::animateOut() {
    m_label->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
    if (m_icon) {
        m_icon->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
    }
    m_bg->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
}

void PositionableNotification::waitAndHide() {
    this->setTime(NOTIFICATION_DEFAULT_TIME);
}

void PositionableNotification::show(CCPoint pos) {
    if (!s_queue) {
        s_queue = CCArray::create();
        s_queue->retain();
    }
    if (!m_showing) {
        if (!s_queue->containsObject(this)) {
            s_queue->addObject(this);
        }
        if (s_queue->firstObject() != this) {
            return;
        }
        if (!this->getParent()) {
            this->setPosition(pos);
            this->setZOrder(CCScene::get()->getChildrenCount() > 0 ? CCScene::get()->getHighestChildZ() + 100 : 100);
        }
        
        OverlayManager::get()->addChild(this);
        m_showing = true;
    }

    this->runAction(CCSequence::create(
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::animateIn)),
        // wait for fade-in to finish
        CCDelayTime::create(.3f),
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::wait)),
        nullptr
    ));
}

void PositionableNotification::show(Alignement alignement) {
    auto winSize = CCDirector::get()->getWinSize();
    CCPoint pos;

    switch (alignement.m_verticalAlignement) {
        case LEFT:
            pos.x = winSize.width * 1/4;
            break;
        case MIDDLE:
            pos.x = winSize.width * 2/4; // I know that we can simplify this to `1/2` but for the readability, i'm not going to
            break;
        case RIGHT:
            pos.x = winSize.width * 3/4;
    }
    switch (alignement.m_horizontalAlignement) {
        case BOTTOM:
            pos.y = winSize.height * 1/4;
            break;
        case CENTER:
            pos.y = winSize.height * 2/4; // I know that we can simplify this to `1/2` but for the readability, i'm not going to
            break;
        case TOP:
            pos.y = winSize.height * 3/4;
    }

    show(pos);
}

void PositionableNotification::show() {
    auto winSize = CCDirector::get()->getWinSize();
    show(ccp(winSize.width / 2, winSize.height / 4));
}

void PositionableNotification::wait() {
    this->stopAllActions();
    if (m_time) {
        this->runAction(CCSequence::create(
            CCDelayTime::create(m_time),
            CCCallFunc::create(this, callfunc_selector(PositionableNotification::hide)),
            nullptr
        ));
    }
}

void PositionableNotification::hide() {
    this->stopAllActions();
    this->runAction(CCSequence::create(
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::animateOut)),
        // wait for fade-out to finish
        CCDelayTime::create(NOTIFICATION_FADEOUT),
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::showNextNotification)),
        nullptr
    ));
}

void PositionableNotification::cancel() {
    if(m_pParent) return this->hide();

    if (s_queue->containsObject(this)) {
        s_queue->removeObject(this);
    }
}