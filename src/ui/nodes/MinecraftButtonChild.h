#ifndef _MINECRAFT_BUTTON_CHILD_H_
#define _MINECRAFT_BUTTON_CHILD_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MinecraftButtonChild : public CCMenuItemSprite
{
public:
    CCSprite* edgeTexture;
    static MinecraftButtonChild* create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector);
    void update(float dt);
    void onHover();
    void onHoverExit();
    void selected();
    void activate();

    void setInvisible();
    void setVisibleFade();
    void setVisibleDelay(float dt);
    void setPosition(CCPoint p);
private:
    bool isHovering = false;
    bool isInvisible = false;
    bool fadeStarted = false;
    float width = 50;
    CCLabelBMFont* label;
};

#endif