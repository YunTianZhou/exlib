#pragma once

#include <GL/glew.h>
#include "exlib/core/types.hpp"
#include "exlib/core/exception.hpp"

namespace ex::gl {

class EXLIB_API Texture {
public:
	// Filtering modes
	enum class Filter : GLint {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	// Wrapping modes
	enum class Wrap : GLint {
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER
	};

public:
	// Constructors and Destructors
	Texture();
	Texture(Vec2i _size, const unsigned char* buffer);
	~Texture();

	// Copy and Move
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other);
	Texture& operator=(Texture&& other);
	Texture copy() const;

	// Bind and Unbind
	inline void bind(GLuint slot = 0) const;
	inline void unbind() const;

	// Getters
	inline bool is_exist() const { return id != 0; }
	inline Vec2i get_size() const { return size; }

	// Setters
	void set_data(Vec2i _size, const unsigned char* buffer);
	void update_sub(const Vec2i& offset, const Vec2i& sub_size, const unsigned char* data);
	void set_filter(Filter min_filter, Filter mag_filter);
	void set_wrap(Wrap wrap_s, Wrap wrap_t);

	// Mipmaps
	void generate_mipmaps();

private:
	void set_default_parameters();

private:
	GLuint id;
	Vec2i size;
};

inline void Texture::bind(GLuint slot) const {
	if (!id) {
		EX_THROW("Texture not exist");
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

inline void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
