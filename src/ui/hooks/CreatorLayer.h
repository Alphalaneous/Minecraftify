#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../nodes/MCButton.h"
#include "../nodes/MCLabel.h"
#include "../layers/MCScrollLayer.h"
#include "CCMenuItemSpriteExtra.h"

#define convertToMCButton(id, text) \
		if(auto origButton = this->getChildByIDRecursive(id)){\
			if(MyCCMenuItemSpriteExtra* myButton = static_cast<MyCCMenuItemSpriteExtra*>(origButton)){\
				MCButton* button = MCButton::create(text, 38.1f, myButton->m_pListener, myButton->m_pfnSelector);\
				minecraftButtonMenu->addChild(button);\
			}\
		}

class $modify(MyCreatorLayer, CreatorLayer){

	static void onModify(auto& self) {
        (void) self.setHookPriority("CreatorLayer::init", INT_MIN/2-1);
    }

	bool init(){

		if(!CreatorLayer::init()){
			return false;
		}

		if(!Loader::get()->isModLoaded("geode.node-ids")) return true;

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		MCScrollLayer* scrollLayer = MCScrollLayer::create("Online Levels");

		CCLayer* content = CCLayer::create();

		Utils::getNodeSafe(this, "background")->setVisible(false);
		Utils::getNodeSafe(this, "creator-buttons-menu")->setVisible(false);
		Utils::getNodeSafe(this, "bottom-left-corner")->setVisible(false);
		Utils::getNodeSafe(this, "top-left-corner")->setVisible(false);
		Utils::getNodeSafe(this, "treasure-room-button")->setVisible(false);
		Utils::getNodeSafe(this, "vault-button")->setVisible(false);
		Utils::getNodeSafe(this, "exit-button")->setVisible(false);

    	//this->addChild(Utils::generateDirtBG());

		MCButton* createButton = MCButton::create("Create", 38.1f, this, menu_selector(CreatorLayer::onMyLevels));
		MCButton* searchButton = MCButton::create("Search", 38.1f, this, menu_selector(CreatorLayer::onOnlineLevels));

		MCButton* featuredButton = MCButton::create("Featured", 38.1f, this, menu_selector(CreatorLayer::onFeaturedLevels));
		MCButton* dailyButton = MCButton::create("Daily", 38.1f, this, menu_selector(CreatorLayer::onDailyLevel));
		MCButton* weeklyButton = MCButton::create("Weekly", 38.1f, this, menu_selector(CreatorLayer::onWeeklyLevel));
		MCButton* eventButton = MCButton::create("Event", 38.1f, this, menu_selector(CreatorLayer::onEventLevel));
		MCButton* scoresButton = MCButton::create("Leaderboard", 38.1f, this, menu_selector(CreatorLayer::onLeaderboards));
		MCButton* savedButton = MCButton::create("Saved", 38.1f, this, menu_selector(CreatorLayer::onSavedLevels));

		MCButton* questsButton = MCButton::create("Quests", 38.1f, this, menu_selector(CreatorLayer::onChallenge));
		MCButton* gauntletsButton = MCButton::create("Gauntlets", 38.1f, this, menu_selector(CreatorLayer::onGauntlets));
		MCButton* pathsButton = MCButton::create("Paths", 38.1f, this, menu_selector(CreatorLayer::onPaths));
		MCButton* listsButton = MCButton::create("Lists", 38.1f, this, menu_selector(CreatorLayer::onTopLists));
		MCButton* mapPacksButton = MCButton::create("Map Packs", 38.1f, this, menu_selector(CreatorLayer::onMapPacks));
		MCButton* vaultButton = MCButton::create("Vault", 38.1f, this, menu_selector(CreatorLayer::onSecretVault));
		MCButton* treasureButton = MCButton::create("Treasure", 38.1f, this, menu_selector(CreatorLayer::onTreasureRoom));

		float gap = 58.0f;

		CCMenu* importantButtonMenu = CCMenu::create();
		importantButtonMenu->setContentSize({300, gap});
		importantButtonMenu->setPosition({winSize.width/2, 30});
		importantButtonMenu->setID("important-menu"_spr);

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

		minecraftButtonMenu->setContentSize({300, 300});
		minecraftButtonMenu->setPosition({winSize.width/2, gap});
		minecraftButtonMenu->setID("creator-buttons-menu"_spr);

		ColumnLayout* columnLayout2 = ColumnLayout::create();
		columnLayout2->setAxis(Axis::Row);
		columnLayout2->setGap(5);
		columnLayout2->setAutoScale(false);
		columnLayout2->setGrowCrossAxis(true);


		minecraftButtonMenu->setLayout(columnLayout2);
		minecraftButtonMenu->setAnchorPoint({0.5, 1});
		
		minecraftButtonMenu->addChild(dailyButton);
		minecraftButtonMenu->addChild(weeklyButton);
		minecraftButtonMenu->addChild(eventButton);
		minecraftButtonMenu->addChild(pathsButton);
		minecraftButtonMenu->addChild(scoresButton);
		minecraftButtonMenu->addChild(savedButton);
		minecraftButtonMenu->addChild(questsButton);
		minecraftButtonMenu->addChild(gauntletsButton);
		minecraftButtonMenu->addChild(listsButton);
		minecraftButtonMenu->addChild(mapPacksButton);
		minecraftButtonMenu->addChild(featuredButton);
		minecraftButtonMenu->addChild(vaultButton);
		minecraftButtonMenu->addChild(treasureButton);

		convertToMCButton("super-expert-button", "Super Expert");
		convertToMCButton("spaghettdev.gd-roulette/roulette-button", "Roulette");
		convertToMCButton("demon-progression-button", "Demon Progression");
		convertToMCButton("cvolton.betterinfo/main-button", "BetterInfo");

		minecraftButtonMenu->updateLayout();

		content->addChild(minecraftButtonMenu);
		content->addChild(importantButtonMenu);

		
		MCButton* doneButton = MCButton::create("Done", 49.1f, this, menu_selector(CreatorLayer::onBack));

		doneButton->setPosition({winSize.width/2, 16});
		doneButton->setID("done-button"_spr);

   	 	scrollLayer->addChild(doneButton);

		MCLabel* titleText = MCLabel::create("Online Levels", "minecraft.fnt"_spr);
		titleText->setScale(0.4f);
		titleText->setPosition({winSize.width/2, winSize.height-30});

		//this->addChild(titleText);
		titleText->setID("title-text"_spr);

		content->setContentSize({content->getContentSize().width, importantButtonMenu->getContentSize().height + minecraftButtonMenu->getContentSize().height + gap});

		scrollLayer->addContent(content);
		this->addChild(scrollLayer);

		return true;
	}
};