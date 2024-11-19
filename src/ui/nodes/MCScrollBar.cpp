#include "MCScrollBar.h"

MCScrollBar* MCScrollBar::create(ScrollLayer* node, float height){
    MCScrollBar *ret = new (std::nothrow) MCScrollBar();
    if (ret && ret->init(node, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MCScrollBar::init(ScrollLayer* node, float height){

    CCLayerColor::init();

    this->m_scrollLayer = node;
    this->m_height = height;
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint({0.5, 1});
    setOpacity(255);
    setColor({0,0,0});

    setContentSize({6, height});

    auto contentHeight = m_scrollLayer->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_scrollLayer->getScaledContentSize().height;

    auto p = targetHeight / contentHeight;

    float thumbHeight = std::min(p, 1.f) * targetHeight;

    CCNode* thumb = CCNode::create();

    CCLayerColor* thumTexA = CCLayerColor::create({0x80,0x80,0x80}, 6, thumbHeight);
    thumTexA->setOpacity(255);

    CCLayerColor* thumTexB = CCLayerColor::create({0xC0,0xC0,0xC0}, 5, thumbHeight-1);
    thumTexB->setPosition({0, 1});
    thumTexB->setOpacity(255);

    thumb->setPosition({0, height});

    thumb->addChild(thumTexA);
    thumb->addChild(thumTexB);
    thumb->setAnchorPoint({0.5, 0.5});
    thumb->setContentSize({6, thumbHeight});

    this->m_thumb = thumb;

    setZOrder(50);

    addChild(thumb);

    this->setTouchEnabled(true);

    this->scheduleUpdate();

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

    return true;
}

void MCScrollBar::update(float dt){

    auto y = m_scrollLayer->m_contentLayer->getPositionY();

    auto contentHeight = m_scrollLayer->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_scrollLayer->getScaledContentSize().height;

    auto h = contentHeight - targetHeight + m_scrollLayer->m_scrollLimitTop;
    auto p = targetHeight / contentHeight;

    float thumbHeight = m_thumb->getContentSize().height;

    float offset = (targetHeight - thumbHeight) * -(y/h);

    float thumbPosY = (m_height / 2 + thumbHeight / 2 + offset) - m_height;

    m_thumb->setPosition(m_obContentSize / 2 + ccp(0.f, thumbPosY));
}

bool MCScrollBar::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    
    auto const size = this->getContentSize();
    auto const pos = this->convertToNodeSpace(touch->getLocation());
    auto const rect = CCRect{0, 0, size.width, size.height};

    if (!m_scrollLayer || !rect.containsPoint(pos)) return false;

    this->ccTouchMoved(touch, event);

    m_touchDown = true;

    return true;
}

void MCScrollBar::ccTouchEnded(CCTouch*, CCEvent*) {
    m_touchDown = false;
}

void MCScrollBar::ccTouchCancelled(CCTouch*, CCEvent*) {
    m_touchDown = false;
}

void MCScrollBar::ccTouchMoved(CCTouch* touch, CCEvent*) {
    
    if (!m_scrollLayer) return;

    auto pos = this->convertToNodeSpace(touch->getLocation());

    auto contentHeight = m_scrollLayer->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_scrollLayer->getScaledContentSize().height;

    auto h = contentHeight - targetHeight + m_scrollLayer->m_scrollLimitTop;
    auto p = targetHeight / contentHeight;

    float thumbHeight = m_thumb->getContentSize().height;

    auto posY = h * ((-pos.y + thumbHeight / 4 - 5) / (targetHeight - thumbHeight / 2 + 10));

    if (posY > 0.0f) posY = 0.0f;
    if (posY < -h) posY = -h;

    m_scrollLayer->m_contentLayer->setPositionY(posY);
}
