#include "ControlsSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

ControlsSettings* ControlsSettings::create() {
    auto ret = new ControlsSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool ControlsSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Controls");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void ControlsSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void ControlsSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* ControlsSettings::scene() {
    auto layer = ControlsSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
