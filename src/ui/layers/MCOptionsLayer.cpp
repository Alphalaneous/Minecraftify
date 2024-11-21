#include "MCOptionsLayer.h"
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

MCOptionsLayer* MCOptionsLayer::create(MCOptionsOuterLayer* topLayer) {
    auto ret = new MCOptionsLayer();
    if (ret && ret->init(topLayer)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

CCNode* safeGet(std::string id, CCNode* layer){
    CCNode* finalNode = CCNode::create();
    if(auto node = layer->getChildByIDRecursive(id)){
        finalNode = node;
    }
    return finalNode;
}


bool MCOptionsLayer::init(MCOptionsOuterLayer* topLayer) {

    MCOptionsInnerLayer::init(topLayer, nullptr);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->m_topLayer->setContentSize(winSize);

    this->setID("settings-layer"_spr);

    this->addChild(Utils::generateDirtBG());

    MCLabel* titleText = MCLabel::create("Options", "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-30});

    this->addChild(titleText);
    titleText->setID("title-text"_spr);

    //OptionsLayer* optionsLayer = OptionsLayer::create();
    //optionsLayer->setKeyboardEnabled(false);
    //optionsLayer->setKeypadEnabled(false);
    //addChild(optionsLayer);

    MCButton* accountButton = MCButton::create("Online...", 39.1f, this, menu_selector(MCOptionsLayer::onOnlineSettings));
    MCButton* audioButton = MCButton::create("Music & Sounds...", 39.1f, this, menu_selector(MCOptionsLayer::onMusicAndSoundSettings));

    MCButton* graphicsButton = MCButton::create("Video Settings...", 39.1f, this, menu_selector(MCOptionsLayer::onVideoSettings));
    MCButton* performanceButton = MCButton::create("Performance Settings...", 39.1f, this, menu_selector(MCOptionsLayer::onPerformanceSettings));
    MCButton* gameplayButton = MCButton::create("Gameplay Settings...", 39.1f, this, menu_selector(MCOptionsLayer::onGameplaySettings));
    MCButton* practiceButton = MCButton::create("Practice Settings...", 39.1f, this, menu_selector(MCOptionsLayer::onPracticeSettings));

    MCButton* controlsButton = MCButton::create("Controls...", 39.1f, this, menu_selector(MCOptionsLayer::onControlsSettings));
    MCButton* miscellaneousButton = MCButton::create("Miscellaneous...", 39.1f, this, menu_selector(MCOptionsLayer::onMiscSettings));
    
    //MCButton* rateButton = MCButton::create("Rate...", 39.1f, this, menu_selector(MCOptionsLayer::onVideoSettings));
    MCButton* helpButton = MCButton::create("Help...", 39.1f, this, menu_selector(MCOptionsLayer::onHelpSettings));
    MCButton* tutorialButton = MCButton::create("Tutorial...", 39.1f, this, menu_selector(MCOptionsLayer::onTutorialSettings));

    CCMenu* doneMenu = CCMenu::create();
    MCButton* doneButton = MCButton::create("Done", 49.1f, this, menu_selector(MCOptionsLayer::onBack));
    doneMenu->addChild(doneButton);
    doneMenu->setPosition({winSize.width/2, 50});
    doneMenu->ignoreAnchorPointForPosition(false);
    doneMenu->setID("done-menu"_spr);

    this->addChild(doneMenu);

	CCMenu* importantButtonMenu = CCMenu::create();
    importantButtonMenu->setContentSize({400, 60});
    importantButtonMenu->setPosition({winSize.width/2, winSize.height - 70});

    ColumnLayout* columnLayout = ColumnLayout::create();
    columnLayout->setAxis(Axis::Row);
    columnLayout->setGap(5);
    columnLayout->setAutoScale(false);
    columnLayout->setGrowCrossAxis(true);

    importantButtonMenu->setLayout(columnLayout);

    importantButtonMenu->addChild(accountButton);
    importantButtonMenu->addChild(audioButton);

    importantButtonMenu->updateLayout();

    CCMenu* minecraftButtonMenu = CCMenu::create();

    minecraftButtonMenu->setContentSize({400, 300});
    minecraftButtonMenu->setPosition({winSize.width/2, winSize.height - 170});

    ColumnLayout* columnLayout2 = ColumnLayout::create();
    columnLayout2->setAxis(Axis::Row);
    columnLayout2->setGap(5);
    columnLayout2->setAutoScale(false);
    columnLayout2->setGrowCrossAxis(true);


    minecraftButtonMenu->setLayout(columnLayout2);

    
    minecraftButtonMenu->addChild(graphicsButton);
    minecraftButtonMenu->addChild(performanceButton);
    minecraftButtonMenu->addChild(gameplayButton);
    minecraftButtonMenu->addChild(practiceButton);
    minecraftButtonMenu->addChild(controlsButton);
    minecraftButtonMenu->addChild(miscellaneousButton);
    minecraftButtonMenu->addChild(helpButton);
    minecraftButtonMenu->addChild(tutorialButton);

    minecraftButtonMenu->updateLayout();

    this->addChild(minecraftButtonMenu);
    this->addChild(importantButtonMenu);

    return true;
}

void MCOptionsLayer::onVideoSettings(CCObject* obj){
    m_topLayer->setLayer(VideoSettings::create(m_topLayer, this));
}

void MCOptionsLayer::onPerformanceSettings(CCObject* obj){
    m_topLayer->setLayer(PerformanceSettings::create(m_topLayer, this));
}

void MCOptionsLayer::onGameplaySettings(CCObject* obj){
    m_topLayer->setLayer(GameplaySettings::create(m_topLayer, this));
}

void MCOptionsLayer::onPracticeSettings(CCObject* obj){
    m_topLayer->setLayer(PracticeSettings::create(m_topLayer, this));
}

void MCOptionsLayer::onControlsSettings(CCObject* obj){
    m_topLayer->setLayer(ControlsSettings::create(m_topLayer, this));
}

void MCOptionsLayer::onMiscSettings(CCObject* obj){
    m_topLayer->setLayer(MiscSettings::create(m_topLayer, this));
}

void MCOptionsLayer::onHelpSettings(CCObject* obj){

    OptionsLayer* optionsLayer = OptionsLayer::create();
    optionsLayer->setKeyboardEnabled(false);
    optionsLayer->setKeypadEnabled(false);

    this->addChild(optionsLayer);

    //(optionsLayer->*menu_selector(OptionsLayer::onHelp))(this);

    //CCDirector::sharedDirector()->pushScene(HelpSettings::scene());
}

void MCOptionsLayer::onTutorialSettings(CCObject* obj){

    //OptionsLayer* optionsLayer = OptionsLayer::create();
    //this->addChild(optionsLayer);

    //(this->*menu_selector(OptionsLayer::onSupport))(this);

    //CCDirector::sharedDirector()->pushScene(TutorialSettings::scene());
}

void MCOptionsLayer::onOnlineSettings(CCObject* obj){
    OptionsLayer* optionsLayer = OptionsLayer::create();
    optionsLayer->setKeyboardEnabled(false);
    optionsLayer->setKeypadEnabled(false);
    
    this->addChild(optionsLayer);

    //(optionsLayer->*menu_selector(OptionsLayer::onAccount))(this);

    //CCDirector::sharedDirector()->pushScene(OnlineSettings::scene());
}

void MCOptionsLayer::onMusicAndSoundSettings(CCObject* obj){
    m_topLayer->setLayer(MusicAndSoundSettings::create(m_topLayer, this));
}

CCScene* MCOptionsLayer::scene(MCOptionsOuterLayer* topLayer) {
    auto layer = MCOptionsLayer::create(topLayer);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
