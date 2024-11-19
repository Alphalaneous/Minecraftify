#pragma once

#include <Geode/Bindings.hpp>
#include "../MCOptionsInnerLayer.h"
#include "../MCOptionsOuterLayer.h"

using namespace geode::prelude;

class VideoSettings : public MCOptionsInnerLayer {
protected:
    virtual bool init(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);
public:

    static VideoSettings* create(MCOptionsOuterLayer* topLayer, CCLayer* prevLayer);

};