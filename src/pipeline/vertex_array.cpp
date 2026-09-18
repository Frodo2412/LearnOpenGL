#include "vertex_array.h"

#include <cassert>
#include <utility>

#include "glad/glad.h"

namespace {
    // Restores the previously bound VAO when it goes out of scope.
    class binding_guard {
        unsigned int previous_;

    public:
        explicit binding_guard(unsigned int const id) : previous_(vertex_array::get_bound_id()) {
            glBindVertexArray(id);
        }

        ~binding_guard() { glBindVertexArray(previous_); }

        binding_guard(binding_guard const &) = delete;

        binding_guard &operator=(binding_guard const &) = delete;
    };

    bool is_integer(attribute_type const type) {
        return type == attribute_type::int32 || type == attribute_type::uint32;
    }
}

vertex_array::vertex_array() {
    glGenVertexArrays(1, &id_);
}

vertex_array::~vertex_array() {
    if (index_buffer_id_ != 0) glDeleteBuffers(1, &index_buffer_id_);
    if (id_ != 0) glDeleteVertexArrays(1, &id_);
}

vertex_array::vertex_array(vertex_array &&other) noexcept
    : id_(std::exchange(other.id_, 0)),
      index_buffer_id_(std::exchange(other.index_buffer_id_, 0)),
      buffers_(std::move(other.buffers_)),
      vertex_count_(std::exchange(other.vertex_count_, 0)),
      index_count_(std::exchange(other.index_count_, 0)) {
    other.buffers_.clear();
}

vertex_array &vertex_array::operator=(vertex_array &&other) noexcept {
    if (this != &other) {
        if (index_buffer_id_ != 0) glDeleteBuffers(1, &index_buffer_id_);
        if (id_ != 0) glDeleteVertexArrays(1, &id_);
        id_ = std::exchange(other.id_, 0);
        index_buffer_id_ = std::exchange(other.index_buffer_id_, 0);
        buffers_ = std::move(other.buffers_);
        other.buffers_.clear();
        vertex_count_ = std::exchange(other.vertex_count_, 0);
        index_count_ = std::exchange(other.index_count_, 0);
    }
    return *this;
}

void vertex_array::bind(vertex_array const &array) {
    glBindVertexArray(array.id_);
}

void vertex_array::unbind() {
    glBindVertexArray(0);
}

unsigned int vertex_array::get_bound_id() {
    GLint id = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id);
    return static_cast<unsigned int>(id);
}

void vertex_array::add_buffer(vertex_buffer buffer, std::vector<vertex_attribute> const &layout,
                              std::size_t const stride) {
    assert(id_ != 0 && "add_buffer on a moved-from vertex_array");
    assert(stride > 0 && "stride must be non-zero");

    binding_guard const guard(id_);
    // GL_ARRAY_BUFFER is not part of VAO state, only the pointers below capture it, so restore it afterwards.
    auto const previous_buffer = vertex_buffer::get_bound_id();
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

    glBindBuffer(GL_ARRAY_BUFFER, previous_buffer);

    if (buffers_.empty()) vertex_count_ = buffer.get_size() / stride;
    buffers_.push_back(std::move(buffer));
}

void vertex_array::set_indices(unsigned int const *indices, std::size_t const count) {
    assert(id_ != 0 && "set_indices on a moved-from vertex_array");

    // The element buffer binding is stored in the VAO, so the VAO must be bound while it is set.
    binding_guard const guard(id_);
    if (index_buffer_id_ == 0) glGenBuffers(1, &index_buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(unsigned int)), indices,
                 GL_STATIC_DRAW);
    index_count_ = count;
}

void vertex_array::draw(primitive const mode) const {
    assert(id_ != 0 && "draw on a moved-from vertex_array");

    binding_guard const guard(id_);
    if (index_buffer_id_ != 0) {
        glDrawElements(static_cast<GLenum>(mode), static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(static_cast<GLenum>(mode), 0, static_cast<GLsizei>(vertex_count_));
    }
}

unsigned int vertex_array::get_id() const {
    return id_;
}

std::size_t vertex_array::get_vertex_count() const {
    return vertex_count_;
}

std::size_t vertex_array::get_index_count() const {
    return index_count_;
}
