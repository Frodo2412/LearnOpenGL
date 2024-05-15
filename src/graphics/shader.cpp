#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

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
    int success;

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
    glCompileShader(shader);
    // print compile errors if any
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        const char* type_str = type == vertex ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR::SHADER::" << type_str << "::COMPILATION_FAILED\n" << info_log << '\n';
    }

    return shader;
}

unsigned int shader::link_program(const unsigned int vertex_shader, const unsigned int fragment_shader)
{
    // shader Program
    const auto id = glCreateProgram();
    int success;

    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << '\n';
    }

    return id;
}

shader::shader(const char* vertex_path, const char* fragment_path)
{
    const auto vertex_code = read_file(vertex_path);

    std::cout << "Vertex code: " << vertex_code.c_str() << '\n';

    const auto fragment_code = read_file(fragment_path);

    std::cout << "Fragment code: " << fragment_code.c_str() << '\n';

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
