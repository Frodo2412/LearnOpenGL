#include "VertexBuffer.h"

#include <cassert>
#include <utility>

#include "glad/glad.h"

VertexBuffer::VertexBuffer(void const *data, std::size_t const size, BufferUsage const usage) {
    glGenBuffers(1, &id_);
    assert(id_ != 0 && "glGenBuffers failed");
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
    size_ = size;
}

VertexBuffer::~VertexBuffer() {
    if (id_ != 0)
        glDeleteBuffers(1, &id_);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : id_(std::exchange(other.id_, 0)), size_(std::exchange(other.size_, 0)) {
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
    if (this != &other) {
        if (id_ != 0)
            glDeleteBuffers(1, &id_);
        id_ = std::exchange(other.id_, 0);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}

void VertexBuffer::bind(VertexBuffer const &buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id_);
}

std::size_t VertexBuffer::get_size() const {
    return size_;
}
