#pragma once

#include <cstddef>

enum class buffer_usage {
    static_draw = 0x88E4, // written once, drawn many times
    dynamic_draw = 0x88E8, // written often, drawn many times
    stream_draw = 0x88E0 // written once, drawn a few times
};

// Owns a GL_ARRAY_BUFFER object. Move-only: the GL buffer is deleted on destruction.
class vertex_buffer {
    unsigned int id_ = 0;
    std::size_t size_ = 0; // bytes uploaded to the GPU

public:
    // Creates a buffer and uploads `size` bytes from `data`. Leaves it bound to GL_ARRAY_BUFFER.
    vertex_buffer(void const *data, std::size_t size, buffer_usage usage = buffer_usage::static_draw);

    ~vertex_buffer();

    vertex_buffer(vertex_buffer const &) = delete;

    vertex_buffer &operator=(vertex_buffer const &) = delete;

    vertex_buffer(vertex_buffer &&other) noexcept;

    vertex_buffer &operator=(vertex_buffer &&other) noexcept;

    // The bound buffer is global GL state (GL_ARRAY_BUFFER), not per-instance state.
    static void bind(vertex_buffer const &buffer);

    [[nodiscard]] std::size_t get_size() const;
};
