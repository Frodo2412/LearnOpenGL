#pragma once
#include <string>

class shader final
{
    enum shader_type
    {
        vertex,
        fragment
    };

    unsigned int id_;

    static unsigned int compile_shader(shader_type type, const char* source);
    static unsigned int link_program(unsigned int vertex_shader, unsigned int fragment_shader);

public:
    explicit shader(const char* vertex_path, const char* fragment_path);

    void use() const;

    void set_uniform(const std::string& name, bool value) const;
    void set_uniform(const std::string& name, int value) const;
    void set_uniform(const std::string& name, float value) const;
};
