#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "psapi.h"

class $modify(MyCCKeyboardDispatcher, CCKeyboardDispatcher) {

    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool arr) {
		
        if(PlayLayer* playLayer = PlayLayer::get()){
            if(key == KEY_F3 && down && !arr){
                GameManager* gm = GameManager::get();
                if(CCNode* debugText = playLayer->getChildByID("debug-text")){
                    bool isVisible = debugText->isVisible();
                    debugText->setVisible(!isVisible);
                    gm->setGameVariable("0109", !isVisible);
                }
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
    }
};

class $modify(MyPlayLayer, PlayLayer){

    struct Fields {
        CCNode* leftDebugNode;
        CCNode* rightDebugNode;
        SEL_SCHEDULE fpsSchedule;
        std::string cpuName;
        std::string gpuVendor;
        std::string gpuVersion;
        std::string gpuRenderer;
        std::string cocosVersion;
        int fpsVal = 0;
        int currentFps = 0;
        int totalMemory = 0;
    };


    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects){
        if(!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        CCConfiguration* config = CCConfiguration::sharedConfiguration();
        CCDictionary* dict = public_cast(config, m_pValueDict);

        m_fields->gpuVendor = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.vendor"))->getCString());
        m_fields->gpuRenderer = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.renderer"))->getCString());
        m_fields->gpuVersion = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.version"))->getCString());
        m_fields->cocosVersion = std::string(typeinfo_cast<CCString*>(dict->objectForKey("cocos2d.x.version"))->getCString());
        m_fields->cpuName = Utils::getCPUInfo();

        Utils::trim(m_fields->gpuVendor);
        Utils::trim(m_fields->gpuRenderer);
        Utils::trim(m_fields->gpuVersion);
        Utils::trim(m_fields->cocosVersion);
        Utils::trim(m_fields->cpuName);

        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

        int conversion = 1048576;

        m_fields->totalMemory = totalPhysMem/conversion;

        CCSize winSize = CCDirector::get()->getWinSize();

        m_fields->leftDebugNode = CCNode::create();
        m_fields->leftDebugNode->setAnchorPoint({0, 1});
        m_fields->leftDebugNode->setPosition({1, winSize.height - 1});
        m_fields->leftDebugNode->setContentSize(winSize);

        GameManager* gm = GameManager::get();

        //show info label: 109

        ColumnLayout* layout = ColumnLayout::create();
        layout->setAxisReverse(true);
        layout->setAxisAlignment(AxisAlignment::End);
        layout->setCrossAxisAlignment(AxisAlignment::Start);
        layout->setCrossAxisLineAlignment(AxisAlignment::Start);
        layout->setGap(0);
        layout->setGrowCrossAxis(true);

        m_fields->leftDebugNode->setLayout(layout);

        m_fields->rightDebugNode = CCNode::create();
        m_fields->rightDebugNode->setAnchorPoint({1, 1});
        m_fields->rightDebugNode->setPosition({winSize.width - 1, winSize.height - 1});
        m_fields->rightDebugNode->setContentSize(winSize);

        ColumnLayout* layout1 = ColumnLayout::create();
        layout1->setAxisReverse(true);
        layout1->setAxisAlignment(AxisAlignment::End);
        layout1->setCrossAxisAlignment(AxisAlignment::Start);
        layout1->setCrossAxisLineAlignment(AxisAlignment::End);
        layout1->setGap(0);
        layout1->setGrowCrossAxis(true);

        m_fields->rightDebugNode->setLayout(layout1);

        addChild(m_fields->leftDebugNode);
        addChild(m_fields->rightDebugNode);

        schedule(schedule_selector(MyPlayLayer::updateDebugLabels));
        setFPS(0);

        schedule(schedule_selector(MyPlayLayer::setFPS), 1);

        if(CCLabelBMFont* debugText = typeinfo_cast<CCLabelBMFont*>(getChildByID("debug-text"))) {
            if(strcmp(debugText->getString(), "Ignore Damage") == 0){ //hacky fix for wrong node IDs
                if(CCNode* node = getChildByID("percentage-label")){
                    debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                }
                else if(CCNode* node = getChildByID("time-label")){
                    debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                }
            }
            debugText->setOpacity(0);
        }

        return true;
    }

    void updateDebugLabels(float dt){
        m_fields->leftDebugNode->removeAllChildren();
        m_fields->rightDebugNode->removeAllChildren();

        if(CCLabelBMFont* debugText = typeinfo_cast<CCLabelBMFont*>(getChildByID("debug-text"))) {
            if(strcmp(debugText->getString(), "Ignore Damage") == 0){ //hacky fix for wrong node IDs
                if(CCNode* node = getChildByID("percentage-label")){
                    debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                }
                else if(CCNode* node = getChildByID("time-label")){
                    debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                }
            }

            if(!debugText->isVisible()) return;

            debugText->setOpacity(0);
            m_fields->leftDebugNode->setZOrder(debugText->getZOrder());
            m_fields->leftDebugNode->setVisible(debugText->isVisible());

            m_fields->currentFps = std::ceil(1/dt);

            std::vector<std::string> lines = Utils::splitString(debugText->getString(), "\n");

            std::vector<std::pair<std::string, std::string>> values;

            for(std::string str : lines){

                std::pair<std::string, std::string> pair;

                std::vector<std::string> parts = Utils::splitString(str, ":");

                if(parts.size() > 1){

                    pair.first = parts.at(0);
                    Utils::trim(pair.first);

                    pair.second = parts.at(1);
                    Utils::trim(pair.second);

                    values.push_back(pair);
                }
            }

            CCLayerColor* versionText = createTextLayer("Minecraft 2.206 (2.206/Geode)");
            CCLayerColor* fpsText = createTextLayer(fmt::format("{} fps", m_fields->fpsVal));

            CCLayerColor* levelIDText = createTextLayer(fmt::format("ID: {}", getFromVector("LevelID", values)));

            std::string timeVal = getFromVector("Time", values);
            std::string attemptVal = getFromVector("Attempt", values);
            std::string clickVal =  getFromVector("Taps", values);

            CCLayerColor* moreInfoText = createTextLayer(fmt::format("A: {}, TW: {}, G: {}", getFromVector("Active", values), getFromVector("TimeWarp", values), getFromVector("Gravity", values)));
            CCLayerColor* levelInfoText = createTextLayer(fmt::format("Time: {:.2f} Attempt: {} Clicks: {}", std::stof(timeVal), attemptVal, clickVal));

            m_fields->leftDebugNode->addChild(versionText);
            m_fields->leftDebugNode->addChild(fpsText);
            m_fields->leftDebugNode->addChild(levelIDText);
            m_fields->leftDebugNode->addChild(moreInfoText);
            m_fields->leftDebugNode->addChild(levelInfoText);

            CCLayerColor* dummySpace0 = createTextLayer(" ");
            dummySpace0->setOpacity(0);
            m_fields->leftDebugNode->addChild(dummySpace0);

            CCLayerColor* coordsText = createTextLayer(fmt::format("XY: {} / {}", getFromVector("X", values), getFromVector("Y", values)));
            CCLayerColor* songsSFXText = createTextLayer(fmt::format("Sounds: {} Songs: {}", getFromVector("SFX", values), getFromVector("Songs", values)));
            CCLayerColor* gradients = createTextLayer(fmt::format("Gradients: {}", getFromVector("Gradients", values)));
            CCLayerColor* particles = createTextLayer(fmt::format("Particles: {}", getFromVector("Particles", values)));
            CCLayerColor* perfText = createTextLayer(fmt::format("Perf M: {}, R: {}, S: {}, F: {}", getFromVector("Move", values), getFromVector("Rotate", values), getFromVector("Scale", values), getFromVector("Follow", values)));
            CCLayerColor* areaText = createTextLayer(fmt::format("Area M: {}, R: {}, S: {}, ColOp: {}", getFromVector("Move", values, 1), getFromVector("Rotate", values, 1), getFromVector("Scale", values, 1), getFromVector("ColOp", values)));


            m_fields->leftDebugNode->addChild(coordsText);
            m_fields->leftDebugNode->addChild(songsSFXText);
            m_fields->leftDebugNode->addChild(gradients);
            m_fields->leftDebugNode->addChild(particles);
            m_fields->leftDebugNode->addChild(perfText);
            m_fields->leftDebugNode->addChild(areaText);

            m_fields->leftDebugNode->updateLayout();

            CCConfiguration* config = CCConfiguration::sharedConfiguration();
            CCDictionary* dict = public_cast(config, m_pValueDict);


            CCSize winSizePixels = CCDirector::get()->getWinSizeInPixels();

            int bit = 0;

            if(sizeof(void*) == 8) bit = 64;
            else if(sizeof(void*) == 4) bit = 32;
            
            CCLayerColor* cocosVersionText = createTextLayer(fmt::format("Cocos2d-x: {} {}bit", m_fields->cocosVersion, bit));
            m_fields->rightDebugNode->addChild(cocosVersionText);

            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);
            GlobalMemoryStatusEx(&memInfo);

            PROCESS_MEMORY_COUNTERS_EX pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
            SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

            DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
            SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

            int conversion = 1048576;

            int availableMemory = memInfo.ullAvailPhys/conversion;
            int usedMemory = pmc.WorkingSetSize/conversion;

            int percentUsed = (usedMemory/(float)availableMemory) * 100;

            CCLayerColor* memUsageText = createTextLayer(fmt::format("Mem: {}% {}/{}MB", percentUsed, usedMemory, availableMemory + usedMemory));
            m_fields->rightDebugNode->addChild(memUsageText);

            CCLayerColor* totalMemText = createTextLayer(fmt::format("Total Mem: {}MB", m_fields->totalMemory));
            m_fields->rightDebugNode->addChild(totalMemText);

            CCLayerColor* dummySpace1 = createTextLayer(" ");
            dummySpace1->setOpacity(0);
            m_fields->rightDebugNode->addChild(dummySpace1);

            CCLayerColor* cpuText = createTextLayer(fmt::format("CPU: {}", m_fields->cpuName));
            m_fields->rightDebugNode->addChild(cpuText);

            CCLayerColor* dummySpace2 = createTextLayer(" ");
            dummySpace2->setOpacity(0);
            m_fields->rightDebugNode->addChild(dummySpace2);

            CCLayerColor* resolutionText = createTextLayer(fmt::format("Display: {}x{} ({})", winSizePixels.width, winSizePixels.height, m_fields->gpuVendor));
            CCLayerColor* gpuInfoText = createTextLayer(m_fields->gpuRenderer);
            CCLayerColor* gpuVersionText = createTextLayer(m_fields->gpuVersion);

            m_fields->rightDebugNode->addChild(resolutionText);
            m_fields->rightDebugNode->addChild(gpuInfoText);
            m_fields->rightDebugNode->addChild(gpuVersionText);

            m_fields->rightDebugNode->updateLayout();
        }
    }

    void setFPS(float dt){
        m_fields->fpsVal = m_fields->currentFps;
    }

    CCLayerColor* createTextLayer(std::string text){
        MCLabel* versionText = MCLabel::create(text, "minecraft-flat.fnt"_spr);
        versionText->setScale(0.4);

        CCLayerColor* bg = CCLayerColor::create();
        bg->setContentSize({versionText->getScaledContentSize().width + 2, versionText->getScaledContentSize().height-1});
        bg->setColor({128, 128, 128});
        bg->setOpacity(96);

        versionText->setPosition({versionText->getPosition().x + 1, versionText->getPosition().y - 0.1f});
        versionText->setAnchorPoint({0, 0});
        bg->addChild(versionText);

        return bg;
    }

    std::string getFromVector(std::string value, std::vector<std::pair<std::string, std::string>> values) {
        return getFromVector(value, values, 0);
    }

    std::string getFromVector(std::string value, std::vector<std::pair<std::string, std::string>> values, int index){
       
        int idx = 0;

        for(std::pair<std::string, std::string> valueA : values){
            if(valueA.first == value){
                if(idx == index) return valueA.second;
                idx++;
            }
        }

        return "";
    }
};