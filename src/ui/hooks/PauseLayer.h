#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

class $modify(MyPauseLayer, PauseLayer){

    void customSetup(){
        PauseLayer::customSetup();

		if(!Loader::get()->isModLoaded("geode.node-ids")) return;

        if(CCNode* node = getChildByID("background")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("music-slider")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("sfx-slider")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("music-label")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("sfx-label")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("left-button-menu")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("right-button-menu")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("bottom-button-menu")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("normal-progress-label")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("normal-progress-bar")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("normal-mode-label")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("practice-progress-label")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("practice-progress-bar")) {
            node->setVisible(false);
        }
        if(CCNode* node = getChildByID("practice-mode-label")) {
            node->setVisible(false);
        }
        
        PlayLayer* playLayer = PlayLayer::get();

        if(CCLabelBMFont* title = typeinfo_cast<CCLabelBMFont*>(getChildByID("level-name"))) {
            title->setFntFile("minecraft.fnt"_spr);
            title->setScale(0.45f);
            title->setPosition({title->getPosition().x, title->getPosition().y - 10});

            if(CCLabelBMFont* time = typeinfo_cast<CCLabelBMFont*>(getChildByID("play-time"))) {
                time->setFntFile("minecraft.fnt"_spr);
                time->setScale(0.45f);
                time->setPosition({title->getPosition().x, title->getPosition().y - 15});
            }
            if(CCLabelBMFont* points = typeinfo_cast<CCLabelBMFont*>(getChildByID("points-label"))) {
                points->setFntFile("minecraft.fnt"_spr);
                points->setScale(0.45f);
                points->setPosition({title->getPosition().x, title->getPosition().y - 30});
            }

            if(getChildByID("normal-progress-bar")){

                CCLabelBMFont* normalProgress = typeinfo_cast<CCLabelBMFont*>(getChildByID("normal-progress-label")); 
                CCLabelBMFont* practiceProgress = typeinfo_cast<CCLabelBMFont*>(getChildByID("practice-progress-label")); 

                if(normalProgress && practiceProgress){
                    std::wstring bestText = L"";
                    bestText.append(L"Best: \u00A7a");
                    bestText.append(Utils::strToWstr(normalProgress->getString()));
                    bestText.append(L" \u00A7f| \u00A7b");
                    bestText.append(Utils::strToWstr(practiceProgress->getString()));

                    MCLabel* label = MCLabel::create(bestText, "minecraft.fnt"_spr);
                    label->setScale(0.45f);
                    label->setPosition({title->getPosition().x, title->getPosition().y - 15});
                    addChild(label);
                }
            }
        }

        setColor({15, 15, 15});
        setOpacity(200);

        CCSize winSize = CCDirector::get()->getWinSize();

        RowLayout* rowLayout = RowLayout::create();
        rowLayout->ignoreInvisibleChildren(true);
        rowLayout->setGrowCrossAxis(true);

        CCNode* innerButtonMenu = CCNode::create();

        innerButtonMenu->setContentSize({200, 200});
        innerButtonMenu->setLayout(rowLayout);
        innerButtonMenu->setAnchorPoint({0.5, 0.5});
        innerButtonMenu->setZOrder(1);
        innerButtonMenu->setID("button-menu"_spr);
        innerButtonMenu->setPosition({winSize.width/2, winSize.height-140});

        CCNode* bottomLeftMenu = CCNode::create();
        bottomLeftMenu->setContentSize({20, 20});
        bottomLeftMenu->setLayout(rowLayout);
        bottomLeftMenu->setAnchorPoint({0.5, 0.5});
        bottomLeftMenu->setZOrder(1);
        bottomLeftMenu->setID("bottom-left-menu"_spr);
        bottomLeftMenu->setPosition({30, 30});

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("play-button"))) {
            
            MCButton* btn2 = MCButton::create("Back to Game", 50.0f, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);

        }


        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("practice-button"))) {
            
            std::string practiceText = "Practice";

            if (playLayer->m_isPracticeMode){
                practiceText = "Exit Practice";
            }
            

            MCButton* btn2 = MCButton::create(practiceText.c_str(), 24.3f, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);

        }

        MCButton* statsButton = MCButton::create("Statistics", 24.3f, this, menu_selector(MyPauseLayer::onStatistics));
        innerButtonMenu->addChild(statsButton);

        MCButton* optionButton = MCButton::create("Options...", 24.3f, this, menu_selector(MyPauseLayer::onMCOptions));
        innerButtonMenu->addChild(optionButton);

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("options-button"))) {
            
            MCButton* btn2 = MCButton::create("", 5.0f, button->m_pListener, button->m_pfnSelector);
            CCSprite* spr = Utils::createPixelSprite("accessibility.png"_spr);
            btn2->setID(button->getID());
            spr->setPosition({btn2->getContentSize().width/2, btn2->getContentSize().height/2});
            btn2->addSprite(spr);
            button->setVisible(false);

            bottomLeftMenu->addChild(btn2);
        }

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("retry-button"))) {
            
            MCButton* btn2 = MCButton::create("Retry", 24.3f, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);
        }
        
        float buttonSize = 50.0f;

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("edit-button"))) {

            if(getChildByIDRecursive("full-restart-button")){
                buttonSize = 24.3f;
            }

            MCButton* btn2 = MCButton::create("Edit Level", buttonSize, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);
        }

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("full-restart-button"))) {

            MCButton* btn2 = MCButton::create("Restart Level", buttonSize, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);
        }        

        if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(getChildByIDRecursive("exit-button"))) {
            
            MCButton* btn2 = MCButton::create("Quit to Title", 50.0f, button->m_pListener, button->m_pfnSelector);
            btn2->setID(button->getID());
            button->setVisible(false);
            innerButtonMenu->addChild(btn2);

        }

        innerButtonMenu->updateLayout();
        bottomLeftMenu->updateLayout();

        addChild(innerButtonMenu);
        addChild(bottomLeftMenu);
    }

    void onMCOptions(CCObject* obj){
        OptionsLayer* optionsLayer = OptionsLayer::create();
        optionsLayer->setZOrder(20);
        optionsLayer->showLayer(false);
    }

    void onStatistics(CCObject* obj){

        PlayLayer* playLayer = PlayLayer::get();

        LevelInfoLayer* lil = LevelInfoLayer::create(playLayer->m_level, false);
        lil->onLevelInfo(nullptr);
    }
};