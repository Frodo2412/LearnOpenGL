#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

int shader::success_ = 0;
char shader::info_log_[512];

auto read_file(const char* shader_path)
{
    std::string shader_code;
    std::ifstream shader_file;
    // ensure ifstream objects can throw exceptions:
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shader_file.open(shader_path);
        std::stringstream shader_stream;
        // read file's buffer contents into streams
        shader_stream << shader_file.rdbuf();
        // close file handlers
        shader_file.close();
        // convert stream into string
        shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << '\n';
    }
    return shader_code;
}

unsigned int shader::compile_shader(const shader_type type, const char* source)
{
    unsigned int shader = 0;

    switch (type)
    {
    case vertex:
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
    case fragment:
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success_);
    if (!success_)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log_);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log_ << '\n';
    }

    return shader;
}

unsigned int shader::link_program(const unsigned int vertex_shader, const unsigned int fragment_shader)
{
    // shader Program
    const auto id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success_);
    if (!success_)
    {
        glGetProgramInfoLog(id, 512, nullptr, info_log_);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log_ << '\n';
    }

    return id;
}

shader::shader(const char* vertex_path, const char* fragment_path)
{
    const auto vertex_code = read_file(vertex_path);
    const auto fragment_code = read_file(fragment_path);

    const auto vertex_shader = compile_shader(vertex, vertex_code.c_str());
    const auto fragment_shader = compile_shader(fragment, fragment_code.c_str());

    id_ = link_program(vertex_shader, fragment_shader);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void shader::use() const
{
    glUseProgram(id_);
}

void shader::set_uniform(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void shader::set_uniform(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void shader::set_uniform(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}
