#pragma once

#include <Geode/Bindings.hpp>
#include "MCOptionsInnerLayer.h"
#include "MCOptionsOuterLayer.h"

using namespace geode::prelude;

class MCOptionsLayer : public MCOptionsInnerLayer {
protected:
    virtual bool init(MCOptionsOuterLayer* topLayer);
public:

    void onVideoSettings(CCObject* obj);
    void onPerformanceSettings(CCObject* obj);
    void onGameplaySettings(CCObject* obj);
    void onPracticeSettings(CCObject* obj);
    void onControlsSettings(CCObject* obj);
    void onMiscSettings(CCObject* obj);
    void onHelpSettings(CCObject* obj);
    void onTutorialSettings(CCObject* obj);
    void onOnlineSettings(CCObject* obj);
    void onMusicAndSoundSettings(CCObject* obj);
   

    static MCOptionsLayer* create(MCOptionsOuterLayer* topLayer);
    static cocos2d::CCScene* scene(MCOptionsOuterLayer* topLayer);
};