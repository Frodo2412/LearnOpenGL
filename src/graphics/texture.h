#pragma once

class texture {
    unsigned int id_;

public:
    explicit texture(char const *file_path);

    [[nodiscard]] unsigned int get_id() const;
};
