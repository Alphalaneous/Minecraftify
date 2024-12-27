#pragma once

#include <Geode/Geode.hpp>
#include <random>
#include "../ui/hooks/CCMenuItemSpriteExtra.h"
#include "../ui/hooks/CCMenuItemToggler.h"
#include "../ui/nodes/MCButton.h"

#define public_cast(value, member) [](auto* v) { \
    class FriendClass__; \
    using T = std::remove_pointer<decltype(v)>::type; \
    class FriendeeClass__: public T { \
    protected: \
        friend FriendClass__; \
    }; \
    class FriendClass__ { \
    public: \
        auto& get(FriendeeClass__* v) { return v->member; } \
    } c; \
    return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

using namespace geode::prelude;

struct ImageData {
    std::string filePath;
    CCImage* img;
    CCTexture2D* texture;
};

class Utils {

public:

    inline static std::vector<ImageData> imageCache;

    static void clearImageCache() {
        for(ImageData i : imageCache){
            i.texture->release();
            i.img->release();
        }
        imageCache.clear();
    }

    static void generateTexture(std::string filePath, GLenum target){

        ImageData imgData;

        bool found = false;

        for (ImageData i : imageCache){
            if(i.filePath == filePath){
                imgData = i;
                found = true;
            }
        }

        if (!found){
            CCImage* img = new CCImage();
            img->initWithImageFile(filePath.c_str());
            img->retain();

            CCTexture2D* texture = new CCTexture2D();
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


    static CCSprite* createSprite(std::string path){

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* spr = CCSprite::create(path.c_str());

        spr->setScale(scale);

        return spr;
    }

    static CCSprite* createPanoSprite(std::string path){

        float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

        CCSprite* spr = CCSprite::create(path.c_str());

        spr->setScale(scale);
        spr->setZOrder(-999);

        return spr;
    }

    static CCSprite* createPixelSprite(std::string path){

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

    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
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
            onBtn = MCButton::create(fmt::format("{}: ON", text), 38.1f, btn->m_pListener, btn->m_pfnSelector);
            
        }
        if(auto btn = static_cast<MyCCMenuItemSpriteExtra*>(toggler->m_offButton)){
            offBtn = MCButton::create(fmt::format("{}: OFF", text), 38.1f, btn->m_pListener, btn->m_pfnSelector);
        }
        if(!onBtn || !offBtn) return nullptr;
        if(auto btn = static_cast<MyCCMenuItemToggler*>(toggler)){
            newToggler = CCMenuItemToggler::create(offBtn, onBtn, btn->m_pListener, btn->m_pfnSelector);
            newToggler->setTag(toggler->getTag());
            newToggler->setSizeMult(0);

            auto onBtnToggler = static_cast<MyCCMenuItemSpriteExtra*>(newToggler->m_onButton);
            onBtnToggler->m_fields->m_isMCButton = true;

            auto offBtnToggler = static_cast<MyCCMenuItemSpriteExtra*>(newToggler->m_offButton);
            offBtnToggler->m_fields->m_isMCButton = true;

            if(toggler->isOn()) newToggler->toggle(true);
        }

        return newToggler;
    }

    static std::wstring strToWstr(std::string string){

        const char* str = string.c_str();
        std::wstring wStr = L"";
        for(const char* it = str; *it; ++it) {
            wStr.push_back(std::btowc(*it));
        }

        return wStr;
    }

    static void trim(std::string& str) {
        str.erase(str.find_last_not_of(' ')+1);   
        str.erase(0, str.find_first_not_of(' '));
    }

    static CCNode* getNodeSafe(CCNode* node, std::string id) {

        if(CCNode* ret = node->getChildByIDRecursive(id)){
            return ret;
        }

        return CCNode::create();
    }

    static CCLayer* convertMoreOptionsLayer(std::vector<std::pair<int, std::string>> values){
        CCLayer* content = CCLayer::create();
	    auto winSize = CCDirector::sharedDirector()->getWinSize();

        MoreOptionsLayer* moreOptionsLayer = MoreOptionsLayer::create();
        moreOptionsLayer->setOpacity(0);
        float gap = 8;
        if(auto node = moreOptionsLayer->getChildByIDRecursive("togglers-menu")){
            CCNode* mainLayer = moreOptionsLayer->getChildByID("main-layer");
            mainLayer->removeAllChildren();
            mainLayer->addChild(node);

            node->removeChildByID("key-bindings-button");
            node->removeChildByID("left-arrow-button");
            node->removeChildByID("right-arrow-button");
            node->removeChildByID("close-button");
            node->removeChildByID("parental-control-button");
            node->removeChildByID("fmod-debug-button");
            node->removeChildByID("saved-songs-button");
            node->setPosition({winSize.width/2, -gap});
            node->setContentSize({300, 300});
            node->setAnchorPoint({0.5, 0});

            ColumnLayout* columnLayout = ColumnLayout::create();
            columnLayout->setAxis(Axis::Row);
            columnLayout->setGap(5);
            columnLayout->setAutoScale(false);
            columnLayout->setGrowCrossAxis(true);
            columnLayout->ignoreInvisibleChildren(true);
            node->setLayout(columnLayout);

            for(std::pair<int, std::string> pair : values){

                auto toggler = typeinfo_cast<CCMenuItemToggler*>(node->getChildByID(fmt::format("option-{}-toggler", pair.first)));

                node->addChild(Utils::convertToggler(pair.second, toggler));
                node->updateLayout();

                mainLayer->setContentSize(node->getContentSize());
                moreOptionsLayer->setContentSize(node->getContentSize());
                content->setContentSize({content->getContentSize().width, node->getContentSize().height + gap * 2});
            }
        }
        
        content->addChild(moreOptionsLayer);
        return content;
    }

    static std::string getCPUInfo() {

        #ifdef GEODE_IS_WINDOWS

        std::array<int, 4> integerBuffer = {};
        constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

        std::array<char, 64> charBuffer = {};

        constexpr std::array<unsigned int, 3> functionIds = {
            0x80000002,
            0x80000003,
            0x80000004
        };

        std::string cpu;

        for (int id : functionIds) {
            __cpuid(integerBuffer.data(), id);
            std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
            cpu += std::string(charBuffer.data());
        }

        return cpu;

        #else
        return "(null)";
        #endif
    }
};