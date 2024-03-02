#pragma once

#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

#include <Geode/Geode.hpp>

class GlobalVars {


protected:
    static GlobalVars* instance;
public:
    bool isInitialLaunch = true;
    float shaderTime = 0.0f;

    static GlobalVars* getSharedInstance(){

        if (!instance) {
            instance = new GlobalVars();
        };
        return instance;
    }

};

#endif