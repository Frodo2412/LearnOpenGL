#pragma once

#include <cstddef>
#include <vector>

#include "vertex_buffer.h"

enum class attribute_type {
    float32 = 0x1406, // GL_FLOAT
    int32 = 0x1404, // GL_INT
    uint32 = 0x1405, // GL_UNSIGNED_INT
    uint8 = 0x1401 // GL_UNSIGNED_BYTE
};

enum class primitive {
    points = 0x0000, // GL_POINTS
    lines = 0x0001, // GL_LINES
    triangles = 0x0004 // GL_TRIANGLES
};

// Describes one attribute inside an interleaved vertex.
struct vertex_attribute {
    unsigned int location{}; // `layout (location = N)` in the vertex shader
    int components{}; // 1-4
    std::size_t offset{}; // in bytes from the start of the vertex
    attribute_type type = attribute_type::float32;
    bool normalized = false;
};

// Owns a vertex array object together with the vertex buffers it references, so the buffers
// cannot be destroyed while the VAO still points at them.
// Move-only: the GL objects are deleted on destruction.
class vertex_array {
    unsigned int id_ = 0;
    std::vector<vertex_buffer> buffers_;
    std::size_t vertex_count_ = 0; // taken from the first buffer added

public:
    vertex_array();

    ~vertex_array();

    vertex_array(vertex_array const &) = delete;

    vertex_array &operator=(vertex_array const &) = delete;

    vertex_array(vertex_array &&other) noexcept;

    vertex_array &operator=(vertex_array &&other) noexcept;

    // Takes ownership of `buffer` and points the attributes in `layout` at it.
    // `stride` is the size in bytes of one vertex. The first buffer added defines the vertex count
    // (buffer size / stride). Leaves this array and the buffer bound.
    void add_buffer(vertex_buffer buffer, std::vector<vertex_attribute> const &layout, std::size_t stride);

    // Binds this array and draws its vertices with glDrawArrays.
    void draw(primitive mode = primitive::triangles) const;
};
