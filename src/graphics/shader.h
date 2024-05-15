#pragma once


enum shader_type
{
    vertex,
    fragment
};

class shader
{
    unsigned int id_;

    static int success_;
    static char info_log_[512];

    static unsigned int compile_shader(shader_type type, const char* source);
    static unsigned int link_program(unsigned int vertex_shader, unsigned int fragment_shader);

public:
    explicit shader(const char* vertex_path, const char* fragment_path);
};
