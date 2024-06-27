#pragma once

#include <Geode/Geode.hpp>
#include <random>
#include "../ui/hooks/CCMenuItemSpriteExtra.h"
#include "../ui/hooks/CCMenuItemToggler.h"
#include "../ui/nodes/MCButton.h"

using namespace geode::prelude;

struct ImageData {
    std::string filePath;
    CCImage* img;
    CCTexture2D* texture;
};

class Utils {

public:

    inline static std::vector<ImageData> imageCache;

    static void generateTexture(std::string filePath, GLenum target){

        ImageData imgData;

        bool found = false;

        for(ImageData i : imageCache){
            if(i.filePath == filePath){
                imgData = i;
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

            imgData.filePath = filePath;
            imgData.img = img;
            imgData.texture = texture;

            imageCache.push_back(imgData);
        }

        int pixelsWide = imgData.texture->getPixelsWide();
        int pixelsHigh = imgData.texture->getPixelsHigh();
        
        CCTexture2DPixelFormat pixelFormat = imgData.texture->getPixelFormat();

        auto data = imgData.img->getData();

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
        default:
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

    static CCSprite* generateDirtBG(CCSize size){

	    auto winSize = CCDirector::sharedDirector()->getWinSize();

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* dirt = CCSprite::create("light_dirt_background.png"_spr);

        ccTexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
        dirt->getTexture()->setTexParameters(&params);
        
        dirt->setZOrder(-999);
        dirt->setAnchorPoint({0, 0});
        dirt->setScale(7.5 * scale);
        dirt->setColor({200,200,200});
        dirt->setPosition({0, 0});

        auto rect = dirt->getTextureRect();
        rect.size = rect.size * (CCPoint(size) / CCPoint(dirt->getScaledContentSize()));
        rect.origin = CCPoint{0, winSize.height};
        dirt->setTextureRect(rect);

        return dirt;
    }

    static CCSprite* generateDirtBG(){
        return generateDirtBG(CCDirector::sharedDirector()->getWinSize());
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
            std::filesystem::path path = Mod::get()->getResourcesDir().append("splashes.txt");

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

    static std::string wstrToStr(std::wstring wstr) {
    
        int slength = (int)wstr.length() + 1;
        int len = wcstombs(nullptr, data(wstr), size(wstr));
        std::string r(len, '\0');
        wcstombs(data(r), data(wstr), size(wstr));

        return r;
    }

    static ccColor3B colorForCode(wchar_t code){

        switch(code){

            case '0':
                return {0,0,0};
            case '1':
                return {0,0,170};
            case '2':
                return {0,170,0};
            case '3':
                return {0,170,170};
            case '4':
                return {170,0,0};
            case '5':
                return {170,0,170};
            case '6':
                return {255,170,0};
            case '7':
                return {170,170,170};
            case '8':
                return {85,85,85};
            case '9':
                return {85,85,255};
            case 'a':
                return {85,255,85};
            case 'b':
                return {85,255,255};
            case 'c':
                return {255,85,85};
            case 'd':
                return {255,85,255};
            case 'e':
                return {255,255,85};
            case 'f':
                return {255,255,255};
            default:
                return {255,255,255};
        }
    }

    static CCMenuItemToggler* convertToggler(std::string text, CCMenuItemToggler* toggler){

        CCMenuItemToggler* newToggler = nullptr;

        MCButton* onBtn;
        MCButton* offBtn;

        if(auto btn = static_cast<MyCCMenuItemSpriteExtra*>(toggler->m_onButton)){
            onBtn = MCButton::create(fmt::format("{}: ON", text), 38.1f, btn->m_fields->m_buttonTarget, btn->m_fields->m_buttonCallback);
        }
        if(auto btn = static_cast<MyCCMenuItemSpriteExtra*>(toggler->m_offButton)){
            offBtn = MCButton::create(fmt::format("{}: OFF", text), 38.1f, btn->m_fields->m_buttonTarget, btn->m_fields->m_buttonCallback);
        }
        if(!onBtn || !offBtn) return nullptr;
        if(auto btn = static_cast<MyCCMenuItemToggler*>(toggler)){
            newToggler = CCMenuItemToggler::create(offBtn, onBtn, btn->m_fields->m_buttonTarget, btn->m_fields->m_buttonCallback);
        }

        return newToggler;
    }

    static CCNode* getNodeSafe(CCNode* node, std::string id) {

        if(CCNode* ret = node->getChildByIDRecursive(id)){
            return ret;
        }

        return CCNode::create();
    }
};