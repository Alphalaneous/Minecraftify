#ifndef _MINECRAFT_BUTTON_H_
#define _MINECRAFT_BUTTON_H_

#include <Geode/Geode.hpp>
#include "MCButtonChild.h"

using namespace geode::prelude;

class MCButton : public CCMenu
{
public:

    bool isBeingTouched = false;

    static MCButton* create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector);
    
    MCButtonChild* child;
    void setInvisible();
    void setVisibleFade();
    void addSprite(CCSprite* sprite);
    CCSize getContentSize();
};

#endif