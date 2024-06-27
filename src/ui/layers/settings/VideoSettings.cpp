#include "VideoSettings.h"
#include <Geode/Geode.hpp>
#include "../../../utils/Utils.h"
#include "../../nodes/MCLabel.h"
#include "../../nodes/MCButton.h"
#include "../../nodes/MCScrollBar.h"
#include "../../layers/MCScrollLayer.h"
#include "../../layers/MCOptionsLayer.h"

using namespace geode::prelude;

VideoSettings* VideoSettings::create(bool fromRefresh) {
    auto ret = new VideoSettings();
    if (ret && ret->init(fromRefresh)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool VideoSettings::init(bool fromRefresh) {

    this->fromRefresh = fromRefresh;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = MCScrollLayer::create("Video Settings");

    CCLayer* content = CCLayer::create();

    //MoreOptionsLayer* moreOptionsLayer = new MoreOptionsLayer();
    //moreOptionsLayer->autorelease();
    //moreOptionsLayer->init();

    content->setContentSize({winSize.width, 3000});
    scrollLayer->addContent(content);

    addChild(scrollLayer);

    setKeypadEnabled(true);
    return true;
}

void VideoSettings::keyBackClicked() {
    if(fromRefresh){
        CCDirector::sharedDirector()->pushScene(MCOptionsLayer::scene(true));
    }
    else {
        CCDirector::sharedDirector()->popScene();
    }
}

void VideoSettings::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* VideoSettings::scene(bool fromRefresh) {
    auto layer = VideoSettings::create(fromRefresh);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
