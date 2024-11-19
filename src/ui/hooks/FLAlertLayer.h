#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

/*class $modify(MyFLAlertLayer, FLAlertLayer){

    bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7, float p8){
	
        if(!FLAlertLayer::init(p0, p1, p2, p3, p4, p5, p6, p7, p8)){
            return false;
        }

		if(!Loader::get()->isModLoaded("geode.node-ids")) return true;

        CCNode* mainLayer = getChildByID("main-layer");
        
        bool hasScroll = mainLayer->getChildByID("scroll-layer");

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        if(CCScale9Sprite* background = typeinfo_cast<CCScale9Sprite*>(mainLayer->getChildByID("background"))){
            CCScale9Sprite* newBackground = CCScale9Sprite::create("ui_background.png"_spr);
            newBackground->setColor(background->getColor());

            CCSize newBGSize = background->getContentSize() / 4 / scale;

            float sizeIncrease = 0;

            if(!hasScroll){
                sizeIncrease = 10;
            }

            newBackground->setContentSize({newBGSize.width + sizeIncrease, newBGSize.height});
            newBackground->setScale(background->getScale() * 4 * scale);
            newBackground->setPosition({mainLayer->getContentSize().width/2, mainLayer->getContentSize().height/2});
            newBackground->setAnchorPoint(background->getAnchorPoint());
            newBackground->setOpacity(background->getOpacity());
            newBackground->setLayout(background->getLayout());
            newBackground->setLayoutOptions(background->getLayoutOptions());
            newBackground->setID(background->getID());
            newBackground->setRotation(background->getRotation());
            newBackground->setVisible(background->isVisible());
            newBackground->setZOrder(background->getZOrder());

            CCSpriteBatchNode* batchNode = newBackground->getChildByType<CCSpriteBatchNode>(0);
            ccTexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
            batchNode->getTexture()->setTexParameters(&params);

            background->removeFromParent();
            mainLayer->addChild(newBackground);

            CCSize textBGSize = {newBackground->getContentSize().width - 8 / scale, newBackground->getContentSize().height - 24 / scale};

            CCScale9Sprite* textBackground = CCScale9Sprite::create("text_field.png"_spr);
            CCSpriteBatchNode* batchNodeA = textBackground->getChildByType<CCSpriteBatchNode>(0);
            ccTexParams paramsA = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
            batchNodeA->getTexture()->setTexParameters(&paramsA);

            textBackground->setScale(4 * scale);
            textBackground->setContentSize(textBGSize);
            textBackground->setZOrder(5);
            textBackground->setPosition({mainLayer->getContentSize().width/2, mainLayer->getContentSize().height/2});
            mainLayer->addChild(textBackground);

        }

        if(CCNode* contentTextArea = mainLayer->getChildByID("content-text-area")) {
            contentTextArea->setPosition({mainLayer->getContentSize().width/2, mainLayer->getContentSize().height/2});
            contentTextArea->setZOrder(7);
            for(CCLabelBMFont* text : CCArrayExt<CCLabelBMFont*>(contentTextArea->getChildByType<MultilineBitmapFont>(0)->getChildren())){
                text->setFntFile("minecraft.fnt"_spr);
                text->setAnchorPoint({0.5, 0});
                text->setScale(0.6f);
                text->setPosition({0, text->getPosition().y});
                std::string labelText = std::string(text->getString());
                Utils::trim(labelText);
                text->setString(labelText.c_str());
            }
        }

        if(CCNode* scrollingLayer = mainLayer->getChildByID("scroll-layer")) {

            if(CCLayer* contentLayer = scrollingLayer->getChildByType<CCLayer>(1)) {

                if(CCNode* textArea = contentLayer->getChildByID("content-text-area")){
                    textArea->ignoreAnchorPointForPosition(false);
                    textArea->setAnchorPoint({0.5, 0});
                    textArea->setPosition({textArea->getPosition().x, 0});

                    for(CCLabelBMFont* text : CCArrayExt<CCLabelBMFont*>(textArea->getChildByType<MultilineBitmapFont>(0)->getChildren())){
                        text->setFntFile("minecraft.fnt"_spr);
                        text->setAnchorPoint({0.5, 1});
                        text->setScale(0.6f);
                        text->setPosition({0, text->getPosition().y});
                        std::string labelText = std::string(text->getString());
                        Utils::trim(labelText);
                        text->setString(labelText.c_str());
                    }

                    contentLayer->setContentSize(textArea->getScaledContentSize());
                }

                CCSize size = contentLayer->getContentSize();

                ScrollLayer* scrollLayer = ScrollLayer::create({0, 0, mainLayer->getContentSize().width, 180}, true, true);
                scrollLayer->setID("scroll-layer"_spr);
                scrollLayer->setTouchEnabled(true);
                scrollLayer->setZOrder(6);

                contentLayer->setPosition({0, 0});
                contentLayer->setAnchorPoint({0, 0});
                contentLayer->ignoreAnchorPointForPosition(false);
                contentLayer->removeFromParent();
                scrollLayer->m_contentLayer->setContentSize(contentLayer->getContentSize());
                scrollLayer->m_contentLayer->addChild(contentLayer);
            
                scrollLayer->setContentSize({mainLayer->getContentSize().width, 180});
                scrollLayer->setPosition({0, mainLayer->getContentSize().height/2 - scrollLayer->getContentSize().height/2});
                scrollLayer->setScale(scrollingLayer->getScale());

                scrollLayer->moveToTop();

                scrollingLayer->setVisible(false);

                mainLayer->addChild(scrollLayer);
            }
        }

        if(CCLabelBMFont* title = typeinfo_cast<CCLabelBMFont*>(mainLayer->getChildByID("title"))){
            title->setFntFile("minecraft.fnt"_spr);
            title->setScale(0.6f);
            title->setPosition({title->getPosition().x, title->getPosition().y - 6.0f});
        }

       
        if(CCNode* menu = mainLayer->getChildByID("main-menu")){

            RowLayout* rowLayout = RowLayout::create();
            rowLayout->ignoreInvisibleChildren(true);

            CCNode* innerButtonMenu = CCNode::create();

            innerButtonMenu->setContentSize({400, 30});
            innerButtonMenu->setLayout(rowLayout);
            innerButtonMenu->setAnchorPoint({0.5, 0.5});
            innerButtonMenu->setZOrder(1);
            innerButtonMenu->setID("button-menu"_spr);

            mainLayer->addChild(innerButtonMenu);

            if(CCNode* bg = mainLayer->getChildByID("background")) {
                innerButtonMenu->setPosition({bg->getPosition().x, bg->getPosition().y - bg->getScaledContentSize().height/2 + innerButtonMenu->getContentSize().height/2 + 4});
            }
  

            if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(menu->getChildByType<CCMenuItemSpriteExtra>(0))){
                std::string text = "";

                if(ButtonSprite* spr = button->getChildByType<ButtonSprite>(0)) {
                    if(CCLabelBMFont* label = spr->getChildByType<CCLabelBMFont>(0)) {
                        text = std::string(label->getString());
                    }
                }

                MCButton* btn1 = MCButton::create(text.c_str(), 39.1f, button->m_fields->m_buttonTarget, button->m_fields->m_buttonCallback);
                btn1->setID(button->getID());
                button->setVisible(false);
                innerButtonMenu->addChild(btn1);
            }
            if(MyCCMenuItemSpriteExtra* button = static_cast<MyCCMenuItemSpriteExtra*>(menu->getChildByType<CCMenuItemSpriteExtra>(1))){
                std::string text = "";

                if(ButtonSprite* spr = button->getChildByType<ButtonSprite>(0)) {
                    if(CCLabelBMFont* label = spr->getChildByType<CCLabelBMFont>(0)) {
                        text = std::string(label->getString());
                    }
                }
                
                MCButton* btn2 = MCButton::create(text.c_str(), 39.1f, button->m_fields->m_buttonTarget, button->m_fields->m_buttonCallback);
                btn2->setID(button->getID());
                button->setVisible(false);
                innerButtonMenu->addChild(btn2);
            }

            innerButtonMenu->updateLayout();
        }

        handleTouchPriority(this);

        return true;
    }

    void show(){
        FLAlertLayer::show();
        if(CCNode* mainLayer = getChildByID("main-layer")) {
            stopAllActions();
            mainLayer->stopAllActions();
            mainLayer->setScale(1);
            setColor({15, 15, 15});
            setOpacity(200);
        }
    }
};*/