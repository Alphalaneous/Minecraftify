#include "MinecraftButtonChild.h"
#include "MinecraftLabel.h"
#include "../../utils/Utils.h"

CCSprite* generateEdgeSprite(gd::string textureName){

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    CCSprite* sprite = CCSprite::create(textureName.c_str(), {49/scale,0,1/scale,5/scale});
    sprite->setAnchorPoint({0,0});

    ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    sprite->getTexture()->setTexParameters(params);

    sprite->setContentSize({sprite->getContentSize().width * scale, sprite->getContentSize().height * scale});

    return sprite;
}

CCSprite* generateSprite(MinecraftButtonChild* parent, gd::string textureName, float width){

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    CCSprite* sprite = CCSprite::create(textureName.c_str(), {0,0,width/scale-1.0f/scale,5/scale});

    ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    sprite->getTexture()->setTexParameters(params);
    sprite->setScale(scale);

    parent->removeChildByID("edge"_spr);

    CCSprite* edgeSprite = generateEdgeSprite(textureName);
    edgeSprite->setPositionX(width-1);
    edgeSprite->setZOrder(sprite->getZOrder()+1);
    edgeSprite->setID("edge"_spr);
    edgeSprite->setScale(scale);

    sprite->setContentSize({sprite->getContentSize().width * scale + edgeSprite->getContentSize().width , sprite->getContentSize().height * scale});
    
    parent->edgeTexture = edgeSprite;
    parent->addChild(edgeSprite);

    return sprite;
}

MinecraftButtonChild* MinecraftButtonChild::create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector){

    MinecraftButtonChild *ret = new (std::nothrow) MinecraftButtonChild();
    ret->width = width;

    CCSprite* buttonSprite = generateSprite(ret, "button.png"_spr, width);
   
    MinecraftLabel* label = MinecraftLabel::create(text, "minecraft.fnt"_spr);
    label->setScale(0.12f);
    label->setZOrder(1);

    if (ret && ret->initWithNormalSprite(buttonSprite, nullptr, nullptr, target, selector))
    {
        ret->autorelease();
        ret->scheduleUpdate();
        ret->setScale(3.5);
        ret->addChild(label);
        ret->setAnchorPoint({0,0});
        ret->label = label;
        label->setPosition({ret->getContentSize().width/2, ret->getContentSize().height/2});

        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MinecraftButtonChild::setPosition(CCPoint p){
    CCNode::setPosition(p);
    Utils::fixSubpixelPosition(this);
}

void setSpritesInvisible(CCNode* node){
    for(int i = 0; i < node->getChildrenCount(); i++){
        CCSprite* spr = dynamic_cast<CCSprite*>(node->getChildren()->objectAtIndex(i));
        if(spr){
            spr->setOpacity(0);
            setSpritesInvisible(spr);
        }
    }
}

void setSpritesVisible(CCNode* node){

    for(int i = 0; i < node->getChildrenCount(); i++){
        CCSprite* spr = dynamic_cast<CCSprite*>(node->getChildren()->objectAtIndex(i));
        if(spr){
            spr->runAction(CCFadeIn::create(1.0f));
            setSpritesVisible(spr);
        }
    }
}

void MinecraftButtonChild::setInvisible(){
    this->isInvisible = true;
    this->label->setOpacity(0);
    setSpritesInvisible(this);

}

void MinecraftButtonChild::setVisibleFade(){

    if(!this->fadeStarted){
        this->fadeStarted = true;
        this->label->runAction(CCFadeIn::create(1.0f));
        setSpritesVisible(this);
        this->scheduleOnce(schedule_selector(MinecraftButtonChild::setVisibleDelay), 1.0f);
    }

}

void MinecraftButtonChild::setVisibleDelay(float dt){
    this->isInvisible = false;
    this->fadeStarted = false;
}

void MinecraftButtonChild::onHover(){
    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite(this, "button_highlighted.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MinecraftButtonChild::onHoverExit(){

    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite(this, "button.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MinecraftButtonChild::selected(){
    if(this->isHovering){
        FMODAudioEngine::sharedEngine()->playEffect("click.ogg"_spr);
        CCMenuItem::activate();
    }
}

void MinecraftButtonChild::activate(){
    //do nothing
}

void MinecraftButtonChild::update(float dt) {

    auto mousePos = getMousePos();

    auto pos = getParent()->convertToWorldSpace(getPosition());
    auto size = getContentSize();
    auto scale = getScale();
    auto anchorPoint = getAnchorPoint();

    float anchorXSubtract = size.width * scale * anchorPoint.x;
    float anchorYSubtract = size.height * scale * anchorPoint.y;

    CCRect rect = CCRect(pos.x - anchorXSubtract, pos.y - anchorYSubtract, size.width * scale, size.height * scale);

    if(rect.containsPoint(mousePos)){
        if(!isHovering){
            onHover();
            isHovering = true;
        }
    }
    else{
        if(isHovering){
            onHoverExit();
            isHovering = false;
        }
    }
}

