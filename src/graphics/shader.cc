#include "graphics/shader.hh"
#include <string>
std::string Shader:: LoadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    } else {
        throw std::runtime_error{"Failed to open shader file: " + filepath};
    }

    return buffer.str();
}


unsigned int Shader:: CompileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    CheckCompilationErrors(shader, type);

    return shader;
}

void Shader:: CheckCompilationErrors(unsigned int shader, unsigned int type) {
    int success;
    char info_log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        std::string fail_type = (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        throw std::runtime_error{"ERROR::SHADER::COMPILATION_FAILED of type: " + fail_type + "\n" + std::string(info_log)};
    }
}

void Shader:: CheckLinkingErrors(unsigned int program) {
    int success;
    char info_log[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, info_log);
        throw std::runtime_error{"ERROR::PROGRAM::LINKING_FAILED\n" + std::string(info_log)};
    }
}
