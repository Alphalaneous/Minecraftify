#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../../utils/GlobalVars.h"
#include "../../utils/Utils.h"
#include "../nodes/Panorama.h"
#include "../nodes/MinecraftLabel.h"
#include "../nodes/MinecraftButton.h"
#include "../layers/MCOptionsLayer.h"
#include "../layers/ExtrasLayer.h"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	
	CCSize lastSize;

	static void onModify(auto& self) {
        (void) self.setHookPriority("MenuLayer::init", -10000);
    }

	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

		Panorama* panorama = Panorama::create();
		panorama->setID("minecraft-panorama"_spr);
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
		profileSprite->setScale(profileSprite->getScale());
		profileSprite->setPosition({profileButton->getContentSize().width/2, profileButton->getContentSize().height/2});
		profileButton->addSprite(profileSprite);

		CCSprite* extrasSprite = Utils::createPixelSprite("select.png"_spr);
		extrasSprite->setZOrder(1);
		extrasSprite->setScale(extrasSprite->getScale()/2);
		extrasSprite->setPosition({extrasButton->getContentSize().width/2, extrasButton->getContentSize().height/2});
		extrasButton->addSprite(extrasSprite);


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

		this->addChild(menu);
		menu->setID("minecraft-menu"_spr);

		auto titleSprite = "minecraft-title.png"_spr;
		if (Utils::isMinceraft()) {
			titleSprite = "minceraft-title.png"_spr;
		}

		CCSprite* title = Utils::createPixelSprite(titleSprite);
		title->setScale(title->getScale()*0.9);
		

		CCSprite* subTitle = Utils::createPixelSprite("edition.png"_spr);
		subTitle->setScale(subTitle->getScale()/scale*0.9);

		subTitle->setPosition({title->getContentSize().width/2, title->getContentSize().height/2 - 11.5f/scale});

		this->addChild(title);
		title->setID("minecraft-title"_spr);

		title->addChild(subTitle);
		subTitle->setID("minecraft-subtitle"_spr);

		MinecraftLabel* versionText = MinecraftLabel::create("Minecraft 2.2 (Geode)", "minecraft.fnt"_spr);
		versionText->setAnchorPoint({0, 0});
		versionText->setScale(0.42f);
		this->addChild(versionText);
		versionText->setID("version-text"_spr);

		MinecraftLabel* creatorName = MinecraftLabel::create("Copyright RobTop Games AB. Do not distribute!", "minecraft.fnt"_spr);
		creatorName->setAnchorPoint({1, 0});
		creatorName->setScale(0.42f);
		this->addChild(creatorName);
		creatorName->setID("copyright-text"_spr);

		std::wstring text = Utils::getSplashText();

		MinecraftLabel* splashText = MinecraftLabel::create(text, "minecraft.fnt"_spr);
		splashText->setColor({255,255,0});
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

			CCLayerColor* layerColor = CCLayerColor::create(ccColor4B{239,50,61,255});
			layerColor->setID("loading-bg"_spr);
			layerColor->setContentSize(winSize);

			this->addChild(layerColor);

			CCSprite* titleSprite = Utils::createSprite("/geode/unzipped/zalphalaneous.minecraft/resources/zalphalaneous.minecraft/mojangstudios.png");

			titleSprite->setPosition({winSize.width/2, winSize.height/2});
			titleSprite->setScale(titleSprite->getScale()*0.3f);
			titleSprite->setID("title-sprite"_spr);

			this->addChild(titleSprite);

			layerColor->runAction(CCFadeOut::create(1.0f));
			titleSprite->runAction(CCFadeOut::create(1.0f));
			
			setAllInvisible();
			this->scheduleOnce(schedule_selector(MyMenuLayer::setAllVisible), 1.0f);
		}


		CCScheduler::get()->scheduleSelector(schedule_selector(MyMenuLayer::myUpdate), this, 0.0, false);

		return true;
	}

	void setVisible(CCMenu* menu){

		for(int i = 0; i < menu->getChildrenCount(); i++){
			MinecraftButton* button = dynamic_cast<MinecraftButton*>(menu->getChildren()->objectAtIndex(i));
			if(button) button->setVisibleFade();
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
		MinecraftLabel* splash = dynamic_cast<MinecraftLabel*>(this->getChildByIDRecursive("splash-text"_spr));
		MinecraftLabel* version = dynamic_cast<MinecraftLabel*>(this->getChildByID("version-text"_spr));
		MinecraftLabel* creator = dynamic_cast<MinecraftLabel*>(this->getChildByID("copyright-text"_spr));

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

	void onMCOptions(CCObject* sender) {
		CCScene* scene = MCOptionsLayer::scene();
		auto transition = CCTransitionFade::create(0.0f, scene);
		CCDirector::sharedDirector()->pushScene(transition);
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

		MinecraftButton* playButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("play-button"_spr));
		MinecraftButton* editButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("edit-button"_spr));
		MinecraftButton* modsButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("mods-button"_spr));
		MinecraftButton* skinButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("skin-button"_spr));
		MinecraftButton* optionsButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("options-button"_spr));
		MinecraftButton* quitButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("quit-button"_spr));
		MinecraftButton* profileButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("profile-button"_spr));
		MinecraftButton* extrasButton = dynamic_cast<MinecraftButton*>(menu->getChildByID("extras-button"_spr));

		CCSprite* title = dynamic_cast<CCSprite*>(this->getChildByID("minecraft-title"_spr));
		MinecraftLabel* version = dynamic_cast<MinecraftLabel*>(this->getChildByID("version-text"_spr));
		MinecraftLabel* creator = dynamic_cast<MinecraftLabel*>(this->getChildByID("copyright-text"_spr));

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
