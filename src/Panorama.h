#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct Shader {
    GLuint vertex = 0;
    GLuint fragment = 0;
    GLuint program = 0;

    Result<std::string> compile(
        std::string vertexSource,
        std::string fragmentSource
    ) {
        vertexSource = utils::string::trim(vertexSource);
       
        auto getShaderLog = [](GLuint id) -> std::string {
            GLint length, written;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            if (length <= 0)
                return "";
            auto stuff = new char[length + 1];
            glGetShaderInfoLog(id, length, &written, stuff);
            std::string result(stuff);
            delete[] stuff;
            return result;
        };
        GLint res;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSources[] = {
#ifdef GEODE_IS_WINDOWS
            "#version 130\n",
#endif
#ifdef GEODE_IS_MOBILE
            "precision highp float;\n",
#endif
            vertexSource.c_str()
        };
        glShaderSource(vertex, sizeof(vertexSources) / sizeof(char*), vertexSources, nullptr);
        glCompileShader(vertex);
        auto vertexLog = getShaderLog(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);
        if(!res) {
            glDeleteShader(vertex);
            vertex = 0;
            return Err("vertex shader compilation failed:\n{}", vertexLog);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSources[] = {
#ifdef GEODE_IS_WINDOWS
            "#version 130\n",
#endif
#ifdef GEODE_IS_MOBILE
            "precision highp float;\n",
#endif
            fragmentSource.c_str()
        };
        glShaderSource(fragment, sizeof(vertexSources) / sizeof(char*), fragmentSources, nullptr);
        glCompileShader(fragment);
        auto fragmentLog = getShaderLog(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);
        if(!res) {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            vertex = 0;
            fragment = 0;
            return Err("fragment shader compilation failed:\n{}", fragmentLog);
        }

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        return Ok(fmt::format(
            "shader compilation successful. logs:\nvert:\n{}\nfrag:\n{}",
            vertexLog, fragmentLog
        ));
    }

    Result<std::string> link() {
        if (!vertex)
            return Err("vertex shader not compiled");
        if (!fragment)
            return Err("fragment shader not compiled");

        auto getProgramLog = [](GLuint id) -> std::string {
            GLint length, written;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
            if (length <= 0)
                return "";
            auto stuff = new char[length + 1];
            glGetProgramInfoLog(id, length, &written, stuff);
            std::string result(stuff);
            delete[] stuff;
            return result;
        };
        GLint res;

        glLinkProgram(program);
        auto programLog = getProgramLog(program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        vertex = 0;
        fragment = 0;

        glGetProgramiv(program, GL_LINK_STATUS, &res);
        if(!res) {
            glDeleteProgram(program);
            program = 0;
            return Err("shader link failed:\n{}", programLog);
        }

        return Ok(fmt::format("shader link successful. log:\n{}", programLog));
    }

    void cleanup() {
        if (program)
            glDeleteProgram(program);
        program = 0;
    }
};

class Panorama : public CCNode
{
public:

    static Panorama* create() {
        auto node = new Panorama;
        if (!node->init()) {
            CC_SAFE_DELETE(node);
            return nullptr;
        }
        node->autorelease();
        return node;
    }

    Shader m_shader;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLint m_uniformResolution = 0;
    GLint m_uniformTime = 0;
    GLint m_uniformMouse = 0;
    unsigned int textureID;

    virtual bool init() override
    {

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
    
    void update(float dt) override {
        GlobalVars::getSharedInstance()->shaderTime += dt;
    }

    static std::tuple<float, float, float, bool> getStuffRecursive(CCNode* node) {
        auto parent = node->getParent();
        if (!parent)
            return std::make_tuple(
                node->getRotation(),
                node->getScaleX(), node->getScaleY(),
                node->isVisible()
            );
        auto [parRot, parScaleX, parScaleY, parVis] = getStuffRecursive(parent);
        return std::make_tuple(
            parRot + node->getRotation(),
            parScaleX * node->getScaleX(), parScaleY * node->getScaleY(),
            parVis && node->isVisible()
        );
    }

    void draw() override {
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
};