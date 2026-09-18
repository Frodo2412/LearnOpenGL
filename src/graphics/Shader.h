#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"

#include <stdexcept>
#include <string>
#include <fstream>

enum class ShaderType {
    VERTEX, FRAGMENT
};

class Shader {
public:
    unsigned int id;

    Shader(const char *vertexPath, const char *fragmentPath);

    static void use(Shader shader); // Static since only a single shader can be active at a time

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    // Points the sampler `name` at the unit `texture` was bound to. Bind it first with Texture::bind.
    // Throws std::logic_error if the texture was never bound.
    void setTexture(const std::string &name, const Texture &texture) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    static void read_shader_files(const char *vertexPath, const char *fragmentPath, std::string &vertexCode,
                                  std::string &fragmentCode, std::ifstream &vShaderFile, std::ifstream &fShaderFile);
    static unsigned int compile_shader(const char *shader_code, ShaderType type);\
    static unsigned int link_program(unsigned int vertexShader, unsigned int fragmentShader);
    static void check_compiler_errors(GLuint shader, const std::string &type);
    static void delete_linked_shaders(unsigned int vertex, unsigned int fragment);
};
#endif
