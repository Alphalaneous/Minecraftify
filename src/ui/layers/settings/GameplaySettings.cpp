#include "GameplaySettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

GameplaySettings* GameplaySettings::create() {
    auto ret = new GameplaySettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool GameplaySettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Gameplay Settings");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void GameplaySettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void GameplaySettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* GameplaySettings::scene() {
    auto layer = GameplaySettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
