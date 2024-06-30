#include "MiscSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

MiscSettings* MiscSettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new MiscSettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MiscSettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Miscellaneous");

    CCLayer* content = CCLayer::create();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    return true;
}
