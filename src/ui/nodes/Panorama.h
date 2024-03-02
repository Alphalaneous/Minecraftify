#include <Geode/Geode.hpp>
#include "../../utils/Shader.h"

using namespace geode::prelude;

class Panorama : public CCNode
{
public:
    Shader m_shader;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLint m_uniformResolution = 0;
    GLint m_uniformTime = 0;
    GLint m_uniformMouse = 0;
    unsigned int textureID;

    static Panorama* create();
    virtual bool init() override;
    void update(float dt) override;
    void draw() override;
};