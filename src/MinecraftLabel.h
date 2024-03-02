#ifndef _MINECRAFT_LABEL_H_
#define _MINECRAFT_LABEL_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MinecraftLabel : public CCLabelBMFont
{
public:

    static MinecraftLabel* create(std::wstring text, std::string font);
    static MinecraftLabel* create(std::string text, std::string font);
    bool init(std::wstring text, std::string font);
    void setColor(ccColor3B color);
private:


};

#endif