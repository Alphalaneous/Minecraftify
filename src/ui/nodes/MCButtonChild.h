#ifndef _MINECRAFT_BUTTON_CHILD_H_
#define _MINECRAFT_BUTTON_CHILD_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MCButtonChild : public CCMenuItemSprite
{
public:
    CCSprite* edgeTexture;
    static MCButtonChild* create(std::string text, float width, CCObject* target, SEL_MenuHandler selector);
    void update(float dt);
    void onHover();
    void onHoverExit();
    void selected();
    void unselected();
    void activate();
    void doClick();
    void setInvisible();
    void setVisibleFade();
    void setVisibleDelay(float dt);
private:
    bool isHovering = false;
    bool isInvisible = false;
    bool fadeStarted = false;
    float width = 50;
    CCLabelBMFont* label;
};

#endif