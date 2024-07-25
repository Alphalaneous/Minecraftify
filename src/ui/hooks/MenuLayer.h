#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../../utils/GlobalVars.h"
#include "../../utils/Utils.h"
#include "../nodes/Panorama.h"
#include "../nodes/MCLabel.h"
#include "../nodes/MCButton.h"
#include "../layers/MCOptionsLayer.h"
#include "../layers/MCOptionsOuterLayer.h"
#include "../layers/ExtrasLayer.h"
#include "../layers/settings/VideoSettings.h"

using namespace geode::prelude;

class $modify(MenuLayer){
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}
		if(CCNode* bottomMenu = this->getChildByIDRecursive("bottom-menu")){
			bottomMenu->getChildByIDRecursive("achievements-button")->setVisible(false);
			bottomMenu->getChildByIDRecursive("settings-button")->setVisible(false);
			bottomMenu->getChildByIDRecursive("stats-button")->setVisible(false);
			bottomMenu->getChildByIDRecursive("newgrounds-button")->setVisible(false);
			bottomMenu->getChildByIDRecursive("geode.loader/geode-button")->setVisible(false);
		}
		if(Loader::get()->isModLoaded("alphalaneous.pages_api")){

			Mod* mod = Loader::get()->getLoadedMod("alphalaneous.pages_api");

			if(mod->getSettingValue<bool>("menulayer-right-menu")){
				if(CCNode* rightSideMenu = this->getChildByIDRecursive("right-side-menu")){
					if(AxisLayout* layout = typeinfo_cast<AxisLayout*>(rightSideMenu->getLayout())){
						layout->setAxis(Axis::Row);
					}
					rightSideMenu->setContentSize({170, 70});
					rightSideMenu->setUserObject("orientation", CCInteger::create(1));
					rightSideMenu->removeChildByID("daily-chest-button");

				}
			}
		}
		
		return true;
	}
};

class $modify(MyMenuLayer, MenuLayer) {

	struct Fields {
		CCSize lastSize;
	};

	static void onModify(auto& self) {
        (void) self.setHookPriority("MenuLayer::init", INT_MIN/2-1);
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

		Utils::getNodeSafe(this, "main-menu")->setVisible(false);
		Utils::getNodeSafe(this, "profile-menu")->setVisible(false);
		Utils::getNodeSafe(this, "player-username")->setVisible(false);
		Utils::getNodeSafe(this, "social-media-menu")->setVisible(false);
		Utils::getNodeSafe(this, "close-menu")->setVisible(false);
		Utils::getNodeSafe(this, "bottom-menu")->setVisible(false);
		Utils::getNodeSafe(this, "more-games-menu")->setVisible(false);
		Utils::getNodeSafe(this, "main-menu-bg")->setVisible(false);
		Utils::getNodeSafe(this, "main-menu-bg")->setScale(0);
		Utils::getNodeSafe(this, "right-side-menu")->setVisible(false);
		Utils::getNodeSafe(this, "main-title")->setVisible(false);

		CCMenu* menu = CCMenu::create();

		MCButton* playButton = MCButton::create("Singleplayer", 49.1f, this, menu_selector(MenuLayer::onPlay));
		MCButton* editButton = MCButton::create("Online Levels", 49.1f, this, menu_selector(MenuLayer::onCreator));
		MCButton* modsButton = MCButton::create("Mods", 24.0f, this, menu_selector(MyMenuLayer::onMods));
		MCButton* skinButton = MCButton::create("Skins", 24.0f, this, menu_selector(MenuLayer::onGarage));

		MCButton* optionsButton = MCButton::create("Options...", 24.0f, this, menu_selector(MenuLayer::onOptions));
		MCButton* quitButton = MCButton::create("Quit Game", 24.0f, this, menu_selector(MenuLayer::onQuit));

		MCButton* profileButton = MCButton::create("", 5.0f, this, menu_selector(MyMenuLayer::onMyProfile));
		MCButton* extrasButton = MCButton::create("", 5.0f, this, menu_selector(MyMenuLayer::onExtras));

		CCSprite* profileSprite = Utils::createPixelSprite("teleport_to_player.png"_spr);
		profileSprite->setZOrder(1);
		profileSprite->setPosition({profileButton->getContentSize().width/2, profileButton->getContentSize().height/2});
		profileButton->addSprite(profileSprite);

		CCSprite* extrasSprite = Utils::createPixelSprite("search.png"_spr);
		extrasSprite->setZOrder(1);
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

		MCLabel* versionText = MCLabel::create("Minecraft 2.206 (Geode)", "minecraft.fnt"_spr);
		versionText->setAnchorPoint({0, 0});
		versionText->setScale(0.42f);
		this->addChild(versionText);
		versionText->setID("version-text"_spr);

		MCLabel* creatorName = MCLabel::create("Copyright RobTop Games AB. Do not distribute!", "minecraft.fnt"_spr);
		creatorName->setAnchorPoint({1, 0});
		creatorName->setScale(0.42f);
		this->addChild(creatorName);
		creatorName->setID("copyright-text"_spr);

		std::wstring text = Utils::getSplashText();

		MCLabel* splashText = MCLabel::create(text, "minecraft.fnt"_spr);

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

			CCSprite* titleSprite = Utils::createSprite("mojangstudios.png"_spr);

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
			MCButton* button = typeinfo_cast<MCButton*>(menu->getChildren()->objectAtIndex(i));
			if(button) button->setVisibleFade();
		}
		
	}

	void setInvisible(CCMenu* menu){
		
		for(int i = 0; i < menu->getChildrenCount(); i++){
			MCButton* button = typeinfo_cast<MCButton*>(menu->getChildren()->objectAtIndex(i));
			if(button)button->setInvisible();
		}
		
	}

	void setAllInvisible(){

		CCMenu* menu = typeinfo_cast<CCMenu*>(this->getChildByID("minecraft-menu"_spr));

		CCSprite* title = typeinfo_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-title"_spr));
		CCSprite* subtitle = typeinfo_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-subtitle"_spr));
		MCLabel* splash = typeinfo_cast<MCLabel*>(this->getChildByIDRecursive("splash-text"_spr));
		MCLabel* version = typeinfo_cast<MCLabel*>(this->getChildByID("version-text"_spr));
		MCLabel* creator = typeinfo_cast<MCLabel*>(this->getChildByID("copyright-text"_spr));

		setInvisible(menu);

		splash->setOpacity(0);
		title->setOpacity(0);
		subtitle->setOpacity(0);
		version->setOpacity(0);
		creator->setOpacity(0);
	}

	void setAllVisible(float dt){

		if(!Loader::get()->isModLoaded("geode.node-ids")) {
			createQuickPopup("Node IDs", "Install Node IDs to get the full Minecraftify! experience", "Cancel", "Okay", [](FLAlertLayer*, bool p0){
				if(p0) {
					geode::openModsList();
				}
			}, true);
		}

		CCMenu* menu = typeinfo_cast<CCMenu*>(this->getChildByID("minecraft-menu"_spr));

		Panorama* panorama = typeinfo_cast<Panorama*>(this->getChildByIDRecursive("minecraft-panorama"_spr));

		CCSprite* title = typeinfo_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-title"_spr));
		CCSprite* subtitle = typeinfo_cast<CCSprite*>(this->getChildByIDRecursive("minecraft-subtitle"_spr));
		CCLabelBMFont* splash = typeinfo_cast<CCLabelBMFont*>(this->getChildByIDRecursive("splash-text"_spr));
		CCLabelBMFont* version = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("version-text"_spr));
		CCLabelBMFont* creator = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("copyright-text"_spr));

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
		MCOptionsOuterLayer* outerLayer = MCOptionsOuterLayer::create();
		outerLayer->setLayer(MCOptionsLayer::create(outerLayer));

		CCScene* scene = CCScene::create();
		scene->addChild(outerLayer);
		CCDirector::sharedDirector()->pushScene(scene);
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

		MCButton* playButton = typeinfo_cast<MCButton*>(menu->getChildByID("play-button"_spr));
		MCButton* editButton = typeinfo_cast<MCButton*>(menu->getChildByID("edit-button"_spr));
		MCButton* modsButton = typeinfo_cast<MCButton*>(menu->getChildByID("mods-button"_spr));
		MCButton* skinButton = typeinfo_cast<MCButton*>(menu->getChildByID("skin-button"_spr));
		MCButton* optionsButton = typeinfo_cast<MCButton*>(menu->getChildByID("options-button"_spr));
		MCButton* quitButton = typeinfo_cast<MCButton*>(menu->getChildByID("quit-button"_spr));
		MCButton* profileButton = typeinfo_cast<MCButton*>(menu->getChildByID("profile-button"_spr));
		MCButton* extrasButton = typeinfo_cast<MCButton*>(menu->getChildByID("extras-button"_spr));

		CCSprite* title = typeinfo_cast<CCSprite*>(this->getChildByID("minecraft-title"_spr));
		MCLabel* version = typeinfo_cast<MCLabel*>(this->getChildByID("version-text"_spr));
		MCLabel* creator = typeinfo_cast<MCLabel*>(this->getChildByID("copyright-text"_spr));

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
