#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemToggler.hpp>

using namespace geode::prelude;

class $modify(MyCCMenuItemToggler, CCMenuItemToggler){

    struct Fields {
        cocos2d::SEL_MenuHandler m_buttonCallback;
        CCObject* m_buttonTarget;
    };

    static CCMenuItemToggler* create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3){
        auto ret = CCMenuItemToggler::create(p0, p1, p2, p3);
        auto myRet = static_cast<MyCCMenuItemToggler*>(ret);

        myRet->m_fields->m_buttonCallback = p3;
        myRet->m_fields->m_buttonTarget = p2;
        return ret;
    }
};