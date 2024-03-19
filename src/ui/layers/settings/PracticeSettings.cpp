#include "PracticeSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

PracticeSettings* PracticeSettings::create() {
    auto ret = new PracticeSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PracticeSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Practice Settings");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void PracticeSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void PracticeSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* PracticeSettings::scene() {
    auto layer = PracticeSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
