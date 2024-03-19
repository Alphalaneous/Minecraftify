#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class MCScrollLayer : public cocos2d::CCLayer {
protected:
    virtual bool init(std::string title, float topHeight, float bottomHeight);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:

    float m_topHeight;
    float m_bottomHeight;
    std::string m_title;

    static MCScrollLayer* create(std::string title, float topHeight = 34, float bottomHeight = 34);
    static cocos2d::CCScene* scene(std::string title, float topHeight = 34, float bottomHeight = 34);
    void addContent(CCLayer* content);
};