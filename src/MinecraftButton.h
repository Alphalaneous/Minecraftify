#ifndef _MINECRAFT_BUTTON_H_
#define _MINECRAFT_BUTTON_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MinecraftButton : public CCMenuItemSprite
{
public:

    static MinecraftButton* create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector);
    void update(float dt);
    void onHover();
    void onHoverExit();
    void selected();
    void setInvisible();
    void setVisibleFade();
    void setVisibleDelay(float dt);
    void setPosition(CCPoint p);
private:
    bool isHovering = false;
    bool isInvisible = false;
    float width = 50;
    CCSprite* edgeTexture;
    CCLabelBMFont* label;
};

#endif