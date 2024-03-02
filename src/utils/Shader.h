#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Shader {
public:

    GLuint vertex = 0;
    GLuint fragment = 0;
    GLuint program = 0;

    Result<std::string> compile(std::string vertexSource, std::string fragmentSource);
    Result<std::string> link();
    void cleanup();
};