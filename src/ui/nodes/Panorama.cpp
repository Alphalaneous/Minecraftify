#include "Panorama.h"
#include "../../utils/GlobalVars.h"
#include "../../utils/Utils.h"

Panorama* Panorama::create(){
    auto ret = new Panorama();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool Panorama::init(){
    std::string vertexPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("cubemap_vert.glsl"_spr, false);
    std::string fragmentPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("cubemap_frag.glsl"_spr, false);
    
    auto vertexSource = file::readString(std::filesystem::path(vertexPath));
    auto fragmentSource = file::readString(std::filesystem::path(fragmentPath));

    m_shader.setup(vertexSource.unwrap(), fragmentSource.unwrap());
    glBindAttribLocation(m_shader.program, 0, "aPosition");

    ccGLUseProgram(m_shader.program);

    GLfloat vertices[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,

        -1.0f,  1.0f,
        1.0f, -1.0f,
        1.0f,  1.0f
    };
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)nullptr);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_uniformResolution = glGetUniformLocation(m_shader.program, "resolution");
    m_uniformTime = glGetUniformLocation(m_shader.program, "time");
    m_uniformMouse = glGetUniformLocation(m_shader.program, "mouse");

    std::string panorama0 = Mod::get()->getSavedValue<std::string>("panorama-0", "panorama_0.png"_spr);
    std::string panorama1 = Mod::get()->getSavedValue<std::string>("panorama-1", "panorama_1.png"_spr);
    std::string panorama2 = Mod::get()->getSavedValue<std::string>("panorama-2", "panorama_2.png"_spr);
    std::string panorama3 = Mod::get()->getSavedValue<std::string>("panorama-3", "panorama_3.png"_spr);
    std::string panorama4 = Mod::get()->getSavedValue<std::string>("panorama-4", "panorama_4.png"_spr);
    std::string panorama5 = Mod::get()->getSavedValue<std::string>("panorama-5", "panorama_5.png"_spr);

    bool changed = Mod::get()->getSavedValue<bool>("changed", false);
    if (changed) {
        Utils::clearImageCache();
        Mod::get()->setSavedValue<bool>("changed", false);
    }

    std::vector<std::string> faces
    {
        panorama1,
        panorama3,
        panorama4,
        panorama5,
        panorama0,
        panorama2
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        Utils::generateTexture(faces[i].c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    auto glv = CCDirector::sharedDirector()->getOpenGLView();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto frSize = glv->getFrameSize();

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    glUniform2f(m_uniformResolution, frSize.width * scale, frSize.height * scale);
    auto mousePos = cocos::getMousePos() / winSize * frSize;
    glUniform2f(m_uniformMouse, mousePos.x, mousePos.y);

    this->scheduleUpdate();

    return true;
}

void Panorama::update(float dt) {
    GlobalVars::getSharedInstance()->shaderTime += dt;
}

void Panorama::draw() {
    glBindVertexArray(m_vao);

    ccGLUseProgram(m_shader.program);

    auto glv = CCDirector::sharedDirector()->getOpenGLView();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto frSize = glv->getFrameSize();


    glUniform2f(m_uniformResolution, frSize.width, frSize.height);
    auto mousePos = cocos::getMousePos() / winSize * frSize;
    glUniform2f(m_uniformMouse, mousePos.x, mousePos.y);

    glUniform1f(m_uniformTime, GlobalVars::getSharedInstance()->shaderTime);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}