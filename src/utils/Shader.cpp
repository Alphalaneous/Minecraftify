#include "Shader.h"

void Shader::setup(std::string vertexSource, std::string fragmentSource){

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
        vertexSource.c_str()
    };

    glShaderSource(vertex, sizeof(vertexSources) / sizeof(char*), vertexSources, nullptr);
    glCompileShader(vertex);

    auto vertexLog = string::trim(getShaderLog(vertex));

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);

    log::info("vertex log: {}", vertexLog);

    if(!res) {
        glDeleteShader(vertex);
        vertex = 0;
        return;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSources[] = {
        fragmentSource.c_str()
    };

    glShaderSource(fragment, sizeof(fragmentSources) / sizeof(char*), fragmentSources, nullptr);
    glCompileShader(fragment);

    auto fragmentLog = string::trim(getShaderLog(fragment));

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);

    log::info("fragment log: {}", fragmentLog);

    if(!res) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        vertex = 0;
        fragment = 0;
        return;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    vertex = 0;
    fragment = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &res);

    if(!res) {
        glDeleteProgram(program);
        program = 0;
        return;
    }
}

void Shader::cleanup() {
    if(program) {
        glDeleteProgram(program);
    }
    program = 0;
}