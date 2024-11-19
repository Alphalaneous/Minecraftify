#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class MCOptionsOuterLayer : public CCLayer {
protected:
    virtual bool init();
public:

    CCLayer* m_topLayer;
    void setLayer(CCLayer* layer);
    static MCOptionsOuterLayer* create();
    static cocos2d::CCScene* scene();
};