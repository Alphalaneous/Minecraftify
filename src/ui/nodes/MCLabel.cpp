#include "MCLabel.h"
#include "../../utils/Utils.h"
#include <cstdlib>
#include <iostream>

struct ColorPos {
    unsigned int pos;
    wchar_t colorCode;
};

MCLabel* MCLabel::create(std::string text, std::string font){

    std::wstring ws(text.begin(), text.end());
    return create(ws, font);
}

MCLabel* MCLabel::create(std::wstring text, std::string font){

    MCLabel *ret = new (std::nothrow) MCLabel();

    if (ret && ret->init(text, font))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void MCLabel::setColor(ccColor3B color){
    for(int i = 0; i < getChildrenCount(); i++){
        CCSprite* spr = typeinfo_cast<CCSprite*>(getChildren()->objectAtIndex(i)); 
        spr->setColor(color);
    }
}

bool isValidChar(wchar_t c){

    std::wstring s = L"01234567890abcdef";

    return (s.find(c) != std::wstring::npos);
}

bool MCLabel::init(std::wstring text, std::string font){

    std::vector<ColorPos> colorPositions;

    std::wstring finalString = text;

    unsigned int offset = 0;

    for(unsigned int i = 0; i < text.size(); i++) {
        
        if(text.at(i) == L'\u00A7'){

            if(i+1 <= text.size() && isValidChar(text.at(i+1))){
                ColorPos p = {i - offset, text.at(i+1)};

                colorPositions.push_back(p);


                finalString = finalString.erase(i - offset, 2);

                offset += 2;
            }
        }
    }

    CCLabelBMFont::initWithString(Utils::wstrToStr(finalString).c_str(), font.c_str());

    for(ColorPos p : colorPositions){

        for(int i = p.pos; i < getChildrenCount(); i++){
            CCSprite* spr = typeinfo_cast<CCSprite*>(getChildren()->objectAtIndex(i)); 

            ccColor3B color = Utils::colorForCode(p.colorCode);
            spr->setColor(color);
        }
    }

    return true;
}
