#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class MCOptionsLayer : public cocos2d::CCLayer {
protected:
    virtual bool init(bool fromRefresh);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    bool fromRefresh;

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
   

    static MCOptionsLayer* create(bool fromRefresh);
    static cocos2d::CCScene* scene(bool fromRefresh = false);
};