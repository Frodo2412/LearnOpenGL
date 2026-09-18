#include "Shader.h"

#include <iostream>
#include <sstream>

static GLenum gl_shader_type(const ShaderType type) {
    switch (type) {
        case ShaderType::VERTEX: return GL_VERTEX_SHADER;
        case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
    }
    return 0; // Should never happen as switch is complete
}

static const char *shader_type_name(const ShaderType type) {
    switch (type) {
        case ShaderType::VERTEX: return "VERTEX";
        case ShaderType::FRAGMENT: return "FRAGMENT";
    }
    return "UNKNOWN"; // Should never happen as switch is complete
}

void Shader::delete_linked_shaders(unsigned int vertex, unsigned int fragment) {
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    read_shader_files(vertexPath, fragmentPath, vertexCode, fragmentCode, vShaderFile, fShaderFile);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex = compile_shader(vShaderCode, ShaderType::VERTEX);
    unsigned int fragment = compile_shader(fShaderCode, ShaderType::FRAGMENT);

    id = link_program(vertex, fragment);

    delete_linked_shaders(vertex, fragment);
}

void Shader::read_shader_files(const char *vertexPath, const char *fragmentPath, std::string &vertexCode,
                               std::string &fragmentCode, std::ifstream &vShaderFile,
                               std::ifstream &fShaderFile) {
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
}


unsigned int Shader::compile_shader(const char *shader_code, ShaderType type) {
    const unsigned int shader_id = glCreateShader(gl_shader_type(type));
    glShaderSource(shader_id, 1, &shader_code, nullptr);
    glCompileShader(shader_id);
    check_compiler_errors(shader_id, shader_type_name(type));
    return shader_id;
}

unsigned int Shader::link_program(const unsigned int vertexShader, const unsigned int fragmentShader) {
    const unsigned int shader_id = glCreateProgram();
    glAttachShader(shader_id, vertexShader);
    glAttachShader(shader_id, fragmentShader);
    glLinkProgram(shader_id);
    check_compiler_errors(shader_id, "PROGRAM");
    return shader_id;
}

void Shader::check_compiler_errors(const GLuint shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                    "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                    "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
