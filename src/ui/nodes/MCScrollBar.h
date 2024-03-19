#ifndef _MINECRAFT_SCROLLBAR_H_
#define _MINECRAFT_SCROLLBAR_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MCScrollBar : public CCLayerColor
{
public:

    ScrollLayer* m_scrollLayer;
    float m_height;
    CCNode* m_thumb;
    bool m_touchDown;

    static MCScrollBar* create(ScrollLayer* node, float height);
    bool init(ScrollLayer* node, float height);
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch*, CCEvent*);
    void ccTouchCancelled(CCTouch*, CCEvent*);
    void ccTouchMoved(CCTouch* touch, CCEvent*);
    void update(float dt);
};

#endif