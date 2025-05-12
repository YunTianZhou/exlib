#pragma once

#include <memory>
#include <filesystem>

#include "exlib/opengl/tex.hpp"

namespace ex {

class Image;

class EXLIB_API Texture {
public:
    using Filter = gl::Tex::Filter;
    using Wrap = gl::Tex::Wrap;

public:
    // Constructors and Destructors
    Texture() = default;
    explicit Texture(const std::filesystem::path& path);
    Texture(Vec2i size, const unsigned char* buffer);
    explicit Texture(const void* data, int size);
    explicit Texture(const Image& image);
    ~Texture() = default;

    // Move Constructors
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Create and Load
    Texture copy() const;
    bool load_from_file(const std::filesystem::path& path);
    bool load_from_memory(const void* data, int size);
    bool load_from_image(const Image& image);

    // Binding
    inline void bind(unsigned int slot = 0) const { tex.bind(slot); };
    inline void unbind() const { tex.unbind(); };

    // Data Upload
    void set_data(Vec2i size, const unsigned char* buffer);
    void update_sub(Vec2i offset, Vec2i sub_size, const unsigned char* data);

    // Parameters
    void set_filter(Filter min_filter, Filter mag_filter);
    void set_wrap(Wrap wrap_s, Wrap wrap_t);

    // Mipmaps
    void generate_mipmaps();

    // Utilities
    inline Vec2i get_size() const { return tex.get_size(); }
    bool is_exist() const { return tex.is_exist(); }

    // Static functions
    static int get_maximum_size();

private:
    friend class Font;
    void double_size();

private:
    gl::Tex tex;
};

}