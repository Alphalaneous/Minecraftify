#include "MusicAndSoundSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

MusicAndSoundSettings* MusicAndSoundSettings::create() {
    auto ret = new MusicAndSoundSettings();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MusicAndSoundSettings::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Music & Sounds");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void MusicAndSoundSettings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void MusicAndSoundSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* MusicAndSoundSettings::scene() {
    auto layer = MusicAndSoundSettings::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
