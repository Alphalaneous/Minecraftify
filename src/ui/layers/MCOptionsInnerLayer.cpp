#include "MCOptionsInnerLayer.h"
#include "MCOptionsOuterLayer.h"

MCOptionsInnerLayer::~MCOptionsInnerLayer(){
    if(m_prevLayer) m_prevLayer->release();
}

bool MCOptionsInnerLayer::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer){
    m_topLayer = topLayer;
    m_prevLayer = prevLayer;
    if(m_prevLayer) m_prevLayer->retain();

    setKeypadEnabled(true);
    setKeyboardEnabled(true);

    log::info("start {}", m_prevLayer);

    return true;
}

void MCOptionsInnerLayer::keyBackClicked() {
    log::info("pressed back");
    goBack();
}

void MCOptionsInnerLayer::onBack(CCObject* object) {
    keyBackClicked();
}

void MCOptionsInnerLayer::goBack(){

    m_topLayer->removeAllChildrenWithCleanup(false);
    log::info("{}", m_prevLayer);

    if(m_prevLayer){
        m_topLayer->addChild(m_prevLayer);
    }
    else{
        CCDirector::get()->popScene();
    }
    
}

static MCOptionsInnerLayer* create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer){
    auto ret = new MCOptionsInnerLayer();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}