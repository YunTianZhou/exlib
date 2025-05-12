#include <stb_image.h>

#include "exlib/graphics/texture.hpp"
#include "exlib/graphics/image.hpp"
#include "exlib/core/exception.hpp"

namespace ex {

Texture::Texture(const std::filesystem::path& path) {
    if (!load_from_file(path))
        EX_THROW("Failed to load texture from file: " + path.string());
}

Texture::Texture(Vec2i size, const unsigned char* buffer)
    : tex(size, buffer) {
}

Texture::Texture(const void* data, int size) {
    if (!load_from_memory(data, size))
        EX_THROW("Failed to load texture from memory (" + std::to_string(size)+" bytes)");
}

Texture::Texture(const Image& image) {
    if (!load_from_image(image))
        EX_THROW("Failed to load texture from Image object");
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
    if (!data) {
        EX_ERROR("Failed to load image from file '" + path.string() + "'");
        return false;
    }

    tex = gl::Tex({ w, h }, data);
    stbi_image_free(data);
    return true;
}

bool Texture::load_from_memory(const void* data, int size) {
    int w, h, channels;
    unsigned char* image = stbi_load_from_memory(
        (const stbi_uc*) (data),
        size,
        &w, &h, &channels,
        4
    );

    if (!image) {
        EX_ERROR("Failed to load image from memory (" + std::to_string(size) + " bytes)");
        return false;
    }

    tex = gl::Tex({ w, h }, image);
    stbi_image_free(image);
    return true;
}

bool Texture::load_from_image(const Image& image) {
    if (image.get_size().x <= 0 || image.get_size().y <= 0 || !image.get_pixels()) {
        EX_ERROR("Cannot load texture from a empty image");
        return false;
    }

    tex = gl::Tex(image.get_size(), image.get_pixels());
    return true;
}

void Texture::set_data(Vec2i size, const unsigned char* buffer) {
    tex.set_data(size, buffer);
}

void Texture::update_sub(Vec2i offset, Vec2i sub_size, const unsigned char* data) {
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

void Texture::double_size() {
    tex.double_size();
}

int Texture::get_maximum_size() {
    return gl::Tex::get_maximum_size();
}

}
