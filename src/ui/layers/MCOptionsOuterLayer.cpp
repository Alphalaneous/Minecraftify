#include "MCOptionsOuterLayer.h"
#include "MCScrollLayer.h"
#include "../../utils/Utils.h"
#include "../nodes/MCLabel.h"
#include "../nodes/MCButton.h"
#include "../layers/settings/VideoSettings.h"
#include "../layers/settings/GameplaySettings.h"
#include "../layers/settings/PerformanceSettings.h"
#include "../layers/settings/PracticeSettings.h"
#include "../layers/settings/ControlsSettings.h"
#include "../layers/settings/MiscSettings.h"
#include "../layers/settings/HelpSettings.h"
#include "../layers/settings/TutorialSettings.h"
#include "../layers/settings/OnlineSettings.h"
#include "../layers/settings/MusicAndSoundSettings.h"

MCOptionsOuterLayer* MCOptionsOuterLayer::create() {
    auto ret = new MCOptionsOuterLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MCOptionsOuterLayer::init() {

    return true;
}

void MCOptionsOuterLayer::setLayer(CCLayer* layer){
    removeAllChildrenWithCleanup(false);
    addChild(layer);
}


CCScene* MCOptionsOuterLayer::scene() {
    auto layer = MCOptionsOuterLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
