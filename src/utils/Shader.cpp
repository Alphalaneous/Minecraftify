#include "Shader.h"

void Shader::setup(std::string vertexSource, std::string fragmentSource){

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

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);
    if(!res) {
        glDeleteShader(vertex);
        vertex = 0;
        return;
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

    glShaderSource(fragment, sizeof(fragmentSources) / sizeof(char*), fragmentSources, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);
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