#include "MCOptionsLayer.h"
#include "../../utils/Utils.h"
#include "../../protocols/NewgroundsAlertProtocol.h"
#include "../nodes/MinecraftLabel.h"
#include "../nodes/MinecraftButton.h"

MCOptionsLayer* MCOptionsLayer::create() {
    auto ret = new MCOptionsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MCOptionsLayer::init() {

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->addChild(Utils::generateDirtBG());

    OptionsLayer* optionsLayer = OptionsLayer::create();
    optionsLayer->setVisible(true);

    CCLayer* layer = dynamic_cast<CCLayer*>(optionsLayer->getChildByID("main-layer"));

    layer->setPosition({optionsLayer->getPosition().x, 0});

	layer->getChildByID("left-chain")->setVisible(false);
	layer->getChildByID("right-chain")->setVisible(false);
	layer->getChildByID("background")->setVisible(false);
	layer->getChildByIDRecursive("exit-button")->setVisible(false);

    this->addChild(optionsLayer);

    MinecraftLabel* titleText = MinecraftLabel::create("Options", "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-30});

    this->addChild(titleText);
    titleText->setID("title-text"_spr);

    MinecraftButton* accountButton = MinecraftButton::create("Online...", 39.1f, this, nullptr);
    MinecraftButton* audioButton = MinecraftButton::create("Music & Sounds...", 39.1f, this, nullptr);

    MinecraftButton* graphicsButton = MinecraftButton::create("Video Settings...", 39.1f, this, nullptr);
    MinecraftButton* gameplayButton = MinecraftButton::create("Gameplay Settings...", 39.1f, this, nullptr);
    MinecraftButton* practiceButton = MinecraftButton::create("Practice Settings...", 39.1f, this, nullptr);
    MinecraftButton* controlsButton = MinecraftButton::create("Controls...", 39.1f, this, nullptr);
    MinecraftButton* miscellaneousButton = MinecraftButton::create("Miscellaneous...", 39.1f, this, nullptr);
    
    MinecraftButton* rateButton = MinecraftButton::create("Rate...", 39.1f, this, nullptr);
    MinecraftButton* helpButton = MinecraftButton::create("Help...", 39.1f, this, nullptr);
    MinecraftButton* tutorialButton = MinecraftButton::create("Tutorial...", 49.1f, this, nullptr);

    CCMenu* doneMenu = CCMenu::create();
    MinecraftButton* doneButton = MinecraftButton::create("Done", 49.1f, this, menu_selector(MCOptionsLayer::onBack));
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
    minecraftButtonMenu->addChild(gameplayButton);
    minecraftButtonMenu->addChild(practiceButton);
    minecraftButtonMenu->addChild(controlsButton);
    minecraftButtonMenu->addChild(miscellaneousButton);
    minecraftButtonMenu->addChild(helpButton);
    minecraftButtonMenu->addChild(tutorialButton);

    minecraftButtonMenu->updateLayout();

    this->addChild(minecraftButtonMenu);
    this->addChild(importantButtonMenu);

    setKeypadEnabled(true);

    return true;
}


void MCOptionsLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void MCOptionsLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* MCOptionsLayer::scene() {
    auto layer = MCOptionsLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
