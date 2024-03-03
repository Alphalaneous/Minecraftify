#include "Panorama.h"
#include "../../utils/GlobalVars.h"

Panorama* Panorama::create(){
    auto node = new Panorama;
    if (!node->init()) {
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    node->autorelease();
    return node;
}

bool Panorama::init(){
    ghc::filesystem::path vertexPath = (std::string)CCFileUtils::sharedFileUtils()->fullPathForFilename("vertex.glsl"_spr, false);
    ghc::filesystem::path fragmentPath = (std::string)CCFileUtils::sharedFileUtils()->fullPathForFilename("fragment.glsl"_spr, false);
    
    auto vertexSource = file::readString(vertexPath);
    auto fragmentSource = file::readString(fragmentPath);

    auto res = m_shader.compile(vertexSource.unwrap(), fragmentSource.unwrap());
    if (!res) {
        log::error("{}", res.unwrapErr());
        return false;
    }

    glBindAttribLocation(m_shader.program, 0, "aPosition");
    res = m_shader.link();

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

    std::vector<std::string> faces
    {
        "panorama_1.png"_spr,
        "panorama_3.png"_spr,
        "panorama_4.png"_spr,
        "panorama_5.png"_spr,
        "panorama_0.png"_spr,
        "panorama_2.png"_spr
    };

    if (Mod::get()->getSettingValue<bool>("use-gd-pano"))
    {
        // much easier to do this
        faces = {
            "gd-background.png"_spr,
            "gd-background.png"_spr,
            "gd-background.png"_spr,
            "gd-background.png"_spr,
            "gd-background.png"_spr,
            "gd-background.png"_spr
        };
    }


    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {

        CCImage img;
        img.initWithImageFile(faces[i].c_str());

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,  0, 
            GL_RGB, 
            img.getWidth(),  
            img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getData());

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

#ifndef GEODE_IS_MACOS
    CC_INCREMENT_GL_DRAWS(1);
#endif
}