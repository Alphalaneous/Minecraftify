#pragma once

#include <Geode/Geode.hpp>
#include <random>

using namespace geode::prelude;

struct ImageCache{
    std::string filePath;
    CCImage* img;
    CCTexture2D* texture;
};

class Utils {

public:

    inline static std::vector<ImageCache> imageCache;

    static void generateTexture(std::string filePath, GLenum target){

        ImageCache imgCache;

        bool found = false;

        for(ImageCache i : imageCache){
            if(i.filePath == filePath){
                imgCache = i;
                found = true;
                break;
            }
        }

        if(!found){
            CCImage* img = new CCImage;
            img->initWithImageFile(filePath.c_str());
            img->retain();

            CCTexture2D* texture = new CCTexture2D;
            texture->initWithImage(img);
            texture->retain();

            imgCache.filePath = filePath;
            imgCache.img = img;
            imgCache.texture = texture;

            imageCache.push_back(imgCache);
        }

        int pixelsWide = imgCache.texture->getPixelsWide();
        int pixelsHigh = imgCache.texture->getPixelsHigh();
        
        CCTexture2DPixelFormat pixelFormat = imgCache.texture->getPixelFormat();

        auto data = imgCache.img->getData();

        switch(pixelFormat)
        {
        case kCCTexture2DPixelFormat_RGBA8888:
            glTexImage2D(target, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case kCCTexture2DPixelFormat_RGB888:
            glTexImage2D(target, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case kCCTexture2DPixelFormat_RGBA4444:
            glTexImage2D(target, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
            break;
        case kCCTexture2DPixelFormat_RGB5A1:
            glTexImage2D(target, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
            break;
        case kCCTexture2DPixelFormat_RGB565:
            glTexImage2D(target, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            break;
        case kCCTexture2DPixelFormat_AI88:
            glTexImage2D(target, 0, GL_LUMINANCE_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
            break;
        case kCCTexture2DPixelFormat_A8:
            glTexImage2D(target, 0, GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
            break;
        case kCCTexture2DPixelFormat_I8:
            glTexImage2D(target, 0, GL_LUMINANCE, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
            break;
        }
    }


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

        ccTexParams params = {GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
        spr->getTexture()->setTexParameters(&params);

        return spr;
    }

    static void fixSubpixelPosition(CCNode* node){
        /*if(node){
            CCPoint coords = node->getPosition();
            CCPoint location = CCDirector::sharedDirector()->convertToGL(coords);

            float glX = std::floor(location.x);
            float glY = std::floor(location.y);

            CCPoint newLocation = CCDirector::sharedDirector()->convertToUI({glX, glY});

            node->setPosition(newLocation);
        }*/
        
        //todo find a way to fix subpixel issues
    }

    static CCNode* generateDirtBG(){

		auto winSize = CCDirector::sharedDirector()->getWinSize();

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* dirt = CCSprite::create("light_dirt_background.png"_spr);

        // set GL_REPEAT to.. repeat the texture
        ccTexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
        dirt->getTexture()->setTexParameters(&params);
        
        dirt->setZOrder(-999);
        dirt->setAnchorPoint({0, 0});
        dirt->setScale(7.5 * scale);
        dirt->setOpacity(200);
        dirt->setPosition({0, 0});

        auto rect = dirt->getTextureRect();
        rect.size = rect.size * (CCPoint(winSize) / CCPoint(dirt->getScaledContentSize()));
        dirt->setTextureRect(rect);

        return dirt;
    }

    static std::vector<std::wstring> splitString(const std::wstring& str, const std::wstring& delimiter) {
        std::vector<std::wstring> strings;

        std::wstring::size_type pos = 0;
        std::wstring::size_type prev = 0;
        while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
        {
            strings.push_back(str.substr(prev, pos - prev));
            prev = pos + delimiter.size();
        }

        strings.push_back(str.substr(prev));

        return strings;
    }

    static int random(int min, int max) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<uint32_t> chance(min, max);
        
        return chance(rng);
    }

    static bool isMinceraft(){

        static bool isGenerated = false;
        static bool result = false; 
        if(!isGenerated){
            result = random(1, 1000) == 1;
            isGenerated = true;
        }
        return result;
    }

    inline static std::vector<std::wstring> splashSplit;
    inline static bool hasBeenGenerated = false;

    static std::wstring getSplashText(){
        if(!hasBeenGenerated){
            ghc::filesystem::path path = Mod::get()->getResourcesDir().append("splashes.txt");

            std::wifstream input(path.string());
            std::wstringstream buffer;
            buffer << input.rdbuf();
            input.close();

            std::wstring splashText = buffer.str();

            splashSplit = Utils::splitString(splashText, L"\n");
            hasBeenGenerated = true;
        }

        std::wstring newSplash = splashSplit.at(random(0, splashSplit.size()-1));

        return newSplash;
    }
};