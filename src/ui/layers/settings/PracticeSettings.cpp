#include "PracticeSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

PracticeSettings* PracticeSettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new PracticeSettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PracticeSettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Practice Settings");

    std::vector<std::pair<int, std::string>> values = {
        {71, "Hide Practice Buttons"},
        {134, "Hide Attempts"},
        {27, "Auto Checkpoints"},
        {68, "Quick Checkpoints"},
        {100, "Death Effects"},
        {125, "Editor Normal Music"},
        {166, "Show Hitboxes"},
        {171, "No Player Hitbox"}
    };

    //CCLayer* content = Utils::convertMoreOptionsLayer(values);

    //scrollLayer->addContent(content);

    addChild(scrollLayer);

    return true;
}

