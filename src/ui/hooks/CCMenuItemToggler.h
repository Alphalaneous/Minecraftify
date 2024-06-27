#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemToggler.hpp>

#ifndef MYCCMENUITEMTOGGLER_H
#define MYCCMENUITEMTOGGLER_H

class $modify(MyCCMenuItemToggler, CCMenuItemToggler){

    struct Fields {
        cocos2d::SEL_MenuHandler m_buttonCallback;
        CCObject* m_buttonTarget;
    };

    bool init(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3){
        m_fields->m_buttonCallback = p3;
        m_fields->m_buttonTarget = p2;
        return CCMenuItemToggler::init(p0, p1, p2, p3);
    }
};

#endif