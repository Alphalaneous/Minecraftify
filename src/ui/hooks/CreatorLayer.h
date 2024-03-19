#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../nodes/MCButton.h"
#include "../nodes/MCLabel.h"

class $modify(MyCreatorLayer, CreatorLayer){

	bool init(){

		if(!CreatorLayer::init()){
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		CCNode* background = this->getChildByID("background");
		background->setVisible(false);

		this->getChildByID("creator-buttons-menu")->setVisible(false);
		this->getChildByID("bottom-left-corner")->setVisible(false);
		this->getChildByID("top-left-corner")->setVisible(false);
		this->getChildByIDRecursive("treasure-room-button")->setVisible(false);
		this->getChildByIDRecursive("vault-button")->setVisible(false);
		this->getChildByIDRecursive("exit-button")->setVisible(false);

    	this->addChild(Utils::generateDirtBG());

		MCButton* createButton = MCButton::create("Create", 39.1f, this, menu_selector(CreatorLayer::onMyLevels));
		MCButton* searchButton = MCButton::create("Search", 39.1f, this, menu_selector(CreatorLayer::onOnlineLevels));

		MCButton* featuredButton = MCButton::create("Featured", 39.1f, this, menu_selector(CreatorLayer::onFeaturedLevels));
		MCButton* dailyButton = MCButton::create("Daily", 39.1f, this, menu_selector(CreatorLayer::onDailyLevel));
		MCButton* weeklyButton = MCButton::create("Weekly", 39.1f, this, menu_selector(CreatorLayer::onWeeklyLevel));
		MCButton* scoresButton = MCButton::create("Leaderboard", 39.1f, this, menu_selector(CreatorLayer::onLeaderboards));
		MCButton* savedButton = MCButton::create("Saved", 39.1f, this, menu_selector(CreatorLayer::onSavedLevels));

		MCButton* questsButton = MCButton::create("Quests", 39.1f, this, menu_selector(CreatorLayer::onChallenge));
		MCButton* gauntletsButton = MCButton::create("Gauntlets", 39.1f, this, menu_selector(CreatorLayer::onGauntlets));
		MCButton* pathsButton = MCButton::create("Paths", 39.1f, this, menu_selector(CreatorLayer::onPaths));
		MCButton* listsButton = MCButton::create("Lists", 39.1f, this, menu_selector(CreatorLayer::onTopLists));
		MCButton* mapPacksButton = MCButton::create("Map Packs", 39.1f, this, menu_selector(CreatorLayer::onMapPacks));
		MCButton* vaultButton = MCButton::create("Vault", 39.1f, this, menu_selector(CreatorLayer::onSecretVault));
		MCButton* treasureButton = MCButton::create("Treasure", 39.1f, this, menu_selector(CreatorLayer::onTreasureRoom));

		CCMenu* importantButtonMenu = CCMenu::create();
		importantButtonMenu->setContentSize({400, 60});
		importantButtonMenu->setPosition({winSize.width/2, winSize.height - 70});

		ColumnLayout* columnLayout = ColumnLayout::create();
		columnLayout->setAxis(Axis::Row);
		columnLayout->setGap(5);
		columnLayout->setAutoScale(false);
		columnLayout->setGrowCrossAxis(true);

		importantButtonMenu->setLayout(columnLayout);

		importantButtonMenu->addChild(createButton);
		importantButtonMenu->addChild(searchButton);

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

		
		minecraftButtonMenu->addChild(dailyButton);
		minecraftButtonMenu->addChild(weeklyButton);
		minecraftButtonMenu->addChild(scoresButton);
		minecraftButtonMenu->addChild(pathsButton);
		minecraftButtonMenu->addChild(savedButton);
		minecraftButtonMenu->addChild(questsButton);
		minecraftButtonMenu->addChild(gauntletsButton);
		minecraftButtonMenu->addChild(listsButton);
		minecraftButtonMenu->addChild(mapPacksButton);
		minecraftButtonMenu->addChild(featuredButton);
		minecraftButtonMenu->addChild(vaultButton);
		minecraftButtonMenu->addChild(treasureButton);

		minecraftButtonMenu->updateLayout();

		this->addChild(minecraftButtonMenu);
		this->addChild(importantButtonMenu);

		CCMenu* doneMenu = CCMenu::create();
		MCButton* doneButton = MCButton::create("Done", 49.1f, this, menu_selector(CreatorLayer::onBack));
		doneMenu->addChild(doneButton);
		doneMenu->setPosition({winSize.width/2, 50});
		doneMenu->setID("done-menu"_spr);
    	doneMenu->ignoreAnchorPointForPosition(false);

   	 	this->addChild(doneMenu);

		MCLabel* titleText = MCLabel::create("Online Levels", "minecraft.fnt"_spr);
		titleText->setScale(0.4f);
		titleText->setPosition({winSize.width/2, winSize.height-30});

		this->addChild(titleText);
		titleText->setID("title-text"_spr);

		return true;
	}
};