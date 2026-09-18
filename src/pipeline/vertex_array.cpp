#include "vertex_array.h"

#include <cassert>
#include <utility>

#include "glad/glad.h"

namespace {
    bool is_integer(attribute_type const type) {
        return type == attribute_type::int32 || type == attribute_type::uint32;
    }
}

vertex_array::vertex_array() {
    glGenVertexArrays(1, &id_);
}

vertex_array::~vertex_array() {
    if (id_ != 0) glDeleteVertexArrays(1, &id_);
}

vertex_array::vertex_array(vertex_array &&other) noexcept
    : id_(std::exchange(other.id_, 0)),
      buffers_(std::move(other.buffers_)),
      vertex_count_(std::exchange(other.vertex_count_, 0)) {
    other.buffers_.clear();
}

vertex_array &vertex_array::operator=(vertex_array &&other) noexcept {
    if (this != &other) {
        if (id_ != 0) glDeleteVertexArrays(1, &id_);
        id_ = std::exchange(other.id_, 0);
        buffers_ = std::move(other.buffers_);
        other.buffers_.clear();
        vertex_count_ = std::exchange(other.vertex_count_, 0);
    }
    return *this;
}

void vertex_array::add_buffer(vertex_buffer buffer, std::vector<vertex_attribute> const &layout,
                              std::size_t const stride) {
    assert(id_ != 0 && "add_buffer on a moved-from vertex_array");
    assert(stride > 0 && "stride must be non-zero");

    glBindVertexArray(id_);
    vertex_buffer::bind(buffer);

    for (auto const &[location, components, offset, type, normalized]: layout) {
        auto const *const pointer = reinterpret_cast<void const *>(offset);
        if (is_integer(type) && !normalized) {
            glVertexAttribIPointer(location, components, static_cast<GLenum>(type),
                                   static_cast<GLsizei>(stride), pointer);
        } else {
            glVertexAttribPointer(location, components, static_cast<GLenum>(type), normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(stride), pointer);
        }
        glEnableVertexAttribArray(location);
    }

    if (buffers_.empty()) vertex_count_ = buffer.get_size() / stride;
    buffers_.push_back(std::move(buffer));
}

void vertex_array::draw(primitive const mode) const {
    assert(id_ != 0 && "draw on a moved-from vertex_array");
    glBindVertexArray(id_);
    glDrawArrays(static_cast<GLenum>(mode), 0, static_cast<GLsizei>(vertex_count_));
}
