#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "VertexBuffer.h"

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
struct VertexAttribute {
    unsigned int location{}; // `layout (location = N)` in the vertex shader
    int components{}; // 1-4
    std::size_t offset{}; // in bytes from the start of the vertex
    attribute_type type = attribute_type::float32;
    bool normalized = false;
};

// Owns a vertex array object together with (shared) references to the vertex buffers it points
// at, so a buffer cannot be destroyed while any VAO still points at it. A single buffer may be
// shared across multiple VertexArrays (e.g. two VAOs drawing the same mesh with different shaders).
// Move-only: the GL objects are deleted on destruction.
class VertexArray {
    unsigned int id_ = 0;
    std::vector<std::shared_ptr<VertexBuffer>> buffers_;
    std::size_t vertex_count_ = 0; // taken from the first buffer added

public:
    VertexArray();

    ~VertexArray();

    VertexArray(VertexArray const &) = delete;

    VertexArray &operator=(VertexArray const &) = delete;

    VertexArray(VertexArray &&other) noexcept;

    VertexArray &operator=(VertexArray &&other) noexcept;

    // Shares ownership of `buffer` and points the attributes in `layout` at it.
    // `stride` is the size in bytes of one vertex. The first buffer added defines the vertex count
    // (buffer size / stride). Leaves this array and the buffer bound.
    void add_buffer(std::shared_ptr<VertexBuffer> buffer, std::vector<VertexAttribute> const &layout,
                     std::size_t stride);

    // Binds this array and draws its vertices with glDrawArrays.
    void draw(primitive mode = primitive::triangles) const;
};
