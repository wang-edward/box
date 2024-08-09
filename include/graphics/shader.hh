#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path) {
        // load from file
        std::string vertex_code = LoadShaderSource(vertex_path);
        std::string fragment_code = LoadShaderSource(fragment_path);

        // compile
        unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_code);
        unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_code);

        // link
        shader_program_ = glCreateProgram();
        glAttachShader(shader_program_, vertex_shader);
        glAttachShader(shader_program_, fragment_shader);
        glLinkProgram(shader_program_);

        // check linking errors
        CheckLinkingErrors(shader_program_);

        // delete unused shaders
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    void Bind() const {
        glUseProgram(shader_program_);
    }

    void Unbind() const {
        glUseProgram(0);
    }

    ~Shader() {
        glDeleteProgram(shader_program_);
    }

private:
    unsigned int shader_program_;

    std::string LoadShaderSource(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void CheckCompilationErrors(unsigned int shader, unsigned int type);
    void CheckLinkingErrors(unsigned int program);
};
