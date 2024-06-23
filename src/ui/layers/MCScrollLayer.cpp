#include "MCScrollLayer.h"
#include "../../utils/Utils.h"
#include "../nodes/MCLabel.h"
#include "../nodes/MCButton.h"
#include "../nodes/MCScrollBar.h"

MCScrollLayer* MCScrollLayer::create(std::string title, float topHeight, float bottomHeight) {
    auto ret = new MCScrollLayer();
    if (ret && ret->init(title, topHeight, bottomHeight)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MCScrollLayer::init(std::string title, float topHeight, float bottomHeight) {

    this->m_title = title;
    this->m_topHeight = topHeight;
    this->m_bottomHeight = bottomHeight;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->addChild(Utils::generateDirtBG());

    ScrollLayer* scrollLayer = ScrollLayer::create({0, bottomHeight, winSize.width, winSize.height - (topHeight + bottomHeight)}, true, true);
    scrollLayer->setID("scroll-layer"_spr);
    scrollLayer->setTouchEnabled(false);

    MCLabel* titleText = MCLabel::create(title, "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-25});

    CCSprite* gradientTop = CCSprite::createWithSpriteFrameName("d_gradient_01_001.png");
    gradientTop->setScaleY(0.15f);
    gradientTop->setScaleX(winSize.width/20);
    gradientTop->setColor({0,0,0});
    gradientTop->setZOrder(10);
    gradientTop->setAnchorPoint({0, 1});
    gradientTop->setPosition({0, winSize.height - topHeight});
    gradientTop->setFlipY(true);

    addChild(gradientTop);

    CCSprite* gradientBottom = CCSprite::createWithSpriteFrameName("d_gradient_01_001.png");
    gradientBottom->setScaleY(0.15f);
    gradientBottom->setScaleX(winSize.width/20);
    gradientBottom->setColor({0,0,0});
    gradientBottom->setZOrder(10);
    gradientBottom->setAnchorPoint({0, 0});
    gradientBottom->setPosition({0, bottomHeight});

    addChild(gradientBottom);

    addChild(titleText);
    titleText->setID("title-text"_spr);

    addChild(scrollLayer);

    

    setKeypadEnabled(true);

    return true;
}



void MCScrollLayer::addContent(CCLayer* content){

    ScrollLayer* scrollLayer = typeinfo_cast<ScrollLayer*>(this->getChildByID("scroll-layer"_spr));

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    bool showScrollBar = true;

    if(content->getContentSize().height <= winSize.height - (m_topHeight + m_bottomHeight)){
        content->setContentSize({winSize.width, winSize.height - (m_topHeight + m_bottomHeight)});
        showScrollBar = false;
    }

    CCSprite* dirtBG = Utils::generateDirtBG({winSize.width, content->getContentSize().height});
    dirtBG->setID("background"_spr);
    dirtBG->setColor({100,100,100});

    content->addChild(dirtBG);
    scrollLayer->m_contentLayer->setContentSize(content->getContentSize());
    scrollLayer->m_contentLayer->addChild(content);
    scrollLayer->moveToTop();

    if(showScrollBar){
        MCScrollBar* scrollBar = MCScrollBar::create(scrollLayer, winSize.height - (m_topHeight + m_bottomHeight));

        scrollBar->setPosition({winSize.width/2+140, winSize.height - m_topHeight});

        addChild(scrollBar);
    }
}

void MCScrollLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void MCScrollLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* MCScrollLayer::scene(std::string title, float topHeight, float bottomHeight) {
    auto layer = MCScrollLayer::create(title, topHeight, bottomHeight);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
