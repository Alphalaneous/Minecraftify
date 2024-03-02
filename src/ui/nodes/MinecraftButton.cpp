#include "MinecraftButton.h"
#include "MinecraftButtonChild.h"

MinecraftButton* MinecraftButton::create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector){

    MinecraftButton *ret = new (std::nothrow) MinecraftButton();

    MinecraftButtonChild* child = MinecraftButtonChild::create(text, width, target, selector);
    ret->child = child;
    if (ret && ret->init())
    {
        ret->setContentSize(child->getScaledContentSize());
        ret->addChild(child);
        ret->ignoreAnchorPointForPosition(false);
        ret->autorelease();
        ret->scheduleUpdate();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MinecraftButton::setInvisible(){
    this->child->setInvisible();
}

void MinecraftButton::setVisibleFade(){
    this->child->setVisibleFade();
}

void MinecraftButton::addSprite(CCSprite* sprite){
    this->child->addChild(sprite);
}

CCSize MinecraftButton::getContentSize(){
    return this->child->getContentSize();
}