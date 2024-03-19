#include "MiscSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

MiscSettings* MiscSettings::create() {
    auto ret = new MiscSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MiscSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Miscellaneous");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void MiscSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void MiscSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* MiscSettings::scene() {
    auto layer = MiscSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
