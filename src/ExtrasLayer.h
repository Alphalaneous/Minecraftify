#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ExtrasLayer : public cocos2d::CCLayer {
protected:
    CCLayer* m_menuLayer;
    CCMenu* m_bottomMenu;

    virtual bool init();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    void update(float dt);
    void onNewgrounds(CCObject*);
    static ExtrasLayer* create();
    static cocos2d::CCScene* scene();
};