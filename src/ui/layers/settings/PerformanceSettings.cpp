#include "PerformanceSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

PerformanceSettings* PerformanceSettings::create() {
    auto ret = new PerformanceSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PerformanceSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Performance Settings");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void PerformanceSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void PerformanceSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* PerformanceSettings::scene() {
    auto layer = PerformanceSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
