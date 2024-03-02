#include "ExtrasLayer.h"
#include "../../utils/Utils.h"
#include "../../protocols/NewgroundsAlertProtocol.h"
#include "../nodes/MinecraftLabel.h"
#include "../nodes/MinecraftButton.h"

ExtrasLayer* ExtrasLayer::create() {
    auto ret = new ExtrasLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool ExtrasLayer::init() {

    
    CCScene* scene = MenuLayer::scene(false);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->addChild(Utils::generateDirtBG());

    m_menuLayer = dynamic_cast<CCLayer*>(scene->getChildren()->objectAtIndex(0));

	CCMenuItemSpriteExtra* deathLinkButton = dynamic_cast<CCMenuItemSpriteExtra*>(m_menuLayer->getChildByIDRecursive("alphalaneous.deathlink/deathlink-button"));

	CCMenu* rightMenu = dynamic_cast<CCMenu*>(m_menuLayer->getChildByID("right-side-menu"));

    dynamic_cast<ColumnLayout*>(rightMenu->getLayout())->setAxis(Axis::Row);

    if(deathLinkButton){
        deathLinkButton->removeFromParent();
        rightMenu->addChild(deathLinkButton);
    }


    rightMenu->removeChildByID("daily-chest-button");

    rightMenu->setContentSize({winSize.width, winSize.height/2});

    rightMenu->updateLayout();
    rightMenu->setPosition({winSize.width/2, 150});
    rightMenu->setVisible(true);
    rightMenu->removeFromParent();
    this->addChild(rightMenu);

    //hacky shit to fix globed

    CCMenu* dummyMenuToFixGlobed = CCMenu::create();
    dummyMenuToFixGlobed->setID("bottom-menu");

    CCSprite* dummySprite = CCSprite::create("select.png"_spr);
    CCMenuItemSpriteExtra* dummyGlobed = CCMenuItemSpriteExtra::create(dummySprite, this, nullptr);
    dummyGlobed->setID("dankmeme.globed2/main-menu-button");

    dummyMenuToFixGlobed->addChild(dummyGlobed);

    m_bottomMenu = dynamic_cast<CCMenu*>(m_menuLayer->getChildByID("bottom-menu"));

    m_bottomMenu->setID("extras-menu"_spr);
    m_bottomMenu->setContentSize({winSize.width, winSize.height/2});

    m_bottomMenu->removeChildByID("achievements-button");
    m_bottomMenu->removeChildByID("settings-button");
    m_bottomMenu->removeChildByID("stats-button");
    m_bottomMenu->removeChildByID("newgrounds-button");

    m_bottomMenu->removeChildByID("geode.loader/geode-button");

    m_bottomMenu->setScale(0.8f);

    m_bottomMenu->updateLayout();
    m_bottomMenu->setPosition({winSize.width/2, 100});
    m_bottomMenu->setVisible(true);
    m_bottomMenu->removeFromParent();
    this->addChild(m_bottomMenu);

    m_menuLayer->addChild(dummyMenuToFixGlobed);

    CCMenu* gdButtons = CCMenu::create();

    ColumnLayout* layout = ColumnLayout::create();
    layout->setAxis(Axis::Column);
    layout->setGap(5);
    layout->setAxisReverse(true);
    layout->setAutoScale(false);
    gdButtons->setLayout(layout);

    gdButtons->setPosition({winSize.width/2, winSize.height-100});

    MinecraftButton* dailyChestButton = MinecraftButton::create("Daily Chests", 49.1f, this, menu_selector(MenuLayer::onDaily));
    MinecraftButton* achievementsButton = MinecraftButton::create("Achievements", 49.1f, this, menu_selector(MenuLayer::onAchievements));
	MinecraftButton* statsButton = MinecraftButton::create("Stats", 49.1f, this, menu_selector(MenuLayer::onStats));
	MinecraftButton* newgroundsButton = MinecraftButton::create("Newgrounds", 49.1f, this, menu_selector(ExtrasLayer::onNewgrounds));

    gdButtons->addChild(dailyChestButton);
    dailyChestButton->setID("daily-chests-button"_spr);

    gdButtons->addChild(achievementsButton);
    achievementsButton->setID("achievements-button"_spr);

    gdButtons->addChild(statsButton);
    statsButton->setID("stats-button"_spr);

    gdButtons->addChild(newgroundsButton);
    newgroundsButton->setID("newgrounds-button"_spr);

    gdButtons->updateLayout();
    gdButtons->setID("gd-menu"_spr);

    this->addChild(gdButtons);

    CCMenu* doneMenu = CCMenu::create();
    MinecraftButton* doneButton = MinecraftButton::create("Done", 49.1f, this, menu_selector(ExtrasLayer::onBack));
    doneMenu->addChild(doneButton);
    doneMenu->setPosition({winSize.width/2, 50});
    doneMenu->ignoreAnchorPointForPosition(false);
    doneMenu->setID("done-menu"_spr);

    this->addChild(doneMenu);

    MinecraftLabel* titleText = MinecraftLabel::create("Extras", "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-30});

    this->addChild(titleText);
    titleText->setID("title-text"_spr);

    setKeypadEnabled(true);

    this->scheduleUpdate();

    return true;
}

void ExtrasLayer::onNewgrounds(CCObject* object){
    auto alertLayer = FLAlertLayer::create(new NewgroundsAlertProtocol, "Newgrounds", "Visit Newgrounds to find awesome music?", "Cancel", "Open", 300);
    alertLayer->show();
}

void ExtrasLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void ExtrasLayer::onBack(CCObject* object) {
    keyBackClicked();
}

//hacky shit to fix globed

void ExtrasLayer::update(float dt){

    if(Loader::get()->isModLoaded("dankmeme.globed2")){

        CCNode* button = m_bottomMenu->getChildByIDRecursive("dankmeme.globed2/main-menu-button");

        if(!button){
            CCScene* scene = ExtrasLayer::scene();
            auto transition = CCTransitionFade::create(0.0f, scene);
            CCDirector::sharedDirector()->replaceScene(transition);
        }
    }
}

CCScene* ExtrasLayer::scene() {
    auto layer = ExtrasLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}
