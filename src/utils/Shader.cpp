#include "Shader.h"

Result<std::string> Shader::compile(std::string vertexSource,std::string fragmentSource){

    vertexSource = utils::string::trim(vertexSource);
       
    auto getShaderLog = [](GLuint id) -> std::string {
        GLint length, written;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        if (length <= 0){
            return "";
        }

        auto text = new char[length + 1];
        glGetShaderInfoLog(id, length, &written, text);

        std::string result(text);
        delete[] text;
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
        return Err("Vertex shader compilation failed:\n{}", vertexLog);
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
        return Err("Fragment shader compilation failed:\n{}", fragmentLog);
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return Ok(fmt::format(
        "Shader compilation successful. Logs:\nVert:\n{}\nFrag:\n{}",
        vertexLog, fragmentLog
    ));
}

Result<std::string> Shader::link(){

    if (!vertex) {
        return Err("Vertex shader not compiled!");
    }

    if (!fragment) {
        return Err("Fragment shader not compiled!");
    }

    auto getProgramLog = [](GLuint id) -> std::string {
        GLint length, written;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 0){
            return "";
        }

        auto text = new char[length + 1];
        glGetProgramInfoLog(id, length, &written, text);
        std::string result(text);
        delete[] text;
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
        return Err("Shader link failed:\n{}", programLog);
    }

    return Ok(fmt::format("Shader link successful. Log:\n{}", programLog));
}

void Shader::cleanup() {
    if(program) {
        glDeleteProgram(program);
    }
    program = 0;
}