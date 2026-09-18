#include "vertex_buffer.h"

#include <utility>

#include "glad/glad.h"

vertex_buffer::vertex_buffer(void const *data, std::size_t const size, buffer_usage const usage) : size_(size) {
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
}

vertex_buffer::~vertex_buffer() {
    if (id_ != 0)
        glDeleteBuffers(1, &id_);
}

vertex_buffer::vertex_buffer(vertex_buffer &&other) noexcept
    : id_(std::exchange(other.id_, 0)), size_(std::exchange(other.size_, 0)) {
}

vertex_buffer &vertex_buffer::operator=(vertex_buffer &&other) noexcept {
    if (this != &other) {
        if (id_ != 0)
            glDeleteBuffers(1, &id_);
        id_ = std::exchange(other.id_, 0);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}

void vertex_buffer::bind(vertex_buffer const &buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id_);
}

std::size_t vertex_buffer::get_size() const {
    return size_;
}
