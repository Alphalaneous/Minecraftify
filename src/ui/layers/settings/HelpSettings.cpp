#include "HelpSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

HelpSettings* HelpSettings::create() {
    auto ret = new HelpSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool HelpSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Help");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void HelpSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void HelpSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* HelpSettings::scene() {
    auto layer = HelpSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
