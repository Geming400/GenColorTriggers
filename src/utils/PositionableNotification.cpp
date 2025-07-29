#include "PositionableNotification.hpp"

CCArray* PositionableNotification::s_queue = nullptr;

void PositionableNotification::cancel() {
    if(m_pParent) return this->hide();

    if (s_queue->containsObject(this)) {
        s_queue->removeObject(this);
    }
}

void PositionableNotification::showNextNotification() {
    m_showing = false;
    if (!s_queue) {
        s_queue = CCArray::create();
        s_queue->retain();
    }

    SceneManager::get()->forget(this);
    // remove self from front of queue
    s_queue->removeFirstObject();
    if (auto obj = s_queue->firstObject()) {
        static_cast<PositionableNotification*>(obj)->show();
    }
    this->removeFromParent();
}

void PositionableNotification::show(CCPoint pos) {
    log::info("Showing notif");
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
        
        SceneManager::get()->keepAcrossScenes(this);
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

void PositionableNotification::hide() {
    this->stopAllActions();
    this->runAction(CCSequence::create(
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::animateOut)),
        // wait for fade-out to finish
        CCDelayTime::create(1.f),
        CCCallFunc::create(this, callfunc_selector(PositionableNotification::showNextNotification)),
        nullptr
    ));
}

void PositionableNotification::animateIn() {
    m_label->setOpacity(0);
    if (m_icon) {
        m_icon->setOpacity(0);
    }
    m_bg->setOpacity(0);
    m_label->runAction(CCFadeTo::create(.3f, 255));
    if (m_icon) {
        m_icon->runAction(CCFadeTo::create(.3f, 255));
    }
    m_bg->runAction(CCFadeTo::create(.3f, 150));
}

void PositionableNotification::wait() {
    this->stopAllActions();
    if (m_time) {
        log::info("m_time = {}", m_time);
        this->runAction(CCSequence::create(
            CCDelayTime::create(m_time),
            CCCallFunc::create(this, callfunc_selector(PositionableNotification::hide)),
            nullptr
        ));
    }
}
void PositionableNotification::setTime(float time) {
    m_time = time;
    this->wait();
}
void PositionableNotification::waitAndHide() {
    this->setTime(NOTIFICATION_DEFAULT_TIME);
}

CCPoint PositionableNotification::getMiddleScreenPos() {
    auto winSize = CCDirector::get()->getWinSize();
    return ccp(winSize.width / 2, winSize.height / 2);
}

void PositionableNotification::setTimeMember(float time) {
    m_time = time;
}