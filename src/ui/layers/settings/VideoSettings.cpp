#include "VideoSettings.h"
#include <Geode/Geode.hpp>
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"
#include "../../layers/MCOptionsLayer.h"

using namespace geode::prelude;

VideoSettings* VideoSettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new VideoSettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool VideoSettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Video Settings");

    std::vector<std::pair<int, std::string>> values = {
        {24, "Show Cursor In-Game"},
        {135, "Hide Attempts"},
        {15, "Flip Pause Button"},
        {129, "No Portal Guide"},
        {130, "Orb Guide"},
        {140, "No Orb Scale"},
        {141, "No Trigger Orb Scale"},
        {172, "No Shake"},
        {14, "No Explosion Shake"},
        {72, "No Gravity Effect"},
        {60, "Default Mini Icon"},
        {61, "Swap Spider TP Color"},
        {62, "Swap Dash Fire Color"},
        {96, "Swap Wave Trail Color"},
        {174, "Hide Playtest Text"}

    };

    CCLayer* content = Utils::convertMoreOptionsLayer(values);

    scrollLayer->addContent(content);

    addChild(scrollLayer);

    return true;
}
