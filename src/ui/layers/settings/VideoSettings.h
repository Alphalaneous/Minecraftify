#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class VideoSettings : public cocos2d::CCLayer {
protected:
    virtual bool init(bool fromRefresh);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    bool fromRefresh;
    static VideoSettings* create(bool fromRefresh);
    static cocos2d::CCScene* scene(bool fromRefresh = false);

};