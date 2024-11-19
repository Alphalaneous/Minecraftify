#include "GameplaySettings.h"
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"

GameplaySettings* GameplaySettings::create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {
    auto ret = new GameplaySettings();
    if (ret && ret->init(topLayer, prevLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool GameplaySettings::init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer) {

    MCOptionsInnerLayer::init(topLayer, prevLayer);

    auto scrollLayer = MCScrollLayer::create("Gameplay Settings");

    std::vector<std::pair<int, std::string>> values = {
        {26, "Auto Retry"},
        {52, "Faster Restart"},
        {128, "Lock Cursor"},
        {10, "Flip 2 Player Controls"},
        {11, "Always Limit Controls"},
        {28, "Disable Thumbstick"},
        {163, "Quick Keys"}
    };

    //CCLayer* content = Utils::convertMoreOptionsLayer(values);

    //scrollLayer->addContent(content);
    addChild(scrollLayer);

    return true;
}
