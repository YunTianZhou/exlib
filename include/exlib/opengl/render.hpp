#pragma once

#include <GL/glew.h>

#include "exlib/opengl/types.hpp"

namespace ex::gl {

class VertexArray;
class IndexBuffer;
class Shader;

class EXLIB_API Render {
public:
	Render() = delete;

	static void draw_arrays(PrimitiveType type, const VertexArray& vao, const Shader& shader, GLint first = 0, GLsizei count = -1);
	static void draw_elements(PrimitiveType type, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLint first = 0, GLsizei count = -1);

};

}