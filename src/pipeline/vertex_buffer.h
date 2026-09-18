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
    std::size_t size_ = 0; // bytes currently allocated on the GPU

public:
    // Creates an empty buffer with no storage allocated.
    vertex_buffer();

    // Creates a buffer and uploads `size` bytes from `data`.
    vertex_buffer(void const *data, std::size_t size, buffer_usage usage = buffer_usage::static_draw);

    ~vertex_buffer();

    vertex_buffer(vertex_buffer const &) = delete;

    vertex_buffer &operator=(vertex_buffer const &) = delete;

    vertex_buffer(vertex_buffer &&other) noexcept;

    vertex_buffer &operator=(vertex_buffer &&other) noexcept;

    // The bound buffer is global GL state (GL_ARRAY_BUFFER), not per-instance state.
    static void bind(vertex_buffer const &buffer);

    static void unbind();

    // Id of the buffer currently bound to GL_ARRAY_BUFFER, or 0 if none (queries GL).
    [[nodiscard]] static unsigned int get_bound_id();

    // (Re)allocates storage and uploads `size` bytes. `data` may be nullptr to only allocate.
    void set_data(void const *data, std::size_t size, buffer_usage usage = buffer_usage::static_draw);

    // Overwrites part of the existing storage. Requires offset + size <= get_size().
    void update_data(void const *data, std::size_t size, std::size_t offset = 0) const;

    [[nodiscard]] unsigned int get_id() const;

    [[nodiscard]] std::size_t get_size() const;
};
