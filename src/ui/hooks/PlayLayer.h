#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

#ifdef GEODE_IS_WINDOWS
#include "psapi.h"
#endif

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
        CCLabelBMFont* debugText;
    };


    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects){
        if(!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

		if(!Loader::get()->isModLoaded("geode.node-ids")) return true;

        CCConfiguration* config = CCConfiguration::sharedConfiguration();
        CCDictionary* dict = public_cast(config, m_pValueDict);

        m_fields->gpuVendor = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.vendor"))->getCString());
        m_fields->gpuRenderer = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.renderer"))->getCString());
        m_fields->gpuVersion = std::string(typeinfo_cast<CCString*>(dict->objectForKey("gl.version"))->getCString());
        m_fields->cocosVersion = std::string(typeinfo_cast<CCString*>(dict->objectForKey("cocos2d.x.version"))->getCString());

        #ifndef GEODE_IS_ANDROID

        m_fields->cpuName = Utils::getCPUInfo();

        #endif

        Utils::trim(m_fields->gpuVendor);
        Utils::trim(m_fields->gpuRenderer);
        Utils::trim(m_fields->gpuVersion);
        Utils::trim(m_fields->cocosVersion);
        Utils::trim(m_fields->cpuName);

        #ifndef GEODE_IS_ANDROID

        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

        int conversion = 1048576;

        m_fields->totalMemory = totalPhysMem/conversion;

        #endif

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

        m_fields->leftDebugNode->setZOrder(99);
        m_fields->rightDebugNode->setZOrder(99);

        addChild(m_fields->leftDebugNode);
        addChild(m_fields->rightDebugNode);

        schedule(schedule_selector(MyPlayLayer::updateDebugLabels), 1/60);
        setFPS(0);
        schedule(schedule_selector(MyPlayLayer::setFPS), 1);

        return true;
    }

    void updateDebugLabels(float dt){

        if(!m_fields->debugText){
            if(CCLabelBMFont* debugText = typeinfo_cast<CCLabelBMFont*>(getChildByID("debug-text"))) {
                m_fields->debugText = debugText;

                if(strcmp(debugText->getString(), "Ignore Damage") == 0 || strcmp(debugText->getString(), "Testmode") == 0) { //hacky fix for wrong node IDs
                    if(CCNode* node = getChildByID("percentage-label")){
                        m_fields->debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                    }
                    else if(CCNode* node = getChildByID("time-label")){
                        m_fields->debugText = typeinfo_cast<CCLabelBMFont*>(node); 
                    }
                }
                m_fields->debugText->setOpacity(0);
            }
        }

        if(m_fields->debugText) {
            
            m_fields->leftDebugNode->setVisible(m_fields->debugText->isVisible());
            m_fields->rightDebugNode->setVisible(m_fields->debugText->isVisible());

            if(!m_fields->debugText->isVisible()) return;

            m_fields->currentFps = std::ceil(1/dt);

            std::vector<std::string> lines = Utils::splitString(m_fields->debugText->getString(), "\n");

            createTextLayer("version-label"_spr, m_fields->leftDebugNode, "Minecraft 2.2074 (2.2074/Geode)");
            createTextLayer("fps-label"_spr, m_fields->leftDebugNode, fmt::format("{} fps", m_fields->fpsVal));
            createTextLayer("level-id-label"_spr, m_fields->leftDebugNode, fmt::format("ID: {}", getFromPos(0, lines)));

            std::string timeVal = getFromPos(1, lines);
            std::string attemptVal = getFromPos(2, lines);
            std::string clickVal = getFromPos(3, lines);

            createTextLayer("more-info-label"_spr, m_fields->leftDebugNode, fmt::format("A: {}, TW: {}, G: {}", getFromPos(8, lines), getFromPos(4, lines), getFromPos(5, lines)));
            createTextLayer("level-info-label"_spr, m_fields->leftDebugNode, fmt::format("Time: {:.2f} Attempt: {} Clicks: {}", std::stof(timeVal), attemptVal, clickVal));


            CCLayerColor* dummySpace0 = createTextLayer("blank-label-0"_spr, m_fields->leftDebugNode, " ");
            dummySpace0->setOpacity(0);

            createTextLayer("coords-label"_spr, m_fields->leftDebugNode, fmt::format("XY: {} / {}", getFromPos(6, lines), getFromPos(7, lines)));
            createTextLayer("audio-label"_spr, m_fields->leftDebugNode, fmt::format("Sounds: {} Songs: {}", getFromPos(13, lines), getFromPos(12, lines)));
            createTextLayer("gradients-label"_spr, m_fields->leftDebugNode, fmt::format("Gradients: {}", getFromPos(9, lines)));
            createTextLayer("particles-label"_spr, m_fields->leftDebugNode, fmt::format("Particles: {}", getFromPos(10, lines)));
            createTextLayer("perf-label"_spr, m_fields->leftDebugNode, fmt::format("Perf M: {}, R: {}, S: {}, F: {}", getFromPos(15, lines), getFromPos(16, lines), getFromPos(17, lines), getFromPos(18, lines)));
            createTextLayer("area-label"_spr, m_fields->leftDebugNode, fmt::format("Area M: {}, R: {}, S: {}, ColOp: {}", getFromPos(20, lines), getFromPos(21, lines), getFromPos(22, lines), getFromPos(23, lines)));

            m_fields->leftDebugNode->updateLayout();

            CCSize winSizePixels = CCDirector::get()->getWinSizeInPixels();

            int bit = 0;

            if(sizeof(void*) == 8) bit = 64;
            else if(sizeof(void*) == 4) bit = 32;
            
            CCLayerColor* cocosVersionText = createTextLayer("cocos-label"_spr, m_fields->rightDebugNode, fmt::format("Cocos2d-x: {} {}bit", m_fields->cocosVersion, bit));

            #ifndef GEODE_IS_ANDROID

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

            createTextLayer("mem-usage-label"_spr, m_fields->rightDebugNode, fmt::format("Mem: {}% {}/{}MB", percentUsed, usedMemory, availableMemory + usedMemory));
            createTextLayer("total-mem-label"_spr, m_fields->rightDebugNode, fmt::format("Total Mem: {}MB", m_fields->totalMemory));
            CCLayerColor* dummySpace1 = createTextLayer("blank-label-1"_spr, m_fields->rightDebugNode, " ");
            dummySpace1->setOpacity(0);

            createTextLayer("cpu-label"_spr, m_fields->rightDebugNode, fmt::format("CPU: {}", m_fields->cpuName));

            CCLayerColor* dummySpace2 = createTextLayer("blank-label-2"_spr, m_fields->rightDebugNode, " ");
            dummySpace2->setOpacity(0);

            #endif

            createTextLayer("resolution-label"_spr, m_fields->rightDebugNode, fmt::format("Display: {}x{} ({})", winSizePixels.width, winSizePixels.height, m_fields->gpuVendor));
            createTextLayer("gpu-info-label"_spr, m_fields->rightDebugNode, m_fields->gpuRenderer);
            createTextLayer("gpu-version-label"_spr, m_fields->rightDebugNode, m_fields->gpuVersion);

            m_fields->rightDebugNode->updateLayout();
        }
    }

    void setFPS(float dt){
        m_fields->fpsVal = m_fields->currentFps;
    }

    CCLayerColor* createTextLayer(std::string id, CCNode* node, std::string text){

        if(CCLayerColor* bg0 = typeinfo_cast<CCLayerColor*>(node->getChildByID(id))){
            if(MCLabel* label = bg0->getChildByType<MCLabel>(0)){
                label->setString(text.c_str());
                bg0->setContentSize({label->getScaledContentSize().width + 2, label->getScaledContentSize().height-1});
            }
            return bg0;
        }
        else {
            MCLabel* label = MCLabel::create(text, "minecraft-flat.fnt"_spr);
            label->setScale(0.4);

            CCLayerColor* bg = CCLayerColor::create();
            bg->setContentSize({label->getScaledContentSize().width + 2, label->getScaledContentSize().height-1});
            bg->setColor({128, 128, 128});
            bg->setOpacity(96);

            label->setPosition({label->getPosition().x + 1, label->getPosition().y - 0.1f});
            label->setAnchorPoint({0, 0});
            bg->addChild(label);

            bg->setID(id);
            node->addChild(bg);
            return bg;
        }
    }

    std::string getFromPos(int pos, std::vector<std::string> values){

        std::string ret = "";

        if(pos < values.size()){

            std::string value = values.at(pos);
            std::vector<std::string> splitValue = Utils::splitString(value, ":");

            if(splitValue.size() > 0){
                ret = splitValue.at(1);
            }
            Utils::trim(ret);
        }
        return ret;
    }
};

class $modify(MyCCKeyboardDispatcher, CCKeyboardDispatcher) {

    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool arr) {
		
		if(Loader::get()->isModLoaded("geode.node-ids")) {

            if(MyPlayLayer* playLayer = static_cast<MyPlayLayer*>(PlayLayer::get())){
                if(key == KEY_F3 && down && !arr){
                    GameManager* gm = GameManager::get();
                    if(playLayer->m_fields->debugText){
                        bool isVisible = playLayer->m_fields->debugText->isVisible();
                        playLayer->m_fields->debugText->setVisible(!isVisible);
                        gm->setGameVariable("0109", !isVisible);
                    }
                }
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, arr);
    }
};