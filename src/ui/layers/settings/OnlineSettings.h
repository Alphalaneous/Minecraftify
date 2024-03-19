#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class OnlineSettings : public cocos2d::CCLayer {
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:

    static OnlineSettings* create();
    static cocos2d::CCScene* scene();
};