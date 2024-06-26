#include "ExtrasLayer.h"
#include "../../utils/Utils.h"
#include "../nodes/MCLabel.h"
#include "../nodes/MCButton.h"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

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
    m_menuLayer = typeinfo_cast<CCLayer*>(scene->getChildren()->objectAtIndex(0));

    if(Loader::get()->isModLoaded("alphalaneous.pages_api")){
        CCMenu* rightMenu = typeinfo_cast<CCMenu*>(m_menuLayer->getChildByID("right-side-menu"));
        rightMenu->setPosition({winSize.width/2, 150});
        rightMenu->setVisible(true);
        rightMenu->removeFromParent();

        this->addChild(rightMenu);

        m_bottomMenu = typeinfo_cast<CCMenu*>(m_menuLayer->getChildByID("bottom-menu"));

        m_bottomMenu->setID("extras-menu"_spr);
        m_bottomMenu->setPosition({winSize.width/2, 100});
        m_bottomMenu->setVisible(true);
        m_bottomMenu->setScale(0.8f);
        m_bottomMenu->removeFromParent();

        this->addChild(m_bottomMenu);
    }
    else{

        CCMenu* rightMenu = typeinfo_cast<CCMenu*>(m_menuLayer->getChildByID("right-side-menu"));

        typeinfo_cast<ColumnLayout*>(rightMenu->getLayout())->setAxis(Axis::Row);

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

        m_bottomMenu = typeinfo_cast<CCMenu*>(m_menuLayer->getChildByID("bottom-menu"));

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
        this->scheduleUpdate();
    }

    CCMenu* gdButtons = CCMenu::create();

    RowLayout* layout = RowLayout::create();
    layout->setAxis(Axis::Row);
    layout->setGap(5);
    layout->setAxisReverse(true);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    gdButtons->setLayout(layout);
    gdButtons->setContentSize({350, 100});
    gdButtons->setPosition({winSize.width/2, winSize.height-100});

    MCButton* dailyChestButton = MCButton::create("Daily Chests", 39.1f, this, menu_selector(MenuLayer::onDaily));
    MCButton* achievementsButton = MCButton::create("Achievements", 39.1f, this, menu_selector(MenuLayer::onAchievements));
    MCButton* statsButton = MCButton::create("Stats", 39.1f, this, menu_selector(MenuLayer::onStats));
    MCButton* newgroundsButton = MCButton::create("Newgrounds", 39.1f, this, menu_selector(ExtrasLayer::onNewgrounds));
    MCButton* moreGamesButton = MCButton::create("More Games", 39.1f, this, menu_selector(MenuLayer::onMoreGames));

    gdButtons->addChild(dailyChestButton);
    dailyChestButton->setID("daily-chests-button"_spr);

    gdButtons->addChild(achievementsButton);
    achievementsButton->setID("achievements-button"_spr);

    gdButtons->addChild(statsButton);
    statsButton->setID("stats-button"_spr);

    gdButtons->addChild(newgroundsButton);
    newgroundsButton->setID("newgrounds-button"_spr);

    gdButtons->addChild(moreGamesButton);
    moreGamesButton->setID("more-games-button"_spr);

    gdButtons->updateLayout();
    gdButtons->setID("gd-menu"_spr);

    this->addChild(gdButtons);

    CCMenu* doneMenu = CCMenu::create();
    MCButton* doneButton = MCButton::create("Done", 49.1f, this, menu_selector(ExtrasLayer::onBack));
    doneMenu->addChild(doneButton);
    doneMenu->setPosition({winSize.width/2, 50});
    doneMenu->ignoreAnchorPointForPosition(false);
    doneMenu->setID("done-menu"_spr);

    this->addChild(doneMenu);

    MCLabel* titleText = MCLabel::create("Extras", "minecraft.fnt"_spr);
    titleText->setScale(0.4f);
    titleText->setPosition({winSize.width/2, winSize.height-30});

    this->addChild(titleText);
    titleText->setID("title-text"_spr);

    setKeypadEnabled(true);

    return true;
}

void ExtrasLayer::onNewgrounds(CCObject* object){

    createQuickPopup("Newgrounds", "Visit Newgrounds to find awesome music?", "Cancel", "Open", [](FLAlertLayer*, bool button2){
        if(button2){
            web::openLinkInBrowser("https://newgrounds.com/");
        }
    });
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
