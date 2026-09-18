#include "vertex_buffer.h"

#include <cassert>
#include <utility>

#include "glad/glad.h"

namespace {
    // Restores the previously bound GL_ARRAY_BUFFER when it goes out of scope.
    class binding_guard {
        unsigned int previous_;

    public:
        explicit binding_guard(unsigned int const id) : previous_(vertex_buffer::get_bound_id()) {
            glBindBuffer(GL_ARRAY_BUFFER, id);
        }

        ~binding_guard() { glBindBuffer(GL_ARRAY_BUFFER, previous_); }

        binding_guard(binding_guard const &) = delete;

        binding_guard &operator=(binding_guard const &) = delete;
    };
}

vertex_buffer::vertex_buffer() {
    glGenBuffers(1, &id_);
}

vertex_buffer::vertex_buffer(void const *data, std::size_t const size, buffer_usage const usage) : vertex_buffer() {
    set_data(data, size, usage);
}

vertex_buffer::~vertex_buffer() {
    if (id_ != 0) glDeleteBuffers(1, &id_);
}

vertex_buffer::vertex_buffer(vertex_buffer &&other) noexcept
    : id_(std::exchange(other.id_, 0)), size_(std::exchange(other.size_, 0)) {
}

vertex_buffer &vertex_buffer::operator=(vertex_buffer &&other) noexcept {
    if (this != &other) {
        if (id_ != 0) glDeleteBuffers(1, &id_);
        id_ = std::exchange(other.id_, 0);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}

void vertex_buffer::bind(vertex_buffer const &buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id_);
}

void vertex_buffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int vertex_buffer::get_bound_id() {
    GLint id = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
    return static_cast<unsigned int>(id);
}

void vertex_buffer::set_data(void const *data, std::size_t const size, buffer_usage const usage) {
    assert(id_ != 0 && "set_data on a moved-from vertex_buffer");
    binding_guard const guard(id_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
    size_ = size;
}

void vertex_buffer::update_data(void const *data, std::size_t const size, std::size_t const offset) const {
    assert(id_ != 0 && "update_data on a moved-from vertex_buffer");
    assert(offset + size <= size_ && "update_data out of range");
    binding_guard const guard(id_);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
}

unsigned int vertex_buffer::get_id() const {
    return id_;
}

std::size_t vertex_buffer::get_size() const {
    return size_;
}
