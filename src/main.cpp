#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/CCDirector.hpp>
#include "MinecraftButton.h"
#include <Geode/ui/GeodeUI.hpp>
#include "GlobalVars.h"
#include "ExtrasLayer.h"
#include "Utils.h"
#include "Panorama.h"

using namespace geode::prelude;

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    strings.push_back(str.substr(prev));

    return strings;
}

int random(int min, int max)
{
	static bool first = true;
	if (first) 
	{  
		srand( time(NULL) );
		first = false;
	}
	return min + rand() % (( max + 1 ) - min);
}

std::vector<std::string> splashSplit;
bool hasBeenGenerated = false;

std::string getSplashText(){
	if(!hasBeenGenerated){
		ghc::filesystem::path path = Mod::get()->getResourcesDir().append("splashes.txt");

		std::ifstream input(path.string());
		std::stringstream buffer;
		buffer << input.rdbuf();
		input.close();

		std::string splashText = buffer.str();

		splashSplit = splitString(splashText, "\n");
		hasBeenGenerated = true;
	}

	std::string newSplash = splashSplit.at(random(0, splashSplit.size()-1));

	return newSplash;
}



class $modify(MyMenuLayer, MenuLayer) {
	
	CCSize lastSize;

	static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", -10000);
    }

	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

		Panorama* panorama = Panorama::create();
		panorama->setID("minecraft-panorama"_spr);
		//panorama->setScale(1/scale);
		this->addChild(panorama);

		this->getChildByID("main-menu")->setVisible(false);
		this->getChildByID("profile-menu")->setVisible(false);
		this->getChildByID("player-username")->setVisible(false);
		this->getChildByID("social-media-menu")->setVisible(false);
		this->getChildByID("close-menu")->setVisible(false);
		this->getChildByID("bottom-menu")->setVisible(false);
		this->getChildByID("more-games-menu")->setVisible(false);
		this->getChildByID("main-menu-bg")->setVisible(false);
		this->getChildByID("main-menu-bg")->setScale(0);
		this->getChildByID("right-side-menu")->setVisible(false);
		CCMenu* deathLinkMenu = dynamic_cast<CCMenu*>(this->getChildByID("alphalaneous.deathlink/deathlink-menu"));
		if(deathLinkMenu) deathLinkMenu->setVisible(false);


		CCSprite* mainTitle = dynamic_cast<CCSprite*>(this->getChildByID("main-title"));
		mainTitle->setVisible(false);

		CCMenu* menu = CCMenu::create();

		MinecraftButton* playButton = MinecraftButton::create("Singleplayer", 49.1f, this, menu_selector(MenuLayer::onPlay));
		MinecraftButton* editButton = MinecraftButton::create("Online Levels", 49.1f, this, menu_selector(MenuLayer::onCreator));
		MinecraftButton* modsButton = MinecraftButton::create("Mods", 24.0f, this, menu_selector(MyMenuLayer::onMods));
		MinecraftButton* skinButton = MinecraftButton::create("Skins", 24.0f, this, menu_selector(MenuLayer::onGarage));

		MinecraftButton* optionsButton = MinecraftButton::create("Options...", 24.0f, this, menu_selector(MenuLayer::onOptions));
		MinecraftButton* quitButton = MinecraftButton::create("Quit Game", 24.0f, this, menu_selector(MenuLayer::onQuit));

		MinecraftButton* profileButton = MinecraftButton::create("", 5.0f, this, menu_selector(MyMenuLayer::onMyProfile));
		MinecraftButton* extrasButton = MinecraftButton::create("", 5.0f, this, menu_selector(MyMenuLayer::onExtras));

		CCSprite* profileSprite = Utils::createPixelSprite("language.png"_spr);
		profileSprite->setZOrder(1);
		profileSprite->setPosition({profileButton->getContentSize().width/2, profileButton->getContentSize().height/2});
		profileButton->addChild(profileSprite);

		CCSprite* extrasSprite = Utils::createPixelSprite("select.png"_spr);
		extrasSprite->setZOrder(1);
		extrasSprite->setScale(extrasSprite->getScale()/2);
		extrasSprite->setPosition({extrasButton->getContentSize().width/2, extrasButton->getContentSize().height/2});
		extrasButton->addChild(extrasSprite);


		menu->addChild(playButton);
		playButton->setID("play-button"_spr);
		menu->addChild(editButton);
		editButton->setID("edit-button"_spr);
		menu->addChild(modsButton);
		modsButton->setID("mods-button"_spr);
		menu->addChild(skinButton);
		skinButton->setID("skin-button"_spr);
		menu->addChild(optionsButton);
		optionsButton->setID("options-button"_spr);
		menu->addChild(quitButton);
		quitButton->setID("quit-button"_spr);
		menu->addChild(profileButton);
		profileButton->setID("profile-button"_spr);
		menu->addChild(extrasButton);
		extrasButton->setID("extras-button"_spr);

		menu->ignoreAnchorPointForPosition(false);
		menu->runAction(CCFadeIn::create(1.0f));

		this->addChild(menu);
		menu->setID("minecraft-menu"_spr);

		CCSprite* title = Utils::createPixelSprite("minecraft-title.png"_spr);
		title->setScale(title->getScale()*0.9);
		

		CCSprite* subTitle = Utils::createPixelSprite("edition.png"_spr);
		subTitle->setScale(subTitle->getScale()/scale*0.9);

		subTitle->setPosition({title->getContentSize().width/2, title->getContentSize().height/2 - 11.5f/scale});

		this->addChild(title);
		title->setID("minecraft-title"_spr);

		title->addChild(subTitle);
		subTitle->setID("minecraft-subtitle"_spr);

		CCLabelBMFont* versionText = CCLabelBMFont::create("Minecraft 2.2 (Geode)", "minecraft.fnt"_spr);
		versionText->setAnchorPoint({0, 0});
		versionText->setScale(0.42f);
		this->addChild(versionText);
		versionText->setID("version-text"_spr);

		CCLabelBMFont* creatorName = CCLabelBMFont::create("Copyright RobTop Games AB. Do not distribute!", "minecraft.fnt"_spr);
		creatorName->setAnchorPoint({1, 0});
		creatorName->setScale(0.42f);
		this->addChild(creatorName);
		creatorName->setID("copyright-text"_spr);

		gd::string text = getSplashText();

		CCLabelBMFont* splashText = CCLabelBMFont::create(text.c_str(), "minecraft-gold.fnt"_spr);
		splashText->setPosition({title->getContentSize().width,(title->getContentSize().height) /2});
		splashText->setZOrder(3);
		splashText->setRotation(-20);
		
		float newScale = text.size()/0.1;

		float ratio = std::min(150 / splashText->getContentSize().width, 25 / splashText->getContentSize().height);

		splashText->setScale(ratio/scale);
		title->addChild(splashText);
		splashText->setID("splash-text"_spr);

		float animationLength = 0.25f;

		CCActionInterval* scaleUp = CCEaseSineIn::create(CCScaleBy::create(animationLength, 1.08));
		CCActionInterval* scaleDown = CCEaseSineOut::create(CCScaleBy::create(animationLength, 1/1.08));	

		CCArray* actions = new CCArray();
		actions->addObject(scaleUp);
		actions->addObject(scaleDown);

		CCSequence* moveSeq = CCSequence::create(actions);

		splashText->runAction(CCRepeatForever::create(moveSeq));

		this->scheduleUpdate();

		setPositions(winSize);

		if(GlobalVars::getSharedInstance()->isInitialLaunch){
			setAllInvisible();
			this->scheduleOnce(schedule_selector(MyMenuLayer::setAllVisible), 1.0f);
		}
		CCScheduler::get()->scheduleSelector(schedule_selector(MyMenuLayer::myUpdate), this, 0.0, false);

		return true;
	}


	void setVisible(CCMenu* menu){

		for(int i = 0; i < menu->getChildrenCount(); i++){
			MinecraftButton* button = dynamic_cast<MinecraftButton*>(menu->getChildren()->objectAtIndex(i));
			if(button) button->setVisible();
		}
		
	}

	void setInvisible(CCMenu* menu){
		
		for(int i = 0; i < menu->getChildrenCount(); i++){
			MinecraftButton* button = dynamic_cast<MinecraftButton*>(menu->getChildren()->objectAtIndex(i));
			if(button)button->setInvisible();
		}
		
	}

	void setAllInvisible(){

		CCMenu* menu = dynamic_cast<CCMenu*>(this->getChildByID("minecraft-menu"_spr));

		CCSprite* title = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-title"_spr));
		CCSprite* subtitle = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-subtitle"_spr));
		CCLabelBMFont* splash = dynamic_cast<CCLabelBMFont*>(this->getChildByIDRecursive("splash-text"_spr));
		CCLabelBMFont* version = dynamic_cast<CCLabelBMFont*>(this->getChildByID("version-text"_spr));
		CCLabelBMFont* creator = dynamic_cast<CCLabelBMFont*>(this->getChildByID("copyright-text"_spr));

		setInvisible(menu);

		splash->setOpacity(0);
		title->setOpacity(0);
		subtitle->setOpacity(0);
		version->setOpacity(0);
		creator->setOpacity(0);
	}

	void setAllVisible(float dt){

		CCMenu* menu = dynamic_cast<CCMenu*>(this->getChildByID("minecraft-menu"_spr));

		Panorama* panorama = dynamic_cast<Panorama*>(this->getChildByIDRecursive("minecraft-panorama"_spr));

		CCSprite* title = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-title"_spr));
		CCSprite* subtitle = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-subtitle"_spr));
		CCLabelBMFont* splash = dynamic_cast<CCLabelBMFont*>(this->getChildByIDRecursive("splash-text"_spr));
		CCLabelBMFont* version = dynamic_cast<CCLabelBMFont*>(this->getChildByID("version-text"_spr));
		CCLabelBMFont* creator = dynamic_cast<CCLabelBMFont*>(this->getChildByID("copyright-text"_spr));

		setVisible(menu);

		splash->runAction(CCFadeIn::create(1.0f));
		subtitle->runAction(CCFadeIn::create(1.0f));
		title->runAction(CCFadeIn::create(1.0f));
		version->runAction(CCFadeIn::create(1.0f));
		creator->runAction(CCFadeIn::create(1.0f));


	}


	void onMods(CCObject* sender) {
		geode::openModsList();
	}

	void onMyProfile(CCObject* sender) {

		ProfilePage* page = ProfilePage::create(GJAccountManager::sharedState()->m_accountID, true);
		page->show();
	}

	void onExtras(CCObject* sender) {
		CCScene* scene = ExtrasLayer::scene();
		auto transition = CCTransitionFade::create(0.0f, scene);
		CCDirector::sharedDirector()->pushScene(transition);
	}

	void myUpdate(float dt){

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		float ratio = winSize.width / winSize.height;
		float lastRatio = this->m_fields->lastSize.width / this->m_fields->lastSize.height;

		if(ratio != lastRatio){
			setPositions(winSize);
		}

		this->m_fields->lastSize = winSize;
	}

	void setPositions(CCSize winSize){

		CCNode* menu = this->getChildByID("minecraft-menu"_spr);;

		CCNode* playButton = menu->getChildByID("play-button"_spr);
		CCNode* editButton = menu->getChildByID("edit-button"_spr);
		CCNode* modsButton = menu->getChildByID("mods-button"_spr);
		CCNode* skinButton = menu->getChildByID("skin-button"_spr);
		CCNode* optionsButton = menu->getChildByID("options-button"_spr);
		CCNode* quitButton = menu->getChildByID("quit-button"_spr);
		CCNode* profileButton = menu->getChildByID("profile-button"_spr);
		CCNode* extrasButton = menu->getChildByID("extras-button"_spr);

		CCNode* title = this->getChildByID("minecraft-title"_spr);
		CCNode* version = this->getChildByID("version-text"_spr);
		CCNode* creator = this->getChildByID("copyright-text"_spr);

		playButton->setPosition({winSize.width/2, winSize.height/2+45});
		editButton->setPosition({winSize.width/2, winSize.height/2+23});
		modsButton->setPosition({winSize.width/2 - 44, winSize.height/2+1});
		skinButton->setPosition({winSize.width/2 + 44, winSize.height/2+1});
		optionsButton->setPosition({winSize.width/2 - 44, winSize.height/2-28});
		quitButton->setPosition({winSize.width/2 + 44, winSize.height/2-28});
		profileButton->setPosition({winSize.width/2.0f - 98.5f, winSize.height/2-28});
		extrasButton->setPosition({winSize.width/2.0f + 98.5f, winSize.height/2-28});

		title->setPosition({winSize.width/2.0f, winSize.height-55});
		creator->setPosition({winSize.width-2,0});
		version->setPosition({2,0});

	}

};


class $modify(MyCCDirector, CCDirector){


	bool replaceScene(CCScene *pScene){

		if(!GlobalVars::getSharedInstance()->isInitialLaunch){
			return CCDirector::replaceScene(pScene);
		}
		return true;
	}
};

class $modify(MyLoadingLayer, LoadingLayer){

	float lastWidth = 1;
	float loadingScaleX = 1.5;
	float loadingScaleY = 1.4;
	bool isInitialOpen = true;

	TodoReturn loadingFinished(){
		LoadingLayer::loadingFinished();

		if(GlobalVars::getSharedInstance()->isInitialLaunch){
			this->scheduleOnce(schedule_selector(MyLoadingLayer::doFadeOut), 0.05f);
		}
	}

	void doFadeOut(float dt){
		CCLayerColor* loadingBar = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar"_spr));
		CCLayerColor* loadingBarBG2 = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar-bg2"_spr));

		CCFadeOut* fadeOut = CCFadeOut::create(1.0f);
		loadingBar->runAction(fadeOut);
		CCFadeOut* fadeOut2 = CCFadeOut::create(1.0f);
		loadingBarBG2->runAction(fadeOut2);

		this->scheduleOnce(schedule_selector(MyLoadingLayer::doChangeScene), 1.0f);
		
	}

	void doChangeScene(float dt){

		CCScene* scene = MenuLayer::scene(false);
		auto transition = CCTransitionCrossFade::create(1.0f, scene);
		CCDirector::sharedDirector()->pushScene(transition);
		GlobalVars::getSharedInstance()->isInitialLaunch = false;
	}

	bool init(bool p0){
		if(!LoadingLayer::init(p0)){
			return false;
		}

		CCLabelBMFont* loadingTextOrig = dynamic_cast<CCLabelBMFont*>(this->getChildByID("geode-small-label"));
		loadingTextOrig->setVisible(false);

		this->getChildByID("bg-texture")->setVisible(false);
		this->getChildByID("loading-text")->setVisible(false);
		this->getChildByID("text-area")->setVisible(false);
		this->getChildByID("robtop-logo")->setVisible(false);
		this->getChildByID("gd-logo")->setVisible(false);
		this->getChildByID("fmod-logo")->setVisible(false);
		this->getChildByID("cocos2d-logo")->setVisible(false);
		this->getChildByID("progress-slider")->setVisible(false);
		this->m_sliderBar->setVisible(false);

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		CCLayerColor* layerColor = CCLayerColor::create(ccColor4B{239,50,61,255});
		layerColor->setContentSize(winSize);

		this->addChild(layerColor);

		CCLayerColor* loadingBarBG2 = CCLayerColor::create(ccColor4B{255,255,255,255});
		loadingBarBG2->setContentSize({this->m_fields->loadingScaleX*200+4, this->m_fields->loadingScaleY*5+4});
		loadingBarBG2->ignoreAnchorPointForPosition(false);
		loadingBarBG2->setAnchorPoint({0, 0.5});
		loadingBarBG2->setID("loading-bar-bg2"_spr);
		loadingBarBG2->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX-2, 60});
		this->addChild(loadingBarBG2);

		CCLayerColor* loadingBarBG1 = CCLayerColor::create(ccColor4B{239,50,61,255});
		loadingBarBG1->setContentSize({this->m_fields->loadingScaleX*200+2, this->m_fields->loadingScaleY*5+2});
		loadingBarBG1->ignoreAnchorPointForPosition(false);
		loadingBarBG1->setAnchorPoint({0, 0.5});
		loadingBarBG1->setID("loading-bar-bg1"_spr);

		loadingBarBG1->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX-1.0f, 60});
		this->addChild(loadingBarBG1);

		CCLayerColor* loadingBar = CCLayerColor::create(ccColor4B{255,255,255,255});
		loadingBar->setContentSize({1, 5});
		loadingBar->ignoreAnchorPointForPosition(false);
		loadingBar->setAnchorPoint({0, 0.5});
		loadingBar->setScaleX(this->m_fields->loadingScaleX);
		loadingBar->setScaleY(this->m_fields->loadingScaleY);

		loadingBar->setPosition({winSize.width/2-100*this->m_fields->loadingScaleX, 60});
		loadingBar->setID("loading-bar"_spr);
		this->addChild(loadingBar);

		

		CCSprite* titleSprite = Utils::createSprite("/geode/unzipped/zalphalaneous.minecraft/resources/zalphalaneous.minecraft/mojangstudios.png");

		titleSprite->setPosition({winSize.width/2, winSize.height/2});
		titleSprite->setScale(titleSprite->getScale()*0.3f);

		this->addChild(titleSprite);



		CCLabelBMFont* loadingText = CCLabelBMFont::create("", "/geode/unzipped/zalphalaneous.minecraft/resources/zalphalaneous.minecraft/minecraft.fnt");
		loadingText->setScale(0.4f);
		loadingText->setPosition({winSize.width/2, 30});

		this->addChild(loadingText);
		loadingText->setID("loading-text"_spr);

		CCScheduler::get()->scheduleSelector(schedule_selector(MyLoadingLayer::myUpdate), this, 0.0, false);

		return true;
	}

	void myUpdate(float dt){

		CCLabelBMFont* loadingTextOrig = dynamic_cast<CCLabelBMFont*>(this->getChildByID("geode-small-label"));

		float currentWidth = this->m_sliderBar->getContentSize().width;

		if(currentWidth > 200){
			currentWidth = 200;
		}

		if(currentWidth != this->m_fields->lastWidth ){
			CCLayerColor* loadingBar = dynamic_cast<CCLayerColor*>(this->getChildByID("loading-bar"_spr));
	
			CCScaleTo* scaleTo = CCScaleTo::create(0.1, currentWidth*this->m_fields->loadingScaleX, this->m_fields->loadingScaleY);
			loadingBar->runAction(scaleTo);

			this->m_fields->lastWidth = currentWidth;
		}


		CCLabelBMFont* loadingText = dynamic_cast<CCLabelBMFont*>(this->getChildByID("loading-text"_spr));
		loadingText->setString(loadingTextOrig->getString());
	}

};

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
		MinecraftButton* gauntletsButton = MinecraftButton::create("Guantlets", 39.1f, this, menu_selector(CreatorLayer::onGauntlets));
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

   	 	this->addChild(doneMenu);

		CCLabelBMFont* titleText = CCLabelBMFont::create("Online Levels", "minecraft.fnt"_spr);
		titleText->setScale(0.4f);
		titleText->setPosition({winSize.width/2, winSize.height-30});

		this->addChild(titleText);
		titleText->setID("title-text"_spr);

		return true;
	}
};