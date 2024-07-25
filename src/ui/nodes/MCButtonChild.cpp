#include "MCButtonChild.h"
#include "MCLabel.h"
#include "../../utils/Utils.h"

using namespace geode::prelude;

CCSprite* generateEdgeSprite(gd::string textureName){

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    CCSprite* sprite = CCSprite::create(textureName.c_str(), {49/scale,0,1/scale,5/scale});
    sprite->setAnchorPoint({0,0});

    ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    sprite->getTexture()->setTexParameters(params);

    sprite->setContentSize({sprite->getContentSize().width * scale, sprite->getContentSize().height * scale});

    return sprite;
}

CCSprite* generateSprite(MCButtonChild* parent, gd::string textureName, float width){

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

MCButtonChild* MCButtonChild::create(gd::string text, float width, CCObject* target, SEL_MenuHandler selector){

    MCButtonChild *ret = new (std::nothrow) MCButtonChild();
    ret->width = width;

    CCSprite* buttonSprite = generateSprite(ret, "button.png"_spr, width);
   
    MCLabel* label = MCLabel::create(text, "minecraft.fnt"_spr);
    label->setScale(0.12f);
    label->setZOrder(2);

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


void setSpritesInvisible(CCNode* node){
    for(int i = 0; i < node->getChildrenCount(); i++){
        CCSprite* spr = typeinfo_cast<CCSprite*>(node->getChildren()->objectAtIndex(i));
        if(spr){
            spr->setOpacity(0);
            setSpritesInvisible(spr);
        }
    }
}

void setSpritesVisible(CCNode* node){

    for(int i = 0; i < node->getChildrenCount(); i++){
        CCSprite* spr = typeinfo_cast<CCSprite*>(node->getChildren()->objectAtIndex(i));
        if(spr){
            spr->runAction(CCFadeIn::create(1.0f));
            setSpritesVisible(spr);
        }
    }
}

void MCButtonChild::setInvisible(){
    this->isInvisible = true;
    this->label->setOpacity(0);
    setSpritesInvisible(this);

}

void MCButtonChild::setVisibleFade(){

    if(!this->fadeStarted){
        this->fadeStarted = true;
        this->label->runAction(CCFadeIn::create(1.0f));
        setSpritesVisible(this);
        this->scheduleOnce(schedule_selector(MCButtonChild::setVisibleDelay), 1.0f);
    }

}

void MCButtonChild::setVisibleDelay(float dt){
    this->isInvisible = false;
    this->fadeStarted = false;
}

void MCButtonChild::onHover(){
    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite(this, "button_highlighted.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MCButtonChild::onHoverExit(){

    if(!isInvisible){
        CCSprite* buttonSprite = generateSprite(this, "button.png"_spr, width);
        this->setNormalImage(buttonSprite);
    }
}

void MCButtonChild::unselected(){
    onHoverExit();
}

void MCButtonChild::doClick(){
    bool doAndroidBypass = false;

    #ifdef GEODE_IS_ANDROID
    doAndroidBypass = true;
    #endif


    if(this->isHovering || doAndroidBypass){

        auto engine = FMODAudioEngine::sharedEngine();
        auto system = engine->m_system;

        FMOD::Channel* channel;
        FMOD::Sound* sound;

        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("click.ogg"_spr, false);;
        if(engine->m_sfxVolume > 0) {
            system->createSound(fullPath.c_str(), FMOD_DEFAULT, nullptr, &sound);
            system->playSound(sound, nullptr, false, &channel);
            channel->setVolume(engine->m_sfxVolume);
        }

        geode::Loader::get()->queueInMainThread([this]() { //delay it by a frame because for some reason it crashes the touch dispatcher otherwise ???
            if(m_pListener && m_pfnSelector){
                (m_pListener->*m_pfnSelector)(this);
            }
        });
    }
}

void MCButtonChild::selected(){

    onHover();

    #ifndef GEODE_IS_ANDROID
    doClick();
    #endif
}

void MCButtonChild::activate(){
    #ifdef GEODE_IS_ANDROID
    doClick();
    #endif
}

void MCButtonChild::update(float dt) {

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

