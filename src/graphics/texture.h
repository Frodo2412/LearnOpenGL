#pragma once

class texture
{
    unsigned int id_;

public:
    explicit texture(char const* file_path);

    unsigned int get_id() const;
};
