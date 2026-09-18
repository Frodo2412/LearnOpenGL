#include "Texture.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "glad/glad.h"
#include "stb_image.h"

#include "files/FileSystem.h"

namespace {
    GLenum format_for_channels(int const channels) {
        switch (channels) {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
            default: throw std::runtime_error("Unsupported texture channel count: " + std::to_string(channels));
        }
    }

    // Without mipmaps, minification must not sample mip levels that don't exist.
    GLint min_filter_for(texture_options const &options) {
        bool const linear = options.filter == texture_filter::linear;
        if (!options.mipmaps)
            return linear ? GL_LINEAR : GL_NEAREST;
        return linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
    }
}

Texture::Texture(std::string const &path, texture_options const &options) {
    stbi_set_flip_vertically_on_load(options.flip_vertically);

    int channels = 0;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> const data(
        stbi_load(FileSystem::getPath(path).c_str(), &width_, &height_, &channels, 0), &stbi_image_free);
    if (!data)
        throw std::runtime_error("Failed to load texture: " + path);

    GLenum const format = format_for_channels(channels);

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    // Rows of 1-, 2- and 3-channel images aren't necessarily 4-byte aligned.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width_, height_, 0, format, GL_UNSIGNED_BYTE,
                 data.get());

    auto const wrap = static_cast<GLint>(options.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_for(options));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(options.filter));

    if (options.mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    if (id_ != 0)
        glDeleteTextures(1, &id_);
}

Texture::Texture(Texture &&other) noexcept
    : id_(std::exchange(other.id_, 0)), width_(std::exchange(other.width_, 0)),
      height_(std::exchange(other.height_, 0)), unit_(std::exchange(other.unit_, -1)) {
}

Texture &Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        if (id_ != 0)
            glDeleteTextures(1, &id_);
        id_ = std::exchange(other.id_, 0);
        width_ = std::exchange(other.width_, 0);
        height_ = std::exchange(other.height_, 0);
        unit_ = std::exchange(other.unit_, -1);
    }
    return *this;
}

void Texture::bind(Texture &tex, unsigned int const unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex.id_);
    tex.unit_ = static_cast<int>(unit);
}

int Texture::get_unit() const {
    return unit_;
}

int Texture::get_width() const {
    return width_;
}

int Texture::get_height() const {
    return height_;
}
