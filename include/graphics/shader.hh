#pragma once
#include "core/util.hh"
#include <string>

namespace box {


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

    ~Shader() {
        glDeleteProgram(shader_program_);
    }

    void Bind() const {
        glUseProgram(shader_program_);
    }

    void Unbind() const {
        glUseProgram(0);
    }

    void SetUniform1i(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), value);
    }

    void SetUniform1f(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(shader_program_, name.c_str()), value);
    }

    void SetUniform3f(const std::string& name, float v0, float v1, float v2) const {
        glUniform3f(glGetUniformLocation(shader_program_, name.c_str()), v0, v1, v2);
    }

    void SetUniform3f(const std::string& name, const glm::vec3& vec) const {
        glUniform3f(glGetUniformLocation(shader_program_, name.c_str()), vec.x, vec.y, vec.z);
    }

    void SetUniform4f(const std::string& name, const glm::vec4& vec) const {
        glUniform4f(glGetUniformLocation(shader_program_, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }

    void SetUniformMat4f(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(shader_program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    unsigned int shader_program_;

    std::string LoadShaderSource(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void CheckCompilationErrors(unsigned int shader, unsigned int type);
    void CheckLinkingErrors(unsigned int program);
};


} // namespace box
