#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "exlib/graphics/image.hpp"
#include "exlib/core/exception.hpp"

namespace ex {

Image::Image(Vec2i _size, Color color) {
    resize(_size, color);
}

Image::Image(Vec2i _size, const unsigned char* _pixels) {
    resize(_size, _pixels);
}

Image::Image(const std::filesystem::path& path) {
    if (!load_from_file(path))
        throw Exception("Failed to load image from file.");
}

Image::Image(const void* data, int _size) {
    if (!load_from_memory(data, _size))
        throw Exception("Failed to load image from memory.");
}

Image::Image(Image&& other)
    : size(other.size), pixels(std::move(other.pixels)) {
    other.size = Vec2i{ 0, 0 };
    other.pixels.clear();
}

Image& Image::operator=(Image&& other) {
    if (this != &other) {
        size = other.size;
        pixels = std::move(other.pixels);
        other.size = Vec2i{ 0, 0 };
        other.pixels.clear();
    }
    return *this;
}

Image Image::copy() const {
    Image result;
    result.size = size;
    result.pixels = pixels;
    return result;
}

bool Image::load_from_file(const std::filesystem::path& path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &channels, 4);
    if (!data) return false;

    resize({ w, h }, data);
    stbi_image_free(data);
    return true;
}

bool Image::load_from_memory(const void* data, int _size) {
    int w, h, channels;
    unsigned char* image = stbi_load_from_memory((const stbi_uc*) data, _size, &w, &h, &channels, 4);
    if (!image) return false;

    resize({ w, h }, image);
    stbi_image_free(image);
    return true;
}

bool Image::save_to_file(const std::filesystem::path& path) const {
    if (!pixels.empty() && size.x && size.y) {
        std::filesystem::path extension = path.extension();

        bool success = false;

        if (extension == ".bmp") {
            success = stbi_write_bmp(path.string().c_str(), size.x, size.y, 4, pixels.data());
        }
        else if (extension == ".tga") {
            success = stbi_write_tga(path.string().c_str(), size.x, size.y, 4, pixels.data());
        }
        else if (extension == ".png") {
            success = stbi_write_png(path.string().c_str(), size.x, size.y, 4, pixels.data(), 0);
        }
        else if (extension == ".jpg" || extension == ".jpeg") {
            success = stbi_write_jpg(path.string().c_str(), size.x, size.y, 4, pixels.data(), 90);
        }

        return success;
    }

    return false;
}

std::optional<std::vector<unsigned char>> Image::save_to_memory(const std::string& format) const {
    if (!pixels.empty() && size.x && size.y) {
        std::vector<unsigned char> buffer;

        auto write_to_vector = +[](void* context, void* data, int size) {
            auto* buf = (std::vector<unsigned char>*) (context);
            buf->insert(buf->end(), (unsigned char*) data, (unsigned char*) data + size);
        };

        std::string fmt = format;
        std::transform(fmt.begin(), fmt.end(), fmt.begin(), ::tolower);

        bool success = false;

        if (fmt == "png") {
            success = stbi_write_png_to_func(write_to_vector, &buffer, size.x, size.y, 4, pixels.data(), 0);
        }
        else if (fmt == "bmp") {
            success = stbi_write_bmp_to_func(write_to_vector, &buffer, size.x, size.y, 4, pixels.data());
        }
        else if (fmt == "tga") {
            success = stbi_write_tga_to_func(write_to_vector, &buffer, size.x, size.y, 4, pixels.data());
        }
        else if (fmt == "jpg" || fmt == "jpeg") {
            success = stbi_write_jpg_to_func(write_to_vector, &buffer, size.x, size.y, 4, pixels.data(), 90);
        }

        if (success)
            return buffer;
    }

    return std::nullopt;
}

Color Image::get_pixel(Vec2i pos) const {
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
        throw Exception("Pixel position out of the bounds of the image.");

    int index = (pos.y * size.x + pos.x) * 4;
    const unsigned char* ptr = &pixels[index];
    return Color{ ptr[0], ptr[1], ptr[2], ptr[3]};
}

Vec2i Image::get_size() const {
    return size;
}

const unsigned char* Image::get_pixels() const {
    if (pixels.empty()) 
        throw Exception("Image is empty.");

    return pixels.data();
}

void Image::set_pixel(Vec2i pos, Color color) {
    if (pos.x < 0 || pos.x >= size.x || pos.y < 0 || pos.y >= size.y)
        throw Exception("Pixel position out of the bounds of the image.");

    int index = (pos.y * size.x + pos.x) * 4;
    unsigned char* ptr = &pixels[index];
    *ptr++ = color.r;
    *ptr++ = color.g;
    *ptr++ = color.b;
    *ptr++ = color.a;
}

void Image::resize(Vec2i _size, Color color) {
    if (_size.x && _size.y) {
        std::vector<unsigned char> new_pixels(_size.x * _size.y * 4);

        unsigned char* ptr = new_pixels.data();
        unsigned char* end = ptr + new_pixels.size();

        while (ptr != end) {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }

        pixels = std::move(new_pixels);
        size = _size;
    }
    else {
        std::vector<unsigned char>().swap(pixels);
        size = Vec2i{ 0, 0 };
    }
}

void Image::resize(Vec2i _size, const unsigned char* _pixels) {
    if (_pixels && _size.x && _size.y) {
        std::vector<unsigned char> new_pixels(_pixels, _pixels + _size.x * _size.y * 4);
        pixels = std::move(new_pixels);
        size = _size;
    }
    else {
        std::vector<unsigned char>().swap(pixels);
        size = Vec2i{ 0, 0 };
    }
}

void Image::create_mask(Color color, unsigned char alpha) {
    unsigned char* ptr = pixels.data();
    unsigned char* end = ptr + pixels.size();

    while (ptr != end) {
        if (ptr[0] == color.r &&
            ptr[1] == color.g &&
            ptr[2] == color.b &&
            ptr[3] == color.a) {
            ptr[3] = alpha;
        }

        ptr += 4;
    }
}

void Image::flip_horizontally() {
    if (pixels.empty())
        return;

    int row_size = size.x * 4;

    for (int y = 0; y < size.y; y++) {
        unsigned char* row = pixels.data() + y * row_size;

        for (int x = 0; x < size.x / 2; x++) {
            unsigned char* left = row + x * 4;
            unsigned char* right = row + (size.x - 1 - x) * 4;
            std::swap_ranges(left, left + 4, right);
        }
    }
}

void Image::flip_vertically() {
    if (pixels.empty())
        return;

    int row_size = size.x * 4;

    unsigned char* top = pixels.data();
    unsigned char* bottom = pixels.data() + (size.y - 1) * row_size;

    for (int y = 0; y < size.y / 2; y++) {
        std::swap_ranges(top, top + row_size, bottom);

        top += row_size;
        bottom -= row_size;
    }
}

}
