#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Shader {
public:

    GLuint vertex = 0;
    GLuint fragment = 0;
    GLuint program = 0;

    void setup(std::string vertexSource, std::string fragmentSource);
    void cleanup();
};