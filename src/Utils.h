#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Utils {

public:

    static CCSprite* createSprite(gd::string path){

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* spr = CCSprite::create(path.c_str());

        spr->setScale(scale);

        return spr;
    }

    static CCSprite* createPanoSprite(gd::string path){

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* spr = CCSprite::create(path.c_str());

        spr->setScale(scale);
        spr->setZOrder(-999);

        return spr;
    }

    static CCSprite* createPixelSprite(gd::string path){

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* spr = CCSprite::create(path.c_str());

        spr->setScale(scale);

        ccTexParams* params = new ccTexParams{GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
        spr->getTexture()->setTexParameters(params);

        return spr;
    }

    static CCNode* generateDirtBG(){

		auto winSize = CCDirector::sharedDirector()->getWinSize();

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCNode* dirtNode = CCNode::create();
        dirtNode->setZOrder(-999);
        float x = 0;
        float y = 0;

        while(true){
            CCSprite* dirt = Utils::createPixelSprite("light_dirt_background.png"_spr);
            dirt->setAnchorPoint({0,0});
            dirt->setScale(7.5 * scale);
            dirt->setOpacity(200);
            dirt->setPosition({x*dirt->getScaledContentSize().width, y*dirt->getScaledContentSize().height});
            dirtNode->addChild(dirt);
            x++;
            if(dirt->getPositionX() + dirt->getScaledContentSize().width > winSize.width){
                x=0;
                y++;
            }
            if(dirt->getPositionY() + dirt->getScaledContentSize().height > winSize.height + dirt->getScaledContentSize().height){
                break;
            }
        }
        return dirtNode;
    }

};