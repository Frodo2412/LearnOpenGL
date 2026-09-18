#pragma once

#include <string>

enum class texture_wrap {
    repeat = 0x2901,
    mirrored_repeat = 0x8370,
    clamp_to_edge = 0x812F
};

enum class texture_filter {
    nearest = 0x2600,
    linear = 0x2601
};

struct texture_options {
    texture_wrap wrap = texture_wrap::repeat;
    texture_filter filter = texture_filter::linear;
    bool mipmaps = true;
    bool flip_vertically = true; // image rows start at the top, GL texture coordinates start at the bottom
};

// Owns a GL_TEXTURE_2D object. Move-only: the GL texture is deleted on destruction.
class Texture {
    unsigned int id_ = 0;
    int width_ = 0;
    int height_ = 0;
    int unit_ = -1; // texture unit it was last bound to, -1 if never bound

public:
    // Loads an image (path relative to the project root) into a new texture. The pixel format is picked from the
    // image's channel count. Throws std::runtime_error if the image cannot be loaded.
    explicit Texture(std::string const &path, texture_options const &options = {});

    ~Texture();

    Texture(Texture const &) = delete;

    Texture &operator=(Texture const &) = delete;

    Texture(Texture &&other) noexcept;

    Texture &operator=(Texture &&other) noexcept;

    // Binds `tex` to texture unit `unit` and remembers the unit (see get_unit). Both the active unit and the
    // bound texture are global GL state.
    static void bind(Texture &tex, unsigned int unit = 0);

    // The unit this texture was last bound to, or -1 if it was never bound.
    [[nodiscard]] int get_unit() const;

    [[nodiscard]] int get_width() const;

    [[nodiscard]] int get_height() const;
};
