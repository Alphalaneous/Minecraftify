#include "MCButton.h"
#include "MCButtonChild.h"

MCButton* MCButton::create(std::string text, float width, CCObject* target, SEL_MenuHandler selector){

    MCButton *ret = new (std::nothrow) MCButton();

    MCButtonChild* child = MCButtonChild::create(text, width, target, selector);
    ret->child = child;
    if (ret && ret->init()) {
        ret->autorelease();
        ret->setContentSize(child->getScaledContentSize());
        ret->addChild(child);
        ret->ignoreAnchorPointForPosition(false);
        ret->scheduleUpdate();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void MCButton::setInvisible(){
    this->child->setInvisible();
}

void MCButton::setVisibleFade(){
    this->child->setVisibleFade();
}

void MCButton::addSprite(CCSprite* sprite){
    sprite->setZOrder(5);
    this->child->addChild(sprite);
}

CCSize MCButton::getContentSize(){
    return this->child->getContentSize();
}