#pragma once

#ifndef MYCCMENUITEMSPRITEEXTRA_H
#define MYCCMENUITEMSPRITEEXTRA_H

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra){

    struct Fields {
        cocos2d::SEL_MenuHandler m_buttonCallback;
        CCObject* m_buttonTarget;
        bool m_isMCButton = false;
    };

    bool init(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3){
        m_fields->m_buttonCallback = p3;
        m_fields->m_buttonTarget = p2;
        return CCMenuItemSpriteExtra::init(p0, p1, p2, p3);
    }

    /*void selected(){
        if(!m_fields->m_isMCButton){
            CCMenuItemSpriteExtra::selected();
        }
        else {
            #ifndef GEODE_IS_ANDROID
            FMODAudioEngine::sharedEngine()->playEffect("click.ogg"_spr);
            #endif
            (m_pListener->*m_pfnSelector)(this);
        }
    }

    void unselected(){
        if(!m_fields->m_isMCButton){
            CCMenuItemSpriteExtra::unselected();
        }
    }

    void activate(){
        if(!m_fields->m_isMCButton){
            CCMenuItemSpriteExtra::activate();
        }
    }*/

};

#endif