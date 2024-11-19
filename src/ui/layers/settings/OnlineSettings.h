#pragma once

#include <Geode/Bindings.hpp>
#include "../MCOptionsInnerLayer.h"
#include "../MCOptionsOuterLayer.h"

using namespace geode::prelude;

class OnlineSettings : public MCOptionsInnerLayer {
protected:
    virtual bool init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
public:

    static OnlineSettings* create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
};