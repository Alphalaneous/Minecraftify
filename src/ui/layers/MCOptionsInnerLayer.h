#pragma once

#include <Geode/Bindings.hpp>
#include "MCOptionsOuterLayer.h"

using namespace geode::prelude;

class MCOptionsInnerLayer : public cocos2d::CCLayerRGBA {
protected:
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:

    MCOptionsOuterLayer* m_topLayer;
    CCLayer* m_prevLayer;
    ~MCOptionsInnerLayer();
    void goBack();
    virtual bool init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);

    static MCOptionsInnerLayer* create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
};