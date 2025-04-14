#pragma once

#include <vector>
#include <optional>
#include <filesystem>

#include "exlib/core/types.hpp"

namespace ex {

class EXLIB_API Image {
public:
	// Constructors
	Image() = default;
	explicit Image(Vec2i _size, Color color = Color::White);
	Image(Vec2i _size, const unsigned char* _pixels);
	Image(const std::filesystem::path& path);
	Image(const void* data, int _size);

	// Copy and Move
	Image(const Image& other) = delete;
	Image& operator=(const Image& other) = delete;
	Image(Image&& other);
	Image& operator=(Image&& other);
	Image copy() const;

	// Loaders
	bool load_from_file(const std::filesystem::path& path);
	bool load_from_memory(const void* data, int _size);

	// Savers
	bool save_to_file(const std::filesystem::path& path) const;
	std::optional<std::vector<unsigned char>> save_to_memory(const std::string& format) const;

	// Getters
	Color get_pixel(Vec2i pos) const;
	Vec2i get_size() const;
	const unsigned char* get_pixels() const;

	// Setters
	void set_pixel(Vec2i pos, Color color);
    void resize(Vec2i _size, Color color = Color::Black);
    void resize(Vec2i _size, const unsigned char* _pixels);
	void create_mask(Color color, unsigned char alpha = 0);
	void flip_horizontally();
	void flip_vertically();

private:
	Vec2i size;
	std::vector<unsigned char> pixels;

};

}