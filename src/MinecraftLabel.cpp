#include "MinecraftLabel.h"
#include "Utils.h"

struct ColorPos {
    unsigned int pos;
    wchar_t colorCode;
};

std::string what_the_fuck(std::wstring wstr) {
    
    int slength = (int)wstr.length() + 1;
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, 0, 0);
    std::string r(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, &r[0], len, 0, 0); 
    return r;
}

MinecraftLabel* MinecraftLabel::create(std::string text, std::string font){

    std::wstring ws(text.begin(), text.end());
    return create(ws, font);
}

MinecraftLabel* MinecraftLabel::create(std::wstring text, std::string font){

    MinecraftLabel *ret = new (std::nothrow) MinecraftLabel();

    if (ret && ret->init(text, font))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}



ccColor3B colorForCode(wchar_t code){

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

void MinecraftLabel::setColor(ccColor3B color){
    for(int i = 0; i < getChildrenCount(); i++){
        CCSprite* spr = dynamic_cast<CCSprite*>(getChildren()->objectAtIndex(i)); 
        spr->setColor(color);
    }
}

bool isValidChar(wchar_t c){

    std::wstring s = L"01234567890abcdef";

    return (s.find(c) != std::wstring::npos);
}

bool MinecraftLabel::init(std::wstring text, std::string font){

    std::vector<ColorPos> colorPositions;

    std::wstring finalString = text;

    unsigned int offset = 0;

    for(unsigned int i = 0; i < text.size(); i++) {
        
        if(text.at(i) == L'§'){

            if(i+1 <= text.size() && isValidChar(text.at(i+1))){
                ColorPos p = {i - offset - 1, text.at(i+1)};

                colorPositions.push_back(p);

                finalString = finalString.erase(i - offset - 1, 3);

                offset += 3;
            }
        }
    }

    CCLabelBMFont::initWithString(what_the_fuck(finalString).c_str(), font.c_str());

    for(ColorPos p : colorPositions){

        for(int i = p.pos; i < getChildrenCount(); i++){
            CCSprite* spr = dynamic_cast<CCSprite*>(getChildren()->objectAtIndex(i)); 

            ccColor3B color = colorForCode(p.colorCode);
            spr->setColor(color);
        }
    }

    //fix subpixeling

    for(int i = 0; i < getChildrenCount(); i++){
        CCSprite* spr = dynamic_cast<CCSprite*>(getChildren()->objectAtIndex(i)); 

        Utils::fixSubpixelPosition(spr);
    }

    Utils::fixSubpixelPosition(this);

    return true;
}

void MinecraftLabel::setPosition(CCPoint p){
    CCNode::setPosition(p);

    Utils::fixSubpixelPosition(this);
}