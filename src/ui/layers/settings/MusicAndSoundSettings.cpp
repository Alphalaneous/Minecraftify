#include "MusicAndSoundSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

MusicAndSoundSettings* MusicAndSoundSettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new MusicAndSoundSettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MusicAndSoundSettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Music & Sounds");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    return true;
}
