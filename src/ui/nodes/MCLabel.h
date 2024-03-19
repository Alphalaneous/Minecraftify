#ifndef _MINECRAFT_LABEL_H_
#define _MINECRAFT_LABEL_H_

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MCLabel : public CCLabelBMFont
{
public:

    static MCLabel* create(std::wstring text, std::string font);
    static MCLabel* create(std::string text, std::string font);
    bool init(std::wstring text, std::string font);

    void setColor(ccColor3B color);

};

#endif