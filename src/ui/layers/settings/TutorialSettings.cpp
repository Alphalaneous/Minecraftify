#include "TutorialSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

TutorialSettings* TutorialSettings::create() {
    auto ret = new TutorialSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool TutorialSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Tutorial");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void TutorialSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void TutorialSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* TutorialSettings::scene() {
    auto layer = TutorialSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
