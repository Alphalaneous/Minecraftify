#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../nodes/MinecraftButton.h"
#include "../nodes/MinecraftLabel.h"

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

		MinecraftButton* createButton = MinecraftButton::create("Create", 39.1f, this, menu_selector(CreatorLayer::onMyLevels));
		MinecraftButton* searchButton = MinecraftButton::create("Search", 39.1f, this, menu_selector(CreatorLayer::onOnlineLevels));

		MinecraftButton* featuredButton = MinecraftButton::create("Featured", 39.1f, this, menu_selector(CreatorLayer::onFeaturedLevels));
		MinecraftButton* dailyButton = MinecraftButton::create("Daily", 39.1f, this, menu_selector(CreatorLayer::onDailyLevel));
		MinecraftButton* weeklyButton = MinecraftButton::create("Weekly", 39.1f, this, menu_selector(CreatorLayer::onWeeklyLevel));
		MinecraftButton* scoresButton = MinecraftButton::create("Leaderboard", 39.1f, this, menu_selector(CreatorLayer::onLeaderboards));
		MinecraftButton* savedButton = MinecraftButton::create("Saved", 39.1f, this, menu_selector(CreatorLayer::onSavedLevels));

		MinecraftButton* questsButton = MinecraftButton::create("Quests", 39.1f, this, menu_selector(CreatorLayer::onChallenge));
		MinecraftButton* gauntletsButton = MinecraftButton::create("Gauntlets", 39.1f, this, menu_selector(CreatorLayer::onGauntlets));
		MinecraftButton* pathsButton = MinecraftButton::create("Paths", 39.1f, this, menu_selector(CreatorLayer::onPaths));
		MinecraftButton* listsButton = MinecraftButton::create("Lists", 39.1f, this, menu_selector(CreatorLayer::onTopLists));
		MinecraftButton* mapPacksButton = MinecraftButton::create("Map Packs", 39.1f, this, menu_selector(CreatorLayer::onMapPacks));
		MinecraftButton* vaultButton = MinecraftButton::create("Vault", 39.1f, this, menu_selector(CreatorLayer::onSecretVault));
		MinecraftButton* treasureButton = MinecraftButton::create("Treasure", 39.1f, this, menu_selector(CreatorLayer::onTreasureRoom));

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
		MinecraftButton* doneButton = MinecraftButton::create("Done", 49.1f, this, menu_selector(CreatorLayer::onBack));
		doneMenu->addChild(doneButton);
		doneMenu->setPosition({winSize.width/2, 50});
		doneMenu->setID("done-menu"_spr);
    	doneMenu->ignoreAnchorPointForPosition(false);

   	 	this->addChild(doneMenu);

		MinecraftLabel* titleText = MinecraftLabel::create("Online Levels", "minecraft.fnt"_spr);
		titleText->setScale(0.4f);
		titleText->setPosition({winSize.width/2, winSize.height-30});

		this->addChild(titleText);
		titleText->setID("title-text"_spr);

		return true;
	}
};