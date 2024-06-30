#include "PerformanceSettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

PerformanceSettings* PerformanceSettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new PerformanceSettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PerformanceSettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Performance Settings");

    std::vector<std::pair<int, std::string>> values = {
        {66, "Increase Draw Capacity"},
        {108, "Enable Low Detail"},
        {82, "No High Object Alert"},
        {136, "Extra LDM"},
        {42, "Increase Max Levels"},
        {119, "No Level Saving"},
        {127, "Save Gauntlets"},
        {155, "No Shader Antialiasing"}
    };

    CCLayer* content = Utils::convertMoreOptionsLayer(values);

    scrollLayer->addContent(content);

    addChild(scrollLayer);

    return true;
}

