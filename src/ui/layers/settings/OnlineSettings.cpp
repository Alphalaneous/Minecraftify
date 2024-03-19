#include "OnlineSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

OnlineSettings* OnlineSettings::create() {
    auto ret = new OnlineSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool OnlineSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Online");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void OnlineSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void OnlineSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* OnlineSettings::scene() {
    auto layer = OnlineSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
