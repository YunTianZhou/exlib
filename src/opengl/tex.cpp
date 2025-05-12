#include "exlib/opengl/tex.hpp"

namespace ex::gl {

Tex::Tex()
	: id(0), size() {
	glGenTextures(1, &id);
	if (id == 0)
		EX_THROW("Failed to generate OpenGL texture ID");
}

Tex::Tex(Vec2i _size, const unsigned char* buffer)
	: id(0), size(_size) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	set_default_parameters();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

Tex::~Tex() {
	if (id != 0)
		glDeleteTextures(1, &id);
}

Tex::Tex(Tex&& other)
	: id(other.id), size(other.size) {
	other.id = 0;
	other.size = Vec2i{ 0, 0 };
}

Tex& Tex::operator=(Tex&& other) {
	if (this != &other) {
		if (id != 0)
			glDeleteTextures(1, &id);
		id = other.id;
		size = other.size;
		other.id = 0;
		other.size = Vec2i{ 0, 0 };
	}
	return *this;
}

Tex Tex::copy() const {
	if (id == 0)
		EX_THROW("Texture not exist");

	Tex new_tex(size, nullptr);

	GLuint src_fbo = 0, dst_fbo = 0;
	glGenFramebuffers(1, &src_fbo);
	glGenFramebuffers(1, &dst_fbo);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, src_fbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, new_tex.id, 0);

	glBlitFramebuffer(
		0, 0, size.x, size.y,
		0, 0, size.x, size.y,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);

	glDeleteFramebuffers(1, &src_fbo);
	glDeleteFramebuffers(1, &dst_fbo);

	return new_tex;
}

void Tex::set_data(Vec2i _size, const unsigned char* buffer) {
	size = _size;

	if (id == 0)
		EX_THROW("Texture not exist");

	glBindTexture(GL_TEXTURE_2D, id);
	set_default_parameters();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void Tex::update_sub(const Vec2i& offset, const Vec2i& sub_size, const unsigned char* data) {
	if (id == 0)
		EX_THROW("Texture not exist");
	if (offset.x + sub_size.x > size.x ||
		offset.y + sub_size.y > size.y)
		EX_THROW("Sub update region out of range");

	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, sub_size.x, sub_size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Tex::set_filter(Filter min_filter, Filter mag_filter) {
	if (id == 0)
		EX_THROW("Texture not exist");

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) mag_filter);
}

void Tex::set_wrap(Wrap wrap_s, Wrap wrap_t) {
	if (id == 0)
		EX_THROW("Texture not exist");

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) wrap_t);
}

void Tex::generate_mipmaps() {
	if (id == 0)
		EX_THROW("Texture not exist");

	glBindTexture(GL_TEXTURE_2D, id);
	glGenerateMipmap(GL_TEXTURE_2D);
}

GLint Tex::get_maximum_size() {
	static const unsigned int size = [] {
		GLint value = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		return value;
	}();

	return size;
}

void Tex::double_size() {
	// 1) grab old dims
	const int oldW = size.x, oldH = size.y;
	const int newW = oldW * 2, newH = oldH * 2;

	// 2) create & allocate the new texture
	GLuint newTex = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &newTex);
	glTextureStorage2D(newTex,        // direct-state-access
		1,             // levels
		GL_RGBA8,      // internal format
		newW, newH);   // new size

	// 3) clear whole new texture to (0,0,0,0) on-GPU
	//    (only in GL 4.4+)
	const GLfloat clearColor[4] = { 0, 0, 0, 0 };
	glClearTexImage(newTex, 0, GL_RGBA, GL_FLOAT, clearColor);

	// 4) copy the old into the lower-left corner of the new
	//    (src texture, src target, src level, src x,y,z;
	//     dst texture, dst target, dst level, dst x,y,z;
	//     width, height, depth)
	glCopyImageSubData(
		id, GL_TEXTURE_2D, 0,   // src
		0, 0, 0,
		newTex, GL_TEXTURE_2D, 0,   // dst
		0, 0, 0,
		oldW, oldH, 1
	);

	// 5) tear down the old tex, adopt the new handle & dims
	glDeleteTextures(1, &id);
	id = newTex;
	size = { newW, newH };
}

void Tex::set_default_parameters() {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

}
