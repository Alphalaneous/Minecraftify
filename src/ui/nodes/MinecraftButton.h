#ifndef _MINECRAFT_BUTTON_H_
#define _MINECRAFT_BUTTON_H_

#include <Geode/Geode.hpp>
#include "MinecraftButtonChild.h"

using namespace geode::prelude;

class MinecraftButton : public CCMenu
{
public:

    bool isBeingTouched = false;

    static MinecraftButton* create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector);
    
    MinecraftButtonChild* child;
    void setInvisible();
    void setVisibleFade();
    void addSprite(CCSprite* sprite);
    CCSize getContentSize();
};

#endif