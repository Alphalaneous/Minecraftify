#include "MCOptionsLayer.h"
#include "MinecraftButton.h"
#include "Utils.h"
#include "NewgroundsAlertProtocol.h"
#include "MinecraftLabel.h"

MCOptionsLayer* MCOptionsLayer::create() {
    auto ret = new MCOptionsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MCOptionsLayer::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->addChild(Utils::generateDirtBG());

    OptionsLayer* optionsLayer = OptionsLayer::create();
    optionsLayer->setVisible(true);
    optionsLayer->setPosition({optionsLayer->getPosition().x, -winSize.height});

	optionsLayer->getChildByIDRecursive("left-chain")->setVisible(false);
	optionsLayer->getChildByIDRecursive("right-chain")->setVisible(false);
	optionsLayer->getChildByIDRecursive("background")->setVisible(false);
	optionsLayer->getChildByIDRecursive("exit-button")->setVisible(false);

    this->addChild(optionsLayer);

    MinecraftLabel* titleText = MinecraftLabel::create("Options", "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-30});

    this->addChild(titleText);
    titleText->setID("title-text"_spr);

    setKeypadEnabled(true);

    return true;
}


void MCOptionsLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void MCOptionsLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* MCOptionsLayer::scene() {
    auto layer = MCOptionsLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
