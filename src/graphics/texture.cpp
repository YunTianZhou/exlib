#include <stb_image.h>

#include "exlib/graphics/texture.hpp"
#include "exlib/graphics/image.hpp"
#include "exlib/core/exception.hpp"

namespace ex {

Texture::Texture(const std::filesystem::path& path) {
    if (!load_from_file(path))
        throw Exception("Failed to load texture from file");
}

Texture::Texture(Vec2i size, const unsigned char* buffer)
    : tex(size, buffer) {
}

Texture::Texture(const void* data, int size) {
    if (!load_from_memory(data, size))
        throw Exception("Failed to load texture from memory");
}

Texture::Texture(const Image& image) {
    if (!load_from_image(image))
        throw Exception("Failed to load texture from Image object");
}

Texture::Texture(Texture&& other) noexcept
    : tex(std::move(other.tex)) {
}

Texture& Texture::operator=(Texture&& other) noexcept {
    tex = std::move(other.tex);
    return *this;
}

Texture Texture::copy() const {
    Texture out;
    out.tex = tex.copy();
    return out;
}

bool Texture::load_from_file(const std::filesystem::path& path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &channels, 4);
    if (!data) return false;

    tex = gl::Texture({ w, h }, data);
    stbi_image_free(data);
    return true;
}

bool Texture::load_from_memory(const void* data, int size) {
    int w, h, channels;
    unsigned char* img = stbi_load_from_memory(
        (const unsigned char*) (data),
        size,
        &w, &h, &channels,
        4
    );

    if (!img) 
        return false;

    tex = gl::Texture({ w, h }, img);
    stbi_image_free(img);
    return true;
}

bool Texture::load_from_image(const Image& image) {
    tex = gl::Texture(image.get_size(), image.get_pixels());
    return true;
}

void Texture::set_data(const unsigned char* buffer) {
    Vec2i size = tex.get_size();
    if (size.x == 0 || size.y == 0)
        throw Exception("Cannot set_data on zero-sized texture");
    tex.set_data(size, buffer);
}

void Texture::update_sub(const Vec2i& offset, const Vec2i& sub_size, const unsigned char* data) {
    tex.update_sub(offset, sub_size, data);
}

void Texture::set_filter(Filter min_filter, Filter mag_filter) {
    tex.set_filter(min_filter, mag_filter);
}

void Texture::set_wrap(Wrap wrap_s, Wrap wrap_t) {
    tex.set_wrap(wrap_s, wrap_t);
}

void Texture::generate_mipmaps() {
    tex.generate_mipmaps();
}

}
