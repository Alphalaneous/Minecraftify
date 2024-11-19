#pragma once

#include <Geode/Bindings.hpp>
#include "../MCOptionsInnerLayer.h"
#include "../MCOptionsOuterLayer.h"

using namespace geode::prelude;

class PracticeSettings : public MCOptionsInnerLayer {
protected:
    virtual bool init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
public:

    static PracticeSettings* create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
};