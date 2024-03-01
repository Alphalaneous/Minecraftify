#include "MinecraftButton.h"

CCSprite* generateEdgeSprite(gd::string textureName){

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    CCSprite* sprite = CCSprite::create(textureName.c_str(), {49/scale,0,1/scale,5/scale});
    sprite->setAnchorPoint({0,0});

    ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    sprite->getTexture()->setTexParameters(params);

    sprite->setContentSize({sprite->getContentSize().width * scale, sprite->getContentSize().height * scale});

    return sprite;
}

CCSprite* generateSprite(gd::string textureName, float width){

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    CCSprite* sprite = CCSprite::create(textureName.c_str(), {0,0,width/scale-1.0f/scale,5/scale});

    ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    sprite->getTexture()->setTexParameters(params);
    sprite->setScale(scale);


    CCSprite* edgeSprite = generateEdgeSprite(textureName);
    edgeSprite->setPositionX(width/scale-1/scale);
    edgeSprite->setZOrder(sprite->getZOrder()+1);
    edgeSprite->setID("edge"_spr);

    sprite->setContentSize({sprite->getContentSize().width * scale + edgeSprite->getContentSize().width , sprite->getContentSize().height * scale});

    sprite->addChild(edgeSprite);

    return sprite;
}

MinecraftButton* MinecraftButton::create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector){

    MinecraftButton *ret = new (std::nothrow) MinecraftButton();
    ret->width = width;

    CCSprite* buttonSprite = generateSprite("button.png"_spr, width);
    ret->edgeTexture = dynamic_cast<CCSprite*>(buttonSprite->getChildByID("edge"_spr));
   
    CCLabelBMFont* label = CCLabelBMFont::create(text.c_str(), "minecraft.fnt"_spr);
    label->setScale(0.12f);
    label->setZOrder(1);

    if (ret && ret->initWithNormalSprite(buttonSprite, nullptr, nullptr, target, selector))
    {
        ret->autorelease();
        ret->scheduleUpdate();
        ret->setScale(3.5);
        ret->init();
        ret->addChild(label);
        ret->label = label;
        label->setPosition({ret->getContentSize().width/2, ret->getContentSize().height/2});

        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
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
            setSpritesInvisible(spr);
        }
    }
}

void MinecraftButton::setInvisible(){
    this->isInvisible = true;
    this->setOpacity(0);
    this->label->setOpacity(0);
    this->edgeTexture->setOpacity(0);
    setSpritesInvisible(this);

}

void MinecraftButton::setVisible(){
    this->runAction(CCFadeIn::create(1.0f));
    this->label->runAction(CCFadeIn::create(1.0f));
    this->edgeTexture->runAction(CCFadeIn::create(1.0f));

    setSpritesVisible(this);
	this->scheduleOnce(schedule_selector(MinecraftButton::setVisibleDelay), 1.0f);

}

void MinecraftButton::setVisibleDelay(float dt){
    this->isInvisible = false;
}

void MinecraftButton::onHover(){

    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite("button_highlighted.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MinecraftButton::onHoverExit(){

    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite("button.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MinecraftButton::selected(){
    FMODAudioEngine::sharedEngine()->playEffect("click.ogg"_spr);
}

void MinecraftButton::update(float dt) {

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

